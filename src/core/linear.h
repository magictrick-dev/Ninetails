#ifndef SRC_CORE_LINEAR_H
#define SRC_CORE_LINEAR_H
#include <core/definitions.h>
#include <math.h>
#include <xmmintrin.h>

// --- Vector2 -----------------------------------------------------------------

typedef union vector2
{
    r32 elements[2];

    struct
    {
        r32 x;
        r32 y;
    };

    struct
    {
        r32 u;
        r32 v;
    };

    struct
    {
        r32 width;
        r32 height;
    };

} vector2, v2;

v2 vector2_add(v2 lhs, v2 rhs);
v2 vector2_sub(v2 lhs, v2 rhs);
v2 vector2_mul(v2 vec, r32 scalar);
v2 vector2_div(v2 vec, r32 scalar);
v2 vector2_compwise_mul(v2 lhs, v2 rhs);
v2 vector2_compwise_div(v2 lhs, v2 rhs);
v2 vector2_normalize(v2 vec);
r32 vector2_magnitude_squared(v2 vec);
r32 vector2_magnitude(v2 vec);

// --- Vector3 -----------------------------------------------------------------

typedef union vector3
{
    r32 elements[3];

    struct
    {
        r32 x;
        r32 y;
        r32 z;
    };

    struct
    {
        r32 r;
        r32 g;
        r32 b;
    };

    struct
    {
        r32 u;
        r32 v;
        r32 w;
    };

    struct
    {
        v2 xy;
        r32 _unusued_z;
    };

    struct
    {
        r32 _unused_x;
        v2 yz;
    };

} vector3, v3;

v3 vector3_add(v3 lhs, v3 rhs);
v3 vector3_sub(v3 lhs, v3 rhs);
v3 vector3_mul(v3 vec, r32 scalar);
v3 vector3_div(v3 vec, r32 scalar);
v3 vector3_compwise_mul(v3 lhs, v3 rhs);
v3 vector3_compwise_div(v3 lhs, v3 rhs);
v3 vector3_normalize(v3 vec);
r32 vector3_magnitude_squared(v3 vec);
r32 vector3_magnitude(v3 vec);

// --- Vector4 -----------------------------------------------------------------

typedef union vector4
{
    r32 elements[4];
    __m128 sse;

    struct
    {
        r32 x;
        r32 y;
        r32 z;
        r32 w;
    };

    struct
    {
        r32 r;
        r32 g;
        r32 b;
        r32 a;
    };

    struct
    {
        v3 xyz;
        r32 _unusued_w1;
    };

    struct
    {
        v3 rgb;
        r32 _unusued_w2;
    };

} vector4, v4;

v4 vector4_add(v4 lhs, v4 rhs);
v4 vector4_sub(v4 lhs, v4 rhs);
v4 vector4_mul(v4 vec, r32 scalar);
v4 vector4_div(v4 vec, r32 scalar);
v4 vector4_compwise_mul(v4 lhs, v4 rhs);
v4 vector4_compwise_div(v4 lhs, v4 rhs);
v4 vector4_normalize(v4 vec);
v4 vector4_to_cartesian(v4 vec);
r32 vector4_magnitude_squared(v4 vec);
r32 vector4_magnitude(v4 vec);

void vector4_print(v4 vec);

// --- Matrix4 -----------------------------------------------------------------

typedef union matrix4
{

    r32 elements[4][4];
    v4 columns[4];

} matrix4, m4;

v4 matrix4_mulv4(m4 lhs, v4 rhs);
m4 matrix4_mulm4(m4 lhs, m4 rhs);

m4 matrix4_identity();
m4 matrix4_translate(v3 where);
m4 matrix4_scale(v3 how);

void matrix4_print(m4 mat);

#endif
