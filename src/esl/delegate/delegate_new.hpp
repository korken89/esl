//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

namespace esl
{
//
// Base class definition and defaults
//
template < typename, std::size_t Size, std::size_t Align = alignof(void*) >
class delegate;

template < typename Ret, typename... Args, std::size_t Size, std::size_t Align >
class delegate< Ret(Args...), Size, Align >
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

  //
  // Variable template to generate a vtable (C++14)
  //
  template < typename F >
  constexpr static const vtable make_vtable = {caller< F >, destroyer< F >};

#else

  // Constexpr lambdas available, collapse the variable template (C++17)
  template < typename F >
  constexpr static const vtable make_vtable = {
      [](const void* fun, Args... args) -> Ret {  // caller
        return (*static_cast< const F* >(fun))(args...);
      },
      [](const void* fun) {  // destroyer
        static_cast< const F* >(fun)->~F();
      }};

#endif

  //
  // Storage
  //
  const vtable vtable_;
  std::aligned_storage_t< Size, Align > storage_;

public:
  // default constructors
  constexpr delegate(const delegate&) = default;
  constexpr delegate(delegate&&)      = default;
  constexpr delegate()                = delete;  // There is no empty delegate

  // assignment operators
  constexpr delegate& operator=(const delegate&) = default;
  constexpr delegate& operator=(delegate&&) = default;

  //
  // Explicit construction
  //
  template < typename F, typename = std::enable_if_t< !std::is_same<
                             std::decay_t< F >, delegate >::value > >
  explicit constexpr delegate(F&& fun)
      : vtable_{make_vtable< std::decay_t< F > >}
  {
    static_assert(sizeof(std::decay_t< F >) <= Size,
                  "The callable does not fit inside the delegate");
    new (&storage_) F{std::forward< F >(fun)};
  }

  //
  // Delete stuff when done
  //
  ~delegate()
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
  constexpr bool operator==(const delegate& other) const
  {
    return (vtable_.call == other.vtable_.call) &&
           (vtable_.destroy == other.vtable_.destroy);
  }

  //
  // Helpers to create delegates
  //

  // Make from Methods
  template < typename Obj >
  constexpr static delegate from(Obj& obj, Ret (Obj::*mptr)(Args...))
  {
    return delegate{
        [&obj, mptr](Args... args) -> Ret { return (obj.*mptr)(args...); }};
  }

  template < typename Obj, Ret (Obj::*mptr)(Args...) >
  constexpr static delegate from(Obj& obj)
  {
    return delegate{
        [&obj](Args... args) -> Ret { return (obj.*mptr)(args...); }};
  }

  // Make from Const methods
  template < typename Obj >
  constexpr static delegate from(Obj& obj, Ret (Obj::*mptr)(Args...) const)
  {
    return delegate{
        [&obj, mptr](Args... args) -> Ret { return (obj.*mptr)(args...); }};
  }

  template < typename Obj, Ret (Obj::*mptr)(Args...) const >
  constexpr static delegate from(Obj& obj)
  {
    return delegate{
        [&obj](Args... args) -> Ret { return (obj.*mptr)(args...); }};
  }

  // Make from Functions
  constexpr static delegate from(Ret (&fptr)(Args...))
  {
    return delegate{[&fptr](Args... args) -> Ret { return (fptr)(args...); }};
  }

  template < Ret (*fptr)(Args...) >
  constexpr static delegate from()
  {
    static_assert(fptr != nullptr, "Function pointer must not be null");

    return delegate{[](Args... args) -> Ret { return (*fptr)(args...); }};
  }
};
}  // end namespace esl

// For Cortex-M, sizeof(void*) bytes local storage is enough for function
// pointers and method pointers that are know at compile time.
// For runtime method pointers, 3*sizeof(void*) is needed, or if
// larger captures are desired more can be used.

// using my_delegate = esl::delegate< void(int), sizeof(void *) >;
