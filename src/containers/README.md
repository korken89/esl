# A collection of static containers for C++

Static sized containers, meant for use on embedded platforms without memory allocation.

## `static_vector`

An vector with a max size, statically allocated. Has optional bounds checking.

## `ring_buffer`

A ring_buffer with a max size, statically allocated. Has optional bounds checking.
Note, this simple implementation has that the maximum number of elements is N-1.
