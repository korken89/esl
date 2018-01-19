# A collection of static containers for C++

Static sized containers, meant for use on embedded platforms without memory allocation.

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

* `erase`

### Usage

The `static_vector` does not have storage itself, as can be seen from its constructor:

```C++
static_vector(T *buffer, size_type capacity)
```

It expects a buffer to be given to it at construction. This comes from a design decision that makes it possible to have functions take a `static_vector` and not be dependent on its size.

To construct a `static_vector` including storage there is the `allocator` helper which inherits a `Container` together with `aligned_storage` which is implicitly convertible to the `Container`.

Example usage of `allocator` for creating a `static_vector` with storage:

```C++
using namespace esl;

void foo(static_vector< int > &v)
{
  // ...
}

allocator< static_vector< int >, 10 > my_vec;
//         ^^^^ Container ^^^^   ^^ Buffer Size

int main()
{
  foo(my_vec);

  // ...
}

```

## `ring_buffer.hpp`

A ring_buffer with a max size, statically allocated. Has optional bounds checking.
Note, this simple implementation has that the maximum number of elements is N-1.
