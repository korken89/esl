//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>

namespace esl
{
template < typename T >
class delegate;

template < typename R, typename... Params >
class delegate< R(Params...) >
{
private:
  using CallbackType = R (*)(void* callee, Params...);

  void* obj_;
  CallbackType cb_;

  // method caller
  template < class T, R (T::*Mptr)(Params...) >
  constexpr static R invoke_method(void* obj, Params... params) noexcept(
      noexcept((static_cast< T* >(obj)->*Mptr)(params...)))
  {
    return (static_cast< T* >(obj)->*Mptr)(params...);
  }

  // const method caller
  template < class T, R (T::*Mptr)(Params...) const >
  constexpr static R invoke_method(void* obj, Params... params) noexcept(
      noexcept((static_cast< T* >(obj)->*Mptr)(params...)))
  {
    return (static_cast< T* >(obj)->*Mptr)(params...);
  }

  // function caller
  template < R (*Fptr)(Params...) >
  constexpr static R invoke_function(void*, Params... params) noexcept(
      noexcept((*Fptr)(params...)))
  {
    return (*Fptr)(params...);
  }

public:
  constexpr delegate(void* obj, CallbackType callback) noexcept : obj_(obj),
                                                                  cb_(callback)
  {
  }

  // from method
  template < class T, R (T::*Mptr)(Params...) >
  constexpr static delegate from_method(T& obj) noexcept
  {
    return delegate(&obj, &invoke_method< T, Mptr >);
  }

  // from const method
  template < class T, R (T::*Mptr)(Params...) const >
  constexpr static delegate from_method(T& obj) noexcept
  {
    return delegate(&obj, &invoke_method< T, Mptr >);
  }

  // from function
  template < R (*Fptr)(Params...) >
  constexpr static delegate from_function() noexcept
  {
    return delegate(nullptr, &invoke_function< Fptr >);
  }

  // invoke delegate via operator ()
  template < typename... Args >
  constexpr auto operator()(Args&&... params) const
      noexcept(noexcept((*cb_)(obj_, std::forward< Args >(params)...)))
  {
    return (*cb_)(obj_, std::forward< Args >(params)...);
  }

  // comparison operators
  constexpr bool operator==(const delegate& other) const noexcept
  {
    return (obj_ == other.obj_) && (cb_ == other.cb_);
  }

  constexpr bool operator!=(const delegate& other) const noexcept
  {
    return !((*this) == other);
  }
};

}  // namespace esl
