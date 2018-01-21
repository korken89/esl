# A collection of math for C++

## `vector.hpp`

An vector (in a mathematical sense) which works as expected.

### Example

```C++
// Basic definition
using vtype = esl::vector< type, size >;

// Predefined vectors
using vector2i = vector< int32_t, 2 >;
using vector2u = vector< uint32_t, 2 >;
using vector2f = vector< float, 2 >;
using vector2d = vector< double, 2 >;

using vector3i = vector< int32_t, 3 >;
using vector3u = vector< uint32_t, 3 >;
using vector3f = vector< float, 3 >;
using vector3d = vector< double, 3 >;

using vector4i = vector< int32_t, 4 >;
using vector4u = vector< uint32_t, 4 >;
using vector4f = vector< float, 4 >;
using vector4d = vector< double, 4 >;

using vector5i = vector< int32_t, 5 >;
using vector5u = vector< uint32_t, 5 >;
using vector5f = vector< float, 5 >;
using vector5d = vector< double, 5 >;

using vector6i = vector< int32_t, 6 >;
using vector6u = vector< uint32_t, 6 >;
using vector6f = vector< float, 6 >;
using vector6d = vector< double, 6 >;

// Some operations
auto v1 = esl::vector3f(1, 2, 3);
auto v2 = esl::vector3f(0, 2, 0);
auto v3 = v1.cross(v2);
auto v4 = v1.dot(v2);
auto v5 = v1 + v2;
auto v6 = v1 - v2;
auto v7 = v1 / 2;
auto v8 = 2 * v1;
auto v9 = v1.square();
auto v10 = v1.sqrt();
auto v11 = (-v1).abs();
v1 += v2;

// Some functions
auto s1 = v1.norm_squared();
auto s2 = v1.norm();
auto s3 = v1.sum();

// Some access
auto a = v1.x();
auto b = v1[0];
```

## `quaternion.hpp`

An basic passive Hamilton quaternion, inherits from `vector< T, 4 >` so all vector operations works as well. Note that the internal storage is `[x, y, z, w]`.

### Example

```C++
// Basic definition
using vtype = esl::quaternion< type >;

// Predefined quaternions
using quaternionf = quaternion< float >;
using quaterniond = quaternion< double >;

// Added operations
auto q1 = esl::quaternionf(1, 2, 3, 4);
auto q2 = esl::quaternionf(0, 2, 0, 4);
auto q2 = esl::quaternionf(0, 2, 0, 4);

auto q3 = q1.conj();
auto q4 = q1 * q2;

// Rotations (all 3 are the same)
auto v2 = q1.rotate(v1);
auto v3 = q1 * v1;
auto v4 = v1 * q1;

// Extra access
auto a = v1.w();
auto v1 = q1.vec();
```
