# A collection of static containers for C++

Static sized containers, meant for use on embedded platforms without memory allocation.

## `allocate.hpp`

A helper for generating storage for a `Container`, example:

```C++
allocate< my_container, 5 > c;
```

This will make allocate inherit from `my_container`, where `my_container` must define the `value_type`. `allocate` will create an `aligned_storage` based on `value_type` and the `size` that will be sent to the constructor of `container`. Conceptually like this:

```C++
template < typename Container, std::size_t Capacity >
class allocate : public Container
{
  std::aligned_storage_t< ... > buffer_[Capacity];

public:
  allocate(args...) : Container(buffer_, Capacity, args...)
  {
  }
};
```

If a container needs to be used with `allocate`, then the following things should be done:

* The container must have a type named `value_type`, the `aligned_storage` is based on this.
* The container's constructor must be of the form `container(buffer, capacity, args...)`
* If there a need to have constraints on the capacity, the following trait needs to be specialized:

```C++
template < typename, std::size_t >
struct allocate_capacity_check : std::true_type
{
};

// Example - max uint8
template < typename T, std::size_t Capacity >
struct allocate_capacity_check< my_container< T >, Capacity >
    : std::integral_constant<bool, (Capacity < 256) >
{
  static_assert(Capacity < 256, "Capacity cannot be more than 256.");
};
```

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

A `ring_buffer` with a max size which refers to a statically allocated buffer, the same as `static_vector`. It has optional bounds checking.

### Note

* This implementation has that the maximum number of elements that can be stored is N-1. This to not have a full/empty flag which couples the writing and reading, it is thread safe to read and write to the buffer.
* This implementation is designed for types without destructor.
* Only accepts sizes in powers of 2, will give compile error else (when using `allocate`).

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

### Example

```C++
using namespace esl;

void foo(ring_buffer< int > &buf)
{
  // ...
}

allocate< ring_buffer< int >, 16 > my_buf;
//        ^^^^ Container ^^^  ^^ Buffer Size

int main()
{
  foo(my_buf);

  // ...
}
```
