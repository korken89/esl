# Embedded "Standard" Library (esl)

My collection of good to have algorithms, containers, etc, for embedded use.
Active WIP.

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

#### Delegate

A light-weight `std::function` in a sense, designed for embedded use, see the local [README](src/esl/delegate/README.md) for more information and usage.

#### Math functions

Currently there is a `vector` implementation (in a mathematical sense), see the local [README](src/esl/math/README.md) for more information and usage.

#### Helper functions

Currently there is a `singleton` helper and a `flag_enum` helper, see the local [README](src/esl/helpers/README.md) for more information and usage.

---

## License

Uses the Boost Software License 1.0

## Status

[![Build Status](https://travis-ci.org/korken89/esl.svg?branch=master)](https://travis-ci.org/korken89/esl)


## TODO

* Add documentation

## IDEAS

* Add a script that checks the map file for linked in libs that are not allowed such as double functions
