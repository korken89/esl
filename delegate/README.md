# A fast and simple delegate implementation

## Aims

* Fast and light weight
* No usage of dynamic memory allocation

## Usage

Defining a delegate:
```C++
using cb = delegate< return_type (parameters...) >;
```

Storing a delegate (made from a function):
```C++
int bar(int);

auto d1 = delegate< int(int) >::from<bar>();
```

Storing a delegate (made from a method):
```C++
struct foo {
  int bar(int);
};

foo myFoo;

auto d1 = delegate< int(int) >::from<foo, &foo::bar>(myFoo);
```

Invoking any of the previous delegates:
```C++
auto r = d1(10);
```


## References

Based on [The Impossibly Fast C++ Delegates](https://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates).
