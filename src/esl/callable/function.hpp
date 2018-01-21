//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

// For Cortex-M, sizeof(void*) bytes local storage is enough for function
// pointers and method pointers that are know at compile time.
// For runtime method pointers, 3*sizeof(void*) is needed, or if
// larger captures are desired more can be used.

namespace esl
{
//
// Base class definition and defaults
//
template < typename, std::size_t Size = sizeof(void*),
           std::size_t Align = alignof(void*) >
class function;

template < typename Ret, typename... Args, std::size_t Size, std::size_t Align >
class function< Ret(Args...), Size, Align >
{
private:
  //
  // Local "vtable" definition
  //
  struct vtable
  {
    Ret (*call)(const void*, Args...);
    void (*destroy)(const void*);
  };

// Check for constexpr lambdas
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201603)

  // No constexpr lambdas, use statics to populate the vtable
  template < typename F >
  constexpr static Ret caller(const void* fun, Args... args)
  {
    return (*static_cast< const F* >(fun))(args...);
  }

  template < typename F >
  constexpr static void destroyer(const void* fun)
  {
    static_cast< const F* >(fun)->~F();
  }

  template < typename F >
  constexpr vtable make_vtable() const noexcept
  {
    return {
        caller< F >,    // caller
        destroyer< F >  // destroyer
    };
  }

#else

  // Constexpr lambdas, use lambdas to populate the vtable
  template < typename F >
  constexpr vtable make_vtable() const noexcept
  {
    return {[](const void* fun, Args... args) -> Ret {  // caller
              return (*static_cast< const F* >(fun))(args...);
            },
            [](const void* fun) {  // destroyer
              static_cast< const F* >(fun)->~F();
            }};
  }

#endif

  //
  // Storage
  //
  vtable vtable_;
  std::aligned_storage_t< Size, Align > storage_;

public:
  // default constructors
  constexpr function(const function&) = default;
  constexpr function(function&&)      = default;
  constexpr function()                = delete;  // There is no empty function

  // assignment operators
  constexpr function& operator=(const function&) = default;
  constexpr function& operator=(function&&) = default;

  //
  // Explicit construction
  //
  template < typename F, typename = std::enable_if_t<
                             !std::is_convertible< F, function >::value > >
  explicit constexpr function(F&& fun)
      : vtable_{make_vtable< std::decay_t< F > >()}
  {
    static_assert(sizeof(std::decay_t< F >) <= Size,
                  "The callable does not fit inside the function");

    new (&storage_) F{std::forward< F >(fun)};
  }

  //
  // Delete stuff when done
  //
  ~function()
  {
    vtable_.destroy(&storage_);
  }

  //
  // Call using operator()
  //
  template < typename... Ts >
  constexpr Ret operator()(Ts&&... args) const
  {
    return vtable_.call(&storage_, std::forward< Ts >(args)...);
  }

  //
  // Operators
  //
  constexpr bool operator==(const function& other) const noexcept
  {
    return (vtable_.call == other.vtable_.call) &&
           (vtable_.destroy == other.vtable_.destroy);
  }

  constexpr bool operator!=(const function& other) const noexcept
  {
    return !(*this == other);
  }

  //
  // Helpers to create function
  //

  // Make from Methods
  template < typename Obj >
  constexpr static function from(Obj& obj, Ret (Obj::*mptr)(Args...)) noexcept
  {
    return function{[&obj, mptr](Args... args) -> Ret {
      return (obj.*mptr)(args...);  // call method
    }};
  }

  template < typename Obj, Ret (Obj::*mptr)(Args...) >
  constexpr static function from(Obj& obj) noexcept
  {
    return function{[&obj](Args... args) -> Ret {
      return (obj.*mptr)(args...);  // call method
    }};
  }

  // Make from Const methods
  template < typename Obj >
  constexpr static function from(Obj& obj,
                                 Ret (Obj::*mptr)(Args...) const) noexcept
  {
    return function{[&obj, mptr](Args... args) -> Ret {
      return (obj.*mptr)(args...);  // call method
    }};
  }

  template < typename Obj, Ret (Obj::*mptr)(Args...) const >
  constexpr static function from(Obj& obj) noexcept
  {
    return function{[&obj](Args... args) -> Ret {
      return (obj.*mptr)(args...);  // call method
    }};
  }

  // Make from Functions
  constexpr static function from(Ret (&fptr)(Args...)) noexcept
  {
    return function{[&fptr](Args... args) -> Ret {
      return (fptr)(args...);  // call function
    }};
  }

  template < Ret (*fptr)(Args...) >
  constexpr static function from() noexcept
  {
    static_assert(fptr != nullptr, "Function pointer must not be null");

    return function{[](Args... args) -> Ret {
      return (*fptr)(args...);  // call function
    }};
  }
};
}  // end namespace esl
