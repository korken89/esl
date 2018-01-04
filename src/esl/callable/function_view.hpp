//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <memory>
#include <type_traits>

namespace esl
{
//
// Base class definition and defaults
//
template < typename >
class function_view;

template < typename Ret, typename... Args >
class function_view< Ret(Args...) >
{
private:
  using Caller = Ret (*)(void*, Args...);

  void* object_;
  Caller caller_;

  //
  // constructor - private so users must use factory functions
  //
  constexpr function_view(void* obj, Caller caller) noexcept : object_(obj),
                                                               caller_(caller)
  {
  }

public:
  // default constructors
  constexpr function_view(const function_view&) = default;
  constexpr function_view(function_view&&)      = default;
  constexpr function_view() = delete;  // There is no empty function_view

  // assignment operators
  constexpr function_view& operator=(const function_view&) = default;
  constexpr function_view& operator=(function_view&&) = default;

  //
  // Construction from callable
  //
  template < typename F, typename = std::enable_if_t<
                             !std::is_convertible< F, function_view >::value > >
  constexpr function_view(F&& fun) noexcept
  {
    *this = function_view::from(std::forward< F >(fun));
  }

  //
  // Call using operator()
  //
  template < typename... Ts >
  constexpr Ret operator()(Ts&&... args) const
      noexcept(noexcept(caller_(object_, std::forward< Ts >(args)...)))
  {
    return caller_(object_, std::forward< Ts >(args)...);
  }

  //
  // Operators
  //
  constexpr bool operator==(const function_view& other) const noexcept
  {
    return (caller_ == other.caller_) && (object_ == other.object_);
  }

  constexpr bool operator!=(const function_view& other) const noexcept
  {
    return !(*this == other);
  }

  //
  // Helpers to create function_views
  //

  // Make from Functions
  constexpr static function_view from(Ret (*fptr)(Args...)) noexcept
  {
    return function_view{reinterpret_cast< void* >(fptr),  // save function
                         [](void* object, Args... args) -> Ret {
                           return reinterpret_cast< Ret (*)(Args...) >(object)(
                               args...);  // cast back and call function
                         }};
  }

  template < Ret (*Fptr)(Args...) >
  constexpr static function_view from() noexcept
  {
    static_assert(Fptr != nullptr, "Function pointer must not be null");

    return function_view::from(Fptr);
  }

  // Make from Methods
  template < typename Obj, Ret (Obj::*Mptr)(Args...) >
  constexpr static function_view from(Obj& obj) noexcept
  {
    return function_view{
        std::addressof(obj),  // save object
        [](void* object, Args... args) -> Ret {
          return (static_cast< std::add_pointer_t< Obj > >(object)->*Mptr)(
              args...);
        }};
  }

  // Make from Const methods
  template < typename Obj, Ret (Obj::*Mptr)(Args...) const >
  constexpr static function_view from(Obj& obj) noexcept
  {
    return function_view{
        std::addressof(obj),  // save object
        [](void* object, Args... args) -> Ret {
          return (static_cast< std::add_pointer_t< Obj > >(object)->*Mptr)(
              args...);
        }};
  }
};
}  // end namespace esl
