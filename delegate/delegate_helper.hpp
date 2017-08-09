#pragma once

namespace esl
{

// Helper to extract function signature
template<class T>
struct FunctionInfo;

template<class R, class... A>
struct FunctionInfo<R(*)(A...)> // function pointer
{
    using Signature = R(A...);
};


// Helper to extract method signature + class type
template<class T>
struct MethodInfo;

template<class C, class R, class... A>
struct MethodInfo<R(C::*)(A...)> //method pointer
{
    using ClassType = C;
    using Signature = R(A...);
};

// Specialization for const methods
template<class C, class R, class... A>
struct MethodInfo<R(C::*)(A...) const> : MethodInfo<R(C::*)(A...)> {};

}

// Generate function delegate
#define DELEGATE_FUNCTION(fptr) [&](){                                    \
    using minfo = esl::FunctionInfo<decltype(fptr)>;                      \
    using del = esl::delegate< typename minfo::Signature >;               \
    return del::from_function<(fptr)>(); }()

// Generate method delegate
#define DELEGATE_METHOD(mptr, obj) [&](){                                 \
    using minfo = esl::MethodInfo<decltype(mptr)>;                        \
    using del = esl::delegate< typename minfo::Signature >;               \
    return del::from_method<typename minfo::ClassType, (mptr)>(obj); }()

