# A collection of static containers for C++

Static sized containers, meant for use on embedded platforms without memory allocation.

## `allocate.hpp`

A helper for generating storage for a `Container`, usage example in `static_vector`.

## `static_vector.hpp`

An vector with a max size, that is statically allocated. It supports most of the operations that `std::vector` has, which includes:

#### Access:

* `front`
* `back`
* `operator[]`
* `data`

#### Size info:

* `size`
* `capacity`
* `free`
* `empty`
* `full`

#### Iterators:

* `begin`
* `cbegin`
* `end`
* `cend`

#### Adding elements:

* `push_back`
* `emplace_back`

#### Erasing elements:

* `clear`
* `erase`

### Usage

The `static_vector` does not have storage itself, as can be seen from its constructor:

```C++
static_vector(T *buffer, size_type capacity)
```

It expects a buffer to be given to it at construction. This comes from a design decision that makes it possible to have functions take a `static_vector` and not be dependent on its size.

To construct a `static_vector` including storage there is the `allocate` helper which inherits a `Container` together with `aligned_storage` which is implicitly convertible to the `Container`.

Example usage of `allocate` for creating a `static_vector` with storage:

```C++
using namespace esl;

void foo(static_vector< int > &v)
{
  // ...
}

allocate< static_vector< int >, 10 > my_vec;
//        ^^^^ Container ^^^^   ^^ Buffer Size

int main()
{
  foo(my_vec);

  // ...
}

```

## `ring_buffer.hpp`

A `ring_buffer` with a max size which refers to a statically allocated buffer. It has optional bounds checking.

**Note:** This implementation has that the maximum number of elements that can be stored is N-1. This to not have a full/empty flag which couples the writing and reading, it is thread safe to read and write to the buffer.

### Usage

#### Access:

* `front`
* `back`

#### Size info:

* `size`
* `capacity`
* `free`
* `empty`
* `full`

#### Adding elements:

* `push_back`
* `emplace_back`

#### Erasing elements:

* `clear`
* `pop`


The `static_vector` does not have storage itself, as can be seen from its constructor:

```C++
static_vector(T *buffer, size_type capacity)
```
