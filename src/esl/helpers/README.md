# A collection of helper functions

## `singleton.hpp`: A simple Singleton helper

Makes it easy to generate singletons without the class it self needing to inherit / implement something.

Example usage:

```C++
#include <esl/esl.hpp>

// Simple class, no singleton code
struct my_test
{
  int a, b, c;

  my_test() : a(1), b(2), c(3) { }
};

int main()
{
  // Generate a singleton from the "my_test" class and get its instance
  auto &inst = esl::get_singleton< my_test >();

  // Use as normal
  inst.c = 100;

  // ...
}
```

## `enum_flag.hpp`: A simple emum flag

Is used to replace `flags |= flag`, `flags &= ~flag`, or `if (flags & flag)` type of code with a type-safe replacement.

Example usage:

```C++
#include <esl/esl.hpp>

// Flags
enum class flags
{
  A, // Note: no explicit value (internal representation can be seen as
  B, // shifting based, so one bit per flag)
  C
};

int main()
{
  esl::enum_flag< flags > f; // Can also be given flags that start with true

  // Clearing all flags
  f.clear();

  // Setting flags
  f.set(flags::A, flags::B);

  // Clearing flags
  f.clear(flags::B, flags::C);

  if (f.any(flags::A, flags::C))      // Checks if any of the specified flags are set
    return 0;
  else if (f.all(flags::A, flags::B)) // Checks if all of the specified flags are set
    return 1;
  else if (f.none())                  // Checks if no flags are set
    return 2;
  else
    return -1;
}
```

## `repeat`: Compile-time repeat

Is used to repeat a call multiple times, can be used as compile-time loop unrolling.

Example usage:

```C++
#include <esl/esl.hpp>
#include <iostream>

int main()
{
  esl::repeat([](auto idx) {
    std::cout << "repeat index " << idx << "\n";
  });

  return 0;
}
```
