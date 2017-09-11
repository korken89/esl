# A collection of helper functions

## `singleton.hpp`: A simple Singleton helper

Makes it easy to generate singletons without the class it self needing to inherit / implement something.

Example usage:

```C++
#include <esl/helpers/singleton.hpp>

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
