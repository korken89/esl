# Embedded "Standard" Library (esl)

My collection of good to have algorithms, containers, etc, for embedded use.
Active WIP.

## Status

[![codecov](https://codecov.io/gh/korken89/esl/branch/master/graph/badge.svg)](https://codecov.io/gh/korken89/esl) [![CircleCI](https://circleci.com/gh/korken89/esl.svg?style=svg)](https://circleci.com/gh/korken89/esl) [![LICENSE](https://img.shields.io/badge/license-BSL%201.0-blue.svg)](LICENSE.md)

## Standard

Designed for C++14 or higher.

## Include

To use this library, add `./src` to your includes and:

```C++
#include <esl/esl.hpp>
```

## Parts

#### Containers

Currently there is a `static_vector` and a `ring_buffer` see the local [README](src/esl/containers/README.md) for more information and usage.

#### Callable

Light-weight `std::function` in a sense with both an owning and non-owning implementation, designed for embedded use, see the local [README](src/esl/callable/README.md) for more information and usage.

#### Math functions

Currently there is a `vector` (in a mathematical sense) and a `quaternion` implementation, see the local [README](src/esl/math/README.md) for more information and usage.

#### Helper functions

Currently there is a `repeat` (compile-time loop unrolling), `singleton` helper and a `flag_enum` helper, see the local [README](src/esl/helpers/README.md) for more information and usage.

---

## License

Uses the Boost Software License 1.0

---

## IDEAS

* Add a script that checks the map file for linked in libs that are not allowed such as double functions
