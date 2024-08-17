#include <core/linear.h>

// --- Vector2D Definitions ----------------------------------------------------

v2 
vector2_add(v2 lhs, v2 rhs)
{
    v2 result = {0};
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}

v2 
vector2_sub(v2 lhs, v2 rhs)
{
    v2 result = {0};
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

v2 
vector2_mul(v2 vec, r32 scalar)
{
    v2 result = {0};
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    return result;
}

v2 
vector2_div(v2 vec, r32 scalar)
{
    v2 result = {0};
    result.x = vec.x / scalar;
    result.y = vec.y / scalar;
    return result;
}

v2 
vector2_compwise_mul(v2 lhs, v2 rhs)
{
    v2 result = {0};
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    return result;
}

v2 
vector2_compwise_div(v2 lhs, v2 rhs)
{
    v2 result = {0};
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    return result;
}

v2 
vector2_normalize(v2 vec)
{
    v2 result = {0};
    r32 mag = vector2_magnitude(vec);
    result.x = vec.x / mag;
    result.y = vec.y / mag;
    return result;
}

r32 
vector2_magnitude_squared(v2 vec)
{
    r32 result;
    r32 xs = vec.x * vec.x;
    r32 ys = vec.y * vec.y;
    result = xs + ys;
    return result;
}

r32 
vector2_magnitude(v2 vec)
{
    r32 result = sqrtf(vector2_magnitude_squared(vec));
    return result;
}

// --- Vector3D Definitions ----------------------------------------------------

v3 
vector3_add(v3 lhs, v3 rhs)
{
    v3 result = {0};
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    return result;
}

v3 
vector3_sub(v3 lhs, v3 rhs)
{
    v3 result = {0};
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
}

v3 
vector3_mul(v3 vec, r32 scalar)
{
    v3 result = {0};
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    result.z = vec.z * scalar;
    return result;
}

v3 
vector3_div(v3 vec, r32 scalar)
{
    v3 result = {0};
    result.x = vec.x / scalar;
    result.y = vec.y / scalar;
    result.z = vec.z / scalar;
    return result;
}

v3 
vector3_compwise_mul(v3 lhs, v3 rhs)
{
    v3 result = {0};
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    result.z = lhs.z * rhs.z;
    return result;
}

v3 
vector3_compwise_div(v3 lhs, v3 rhs)
{
    v3 result = {0};
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    result.z = lhs.z / rhs.z;
    return result;
}

v3 
vector3_normalize(v3 vec)
{
    v3 result = {0};
    r32 mag = vector3_magnitude(vec);
    result.x = vec.x / mag;
    result.y = vec.y / mag;
    result.z = vec.z / mag;
    return result;
}

r32 
vector3_magnitude_squared(v3 vec)
{
    r32 result;
    r32 xs = vec.x * vec.x;
    r32 ys = vec.y * vec.y;
    r32 zs = vec.z * vec.z;
    result = xs + ys + zs;
    return result;
}

r32 
vector3_magnitude(v3 vec)
{
    r32 result = sqrtf(vector3_magnitude_squared(vec));
    return result;
}

// --- Vector4 Definition ------------------------------------------------------

v4 
vector4_add(v4 lhs, v4 rhs)
{
    v4 result = {0};
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    return result;
}

v4 
vector4_sub(v4 lhs, v4 rhs)
{
    v4 result = {0};
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
}

v4 
vector4_mul(v4 vec, r32 scalar)
{
    v4 result = {0};
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
    result.z = vec.z * scalar;
    return result;
}

v4 
vector4_div(v4 vec, r32 scalar)
{
    v4 result = {0};
    result.x = vec.x / scalar;
    result.y = vec.y / scalar;
    result.z = vec.z / scalar;
    return result;
}

v4 
vector4_compwise_mul(v4 lhs, v4 rhs)
{
    v4 result = {0};
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    result.z = lhs.z * rhs.z;
    return result;
}

v4 
vector4_compwise_div(v4 lhs, v4 rhs)
{
    v4 result = {0};
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    result.z = lhs.z / rhs.z;
    return result;
}

v4 
vector4_normalize(v4 vec)
{
    v4 result = {0};
    r32 mag = vector4_magnitude(vec);
    result.x = vec.x / mag;
    result.y = vec.y / mag;
    result.z = vec.z / mag;
    return result;
}

v4 
vector4_to_cartesian(v4 vec)
{
    v4 result = {0};
    result.x = vec.x / vec.w;
    result.y = vec.y / vec.w;
    result.z = vec.z / vec.w;
    result.w = vec.w / vec.w;
    return result;
}

r32 
vector4_magnitude_squared(v4 vec)
{
    r32 result = {0};
    r32 xs = vec.x * vec.x;
    r32 ys = vec.y * vec.y;
    r32 zs = vec.z * vec.z;
    result = xs + ys + zs;
    return result;
}

r32 
vector4_magnitude(v4 vec)
{
    r32 result = sqrtf(vector4_magnitude_squared(vec));
    return result;
}

void
vector4_print(v4 vec)
{
    printf("[ %f %f %f %f ]\n", vec.elements[0], vec.elements[1],
            vec.elements[2], vec.elements[3]);
}

// --- Matrix4 Definition ------------------------------------------------------

static inline v4
vector4_linear_combine(v4 left, m4 right)
{

    vector4 result = {0};

    result.sse = _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0x00), right.columns[0].sse);
    result.sse = _mm_add_ps(result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0x55), right.columns[1].sse));
    result.sse = _mm_add_ps(result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0xaa), right.columns[2].sse));
    result.sse = _mm_add_ps(result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0xff), right.columns[3].sse));

    return result;

}

v4 
matrix4_mulv4(m4 left, v4 right)
{

    vector4 result = vector4_linear_combine(right, left);
    return result;

}

m4 
matrix4_mulm4(m4 left, m4 right)
{

    matrix4 result = {0};
    result.columns[0] = vector4_linear_combine(right.columns[0], left);
    result.columns[1] = vector4_linear_combine(right.columns[1], left);
    result.columns[2] = vector4_linear_combine(right.columns[2], left);
    result.columns[3] = vector4_linear_combine(right.columns[3], left);
    return result;

}

m4 
matrix4_identity()
{

    matrix4 identity = {0};
    identity.columns[0].elements[0] = 1.0f;
    identity.columns[1].elements[1] = 1.0f;
    identity.columns[2].elements[2] = 1.0f;
    identity.columns[3].elements[3] = 1.0f;
    return identity;

}

m4
matrix4_translate(v3 where)
{

    matrix4 translation = matrix4_identity();
    translation.columns[3].elements[0] = where.x;
    translation.columns[3].elements[1] = where.y;
    translation.columns[3].elements[2] = where.z;
    return translation;

}

m4 
matrix4_scale(v3 how)
{

    matrix4 scale = matrix4_identity();
    scale.columns[0].elements[0] = how.x;
    scale.columns[1].elements[1] = how.y;
    scale.columns[2].elements[2] = how.z;
    return scale;

}

void
matrix4_print(m4 mat)
{
    vector4_print(mat.columns[0]);
    vector4_print(mat.columns[1]);
    vector4_print(mat.columns[2]);
    vector4_print(mat.columns[3]);
}
