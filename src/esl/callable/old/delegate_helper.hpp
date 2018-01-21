//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace esl
{

// Helper to extract function signature
template < typename T >
struct FunctionInfo;

template < typename R, typename... A >
struct FunctionInfo< R (*)(A...) >  // function pointer
{
  using Signature = R(A...);
};

// Helper to extract method signature + class type
template < typename T >
struct MethodInfo;

template < typename C, typename R, typename... A >
struct MethodInfo< R (C::*)(A...) >  // method pointer
{
  using ClassType = C;
  using Signature = R(A...);
};

// Specialization for const methods
template < typename C, typename R, typename... A >
struct MethodInfo< R (C::*)(A...) const > : MethodInfo< R (C::*)(A...) >
{
};

} // namespace esl

// Generate function delegate
#define DELEGATE_FUNCTION(fptr)                               \
  [&]() {                                                     \
    using minfo = esl::FunctionInfo< decltype(fptr) >;        \
    using del   = esl::delegate< typename minfo::Signature >; \
    return del::from_function< (fptr) >();                    \
  }()

// Generate method delegate
#define DELEGATE_METHOD(mptr, obj)                                     \
  [&]() {                                                              \
    using minfo = esl::MethodInfo< decltype(mptr) >;                   \
    using del   = esl::delegate< typename minfo::Signature >;          \
    return del::from_method< typename minfo::ClassType, (mptr) >(obj); \
  }()
