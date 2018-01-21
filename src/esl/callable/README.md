# A fast and simple `function` and `function_view` implementation

### Overall aims

* Replacement for `std::function` for embedded systems
* Fast and light weight
* No usage of dynamic memory allocation
* No default constructor, there is no such thing as an empty callable

## `function.hpp`

### Extras

* **OWNS** the callable
* Uses a fixed size storage for the callable
* Has default alignment of void *

### Usage

Defining a `function`:
```C++
using cb = function< return_type (parameters...), storage size, alignment = alignof(void *) >;

// Example - define a function
using my_function = function< int(int), sizeof(void *) >;

auto d1 = my_function(<any callable>);

// Lambdas without capture
auto d2 = my_function([](int i){ return i * i; });

// Lambdas can have capture
auto d3 = my_function([&some_var](int i){ return i * i + some_var; });
```

#### Helpers

A `function` made from a function:
```C++
int bar(int);

// Compile-time known
auto d1 = my_function::from<bar>();

// Runtime known
auto d2 = my_function(bar);
```

A `function` made from a method:
```C++
struct foo {
  int bar(int);
};

foo myFoo;

// Compile-time known
auto d1 = my_function::from<foo, &foo::bar>(myFoo);

// Runtime known
auto d2 = my_function::from(myFoo, &foo::bar);
```

Invoking any of the previous:
```C++
auto r = dX(10);
```

## `function_view.hpp`

### Extra

* Does **NOT** own the callable

### Usage

Defining a `function_view`:
```C++
using cb = function_view< return_type (parameters...) >;

// Example
using my_func = function_view< int(int) >;

auto d1 = my_func(<any callable that decays to a function pointer>);
```

#### Helpers

A `function_view` made from a function:
```C++
int bar(int);

// Compile-time known
auto d1 = my_func::from<bar>();

// Runtime known
auto d2 = my_func::from(bar);
```

A `function_view` made from a method:
```C++
struct foo {
  int bar(int);
};

foo myFoo;

// Compile-time known
auto d1 = my_func::from<foo, &foo::bar>(myFoo);
```

A `function_view` made from a lambda **without** capture:
```C++
// Lambda without capture is implicitly convertible to a function pointer
auto d1 = my_func::from([](int i){ return i * i; });
```
If the lambda needs capture, then `function` should be used.

Invoking any of the previous:
```C++
auto r = dX(10);
```
