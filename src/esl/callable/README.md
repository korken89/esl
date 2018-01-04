# A fast and simple `delegate` and `function_view` implementation

## `delegate`

### Aims

* Replacement for `std::function` for embedded systems
* OWNS the callable
* Fast and light weight
* No usage of dynamic memory allocation
* No default constructor, there is no such thing as an empty delegate
* Uses a fixed size storage for callables

### Usage

Defining a delegate:
```C++
using cb = delegate< return_type (parameters...), storage size, alignment >;

// Example
using callback = delegate< int(int), sizeof(void *) >;
```

Storing a delegate (made from a function):
```C++
int bar(int);

auto d1 = callback::from<bar>();
```

Storing a delegate (made from a method):
```C++
struct foo {
  int bar(int);
};

foo myFoo;

auto d1 = callback::from<foo, &foo::bar>(myFoo);
```

Storing a delegate (from lambda):
```C++
auto d1 = callback([](int i){ return i * i; });

// Lambdas can have capture
auto d2 = callback([&some_var](int i){ return i * i + some_var; });
```

Invoking any of the previous:
```C++
auto r = d1(10);
```

## `function_view`

### Aims

* Replacement for `std::function` for embedded systems
* Does NOT own the callable
* Fast and light weight
* No usage of dynamic memory allocation
* No default constructor, there is no such thing as an empty delegate

### Usage

Defining a `function_view`:
```C++
using cb = function_view< return_type (parameters...) >;

// Example
using callback = function_view< int(int) >;
```

Storing a `function_view` (made from a function):
```C++
int bar(int);

auto d1 = callback::from<bar>();
```

Storing a `function_view` (made from a method):
```C++
struct foo {
  int bar(int);
};

foo myFoo;

auto d1 = callback::from<foo, &foo::bar>(myFoo);
```

Invoking any of the previous:
```C++
auto r = d1(10);
```