//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>
#include <type_traits>

namespace esl
{
template < typename T >
class delegate;

template < typename R, typename... Params >
class delegate< R(Params...) > final
{
private:
  using CallbackType = R (*)(void*, Params...);

  using FunctionPtr = R (*)(Params...);

  template < typename Object >
  using MethodPtr = R (Object::*)(Params...);

  template < typename Object >
  using ConstMethodPtr = R (Object::*)(Params...) const;

  void* obj_;
  CallbackType cb_;

  // method caller
  template < typename Object, MethodPtr< Object > Mptr >
  constexpr static R invoke_method(void* obj, Params... params) noexcept(
      noexcept(
          (static_cast< std::add_pointer_t< Object > >(obj)->*Mptr)(params...)))
  {
    return (static_cast< std::add_pointer_t< Object > >(obj)->*Mptr)(params...);
  }

  // const method caller
  template < typename Object, ConstMethodPtr< Object > Mptr >
  constexpr static R invoke_method(void* obj, Params... params) noexcept(
      noexcept(
          (static_cast< std::add_pointer_t< Object > >(obj)->*Mptr)(params...)))
  {
    return (static_cast< std::add_pointer_t< Object > >(obj)->*Mptr)(params...);
  }

  // function caller
  template < FunctionPtr Fptr >
  constexpr static R invoke_function(void*, Params... params) noexcept(
      noexcept((*Fptr)(params...)))
  {
    return (*Fptr)(params...);
  }

  // constructor - private so users must use factory functions
  constexpr delegate(void* obj, CallbackType callback) noexcept
      : obj_(obj), cb_(callback)
  {
  }

public:
  // default constructors
  delegate(const delegate&) = default;
  delegate(delegate&&) = default;

  // assignment operators
  delegate& operator=(const delegate&) = default;
  delegate& operator=(delegate&&) = default;

  // from method
  template < typename Object, MethodPtr< Object > Mptr >
  constexpr static auto from(Object& obj) noexcept
  {
    return delegate(std::addressof(obj), &invoke_method< Object, Mptr >);
  }

  // from const method
  template < typename Object, ConstMethodPtr< Object > Mptr >
  constexpr static auto from(Object& obj) noexcept
  {
    return delegate(std::addressof(obj), &invoke_method< Object, Mptr >);
  }

  // from function
  template < FunctionPtr Fptr >
  constexpr static auto from() noexcept
  {
    static_assert(Fptr != nullptr, "Function pointer must not be null");

    return delegate(nullptr, &invoke_function< Fptr >);
  }

  // invoke delegate via operator ()
  template < typename... Args >
  constexpr auto operator()(Args&&... params) const
      noexcept(noexcept((*cb_)(obj_, std::forward< Args >(params)...)))
  {
    static_assert(sizeof...(Args) == sizeof...(Params),
                  "Wrong number of parameters");

    return (*cb_)(obj_, std::forward< Args >(params)...);
  }

  // comparison operators
  constexpr bool operator==(const delegate& other) const noexcept
  {
    return (obj_ == other.obj_) && (cb_ == other.cb_);
  }

  constexpr bool operator!=(const delegate& other) const noexcept
  {
    return (obj_ != other.obj_) || (cb_ != other.cb_);
  }
};

}  // namespace esl
