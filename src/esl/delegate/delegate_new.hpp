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
    Ret (*call)(void*, Args...);
    void (*destroy)(void*);
  };

// Check for constexpr lambdas
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201603)

  // No constexpr lambdas, use statics to populate the vtable
  template < typename F >
  constexpr static Ret caller(void* fun, Args... args)
  {
    return (*static_cast< F* >(fun))(args...);
  }

  template < typename F >
  constexpr static void destroyer(void* fun)
  {
    static_cast< F* >(fun)->~F();
  }

  //
  // Variable template to generate a vtable
  //
  template < typename F >
  constexpr static const vtable make_vtable =
  {
    caller< F >,
    destroyer< F >
  };

#else

  // Constexpr lambdas available, collapse to a variable template (C++17)
  template < typename F >
  constexpr static const vtable make_vtable =
  {
    [](void* fun, Args... args) -> Ret { // caller
      return (*static_cast< F* >(fun))(args...);
    },
    [](void* fun) { // destroyer
      static_cast< F* >(fun)->~F();
    }
  };

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
  constexpr delegate()                = delete; // There is no empty delegate

  // assignment operators
  constexpr delegate& operator=(const delegate&) = default;
  constexpr delegate& operator=(delegate&&) = default;

  //
  // Explicit construction
  //
  template < typename F,
             typename = std::enable_if_t< !std::is_same<std::decay_t<F>, delegate>::value > >
  constexpr delegate(F&& fun) : vtable_{make_vtable< F >}
  {
    static_assert(sizeof(F) <= Size,
                  "The callable does not fit inside the delegate");
    new (&storage_) F(std::forward< F >(fun));
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
  template <typename... Ts>
  constexpr Ret operator()(Ts&&... args)
  {
    return vtable_.call(&storage_, std::forward< Args >(args)...);
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

  // Methods
  template < typename Obj >
  constexpr static delegate from(Obj& obj, Ret (Obj::*mptr)(Args...))
  {
    return delegate{[&obj, mptr](Args... args) -> Ret {
      return (obj.*mptr)(args...);
    }};
  }

  template < typename Obj, Ret (Obj::*mptr)(Args...) >
  constexpr static delegate from(Obj& obj)
  {    return delegate{[&obj](Args... args) -> Ret {
      return (obj.*mptr)(args...);
    }};
  }

  // Const methods
  template < typename Obj >
  constexpr static delegate from(Obj& obj, Ret (Obj::*mptr)(Args...) const)
  {
    return delegate{[&obj, mptr](Args... args) -> Ret {
      return (obj.*mptr)(args...);
    }};
  }

  template < typename Obj, Ret (Obj::*mptr)(Args...) const >
  constexpr static delegate from(Obj& obj)
  {
    return delegate{[&obj](Args... args) -> Ret {
      return (obj.*mptr)(args...);
    }};
  }

  // Functions
  constexpr static delegate from(Ret (&fptr)(Args...))
  {
    return delegate{[&fptr](Args... args) -> Ret {
      return (fptr)(args...);
    }};
  }

  template < Ret (*fptr)(Args...) >
  constexpr static delegate from()
  {
    static_assert(fptr != nullptr, "Function pointer must not be null");

    return delegate{[](Args... args) -> Ret {
      return (*fptr)(args...);
    }};
  }
};
}  // end namespace esl

//
// Test code
//
// struct foo
// {
//   void bar(int i) const
//   {
//     (void)i;
//   }
// };
//
// void bar(int i)
// {
//   (void)i;
// }

// For Cortex-M, 4 bytes local storage is enough for function pointers
// and method pointers that are know at compile time.
// For runtime method pointers, 12 is needed, or if
// larger captures are desired more can be used.
// using my_delegate = esl::delegate< void(int), sizeof(void *) >;
