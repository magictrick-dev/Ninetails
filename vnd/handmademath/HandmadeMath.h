/*
  HandmadeMath.h v2.0.0

  This is a single header file with a bunch of useful types and functions for
  games and graphics. Consider it a lightweight alternative to GLM that works
  both C and C++.

  =============================================================================
  CONFIG
  =============================================================================

  By default, all angles in Handmade Math are specified in radians. However, it
  can be configured to use degrees or turns instead. Use one of the following
  defines to specify the default unit for angles:

    #define HANDMADE_MATH_USE_RADIANS
    #define HANDMADE_MATH_USE_DEGREES
    #define HANDMADE_MATH_USE_TURNS

  Regardless of the default angle, you can use the following functions to
  specify an angle in a particular unit:

    anglerad(radians)
    angledeg(degrees)
    angleturn(turns)

  The definitions of these functions change depending on the default unit.

  -----------------------------------------------------------------------------

  Handmade Math ships with SSE (SIMD) implementations of several common
  operations. To disable the use of SSE intrinsics, you must define
  HANDMADE_MATH_NO_SSE before including this file:

    #define HANDMADE_MATH_NO_SSE
    #include "HandmadeMath.h"

  -----------------------------------------------------------------------------

  To use Handmade Math without the C runtime library, you must provide your own
  implementations of basic math functions. Otherwise, HandmadeMath.h will use
  the runtime library implementation of these functions.

  Define HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS and provide your own
  implementations of sinf, cosf, tanf, acosf, and sqrtf
  before including HandmadeMath.h, like so:

    #define HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS
    #define sinf MySinF
    #define cosf MyCosF
    #define tanf MyTanF
    #define acosf MyACosF
    #define sqrtf MySqrtF
    #include "HandmadeMath.h"

  By default, it is assumed that your math functions take radians. To use
  different units, you must define angle_user_to_internal and
  angle_internal_to_user. For example, if you want to use degrees in your
  code but your math functions use turns:

    #define angle_user_to_internal(a) ((a)*degtoturn)
    #define angle_internal_to_user(a) ((a)*turntodeg)

  =============================================================================

  LICENSE

  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.

  =============================================================================

  CREDITS

  Originally written by Zakary Strange.

  Functionality:
   Zakary Strange (strangezak@protonmail.com && @strangezak)
   Matt Mascarenhas (@miblo_)
   Aleph
   FieryDrake (@fierydrake)
   Gingerbill (@TheGingerBill)
   Ben Visness (@bvisness)
   Trinton Bullard (@Peliex_Dev)
   @AntonDan
   Logan Forman (@dev_dwarf)

  Fixes:
   Jeroen van Rijn (@J_vanRijn)
   Kiljacken (@Kiljacken)
   Insofaras (@insofaras)
   Daniel Gibson (@DanielGibson)
*/

#ifndef HANDMADE_MATH_H
#define HANDMADE_MATH_H

// Dummy macros for when test framework is not present.
#ifndef COVERAGE
# define COVERAGE(a, b)
#endif

#ifndef ASSERT_COVERED
# define ASSERT_COVERED(a)
#endif

#ifdef HANDMADE_MATH_NO_SSE
# warning "HANDMADE_MATH_NO_SSE is deprecated, use HANDMADE_MATH_NO_SIMD instead"
# define HANDMADE_MATH_NO_SIMD
#endif

/* let's figure out if SSE is really available (unless disabled anyway)
   (it isn't on non-x86/x86_64 platforms or even x86 without explicit SSE support)
   => only use "#ifdef HANDMADE_MATH__USE_SSE" to check for SSE support below this block! */
#ifndef HANDMADE_MATH_NO_SIMD
# ifdef _MSC_VER /* MSVC supports SSE in amd64 mode or _M_IX86_FP >= 1 (2 means SSE2) */
#  if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
#   define HANDMADE_MATH__USE_SSE 1
#  endif
# else /* not MSVC, probably GCC, clang, icc or something that doesn't support SSE anyway */
#  ifdef __SSE__ /* they #define __SSE__ if it's supported */
#   define HANDMADE_MATH__USE_SSE 1
#  endif /*  __SSE__ */
# endif /* not _MSC_VER */
# ifdef __ARM_NEON
#  define HANDMADE_MATH__USE_NEON 1
# endif /* NEON Supported */
#endif /* #ifndef HANDMADE_MATH_NO_SIMD */

#if (!defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
# define HANDMADE_MATH__USE_C11_GENERICS 1
#endif

#ifdef HANDMADE_MATH__USE_SSE
# include <xmmintrin.h>
#endif

#ifdef HANDMADE_MATH__USE_NEON
# include <arm_neon.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable:4201)
#endif

#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wfloat-equal"
# pragma GCC diagnostic ignored "-Wmissing-braces"
# ifdef __clang__
#  pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# endif
#endif

#if defined(__GNUC__) || defined(__clang__)
# define deprecated(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
# define deprecated(msg) __declspec(deprecated(msg))
#else
# define deprecated(msg)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HANDMADE_MATH_USE_DEGREES) \
    && !defined(HANDMADE_MATH_USE_TURNS) \
    && !defined(HANDMADE_MATH_USE_RADIANS)
# define HANDMADE_MATH_USE_RADIANS
#endif

#define pi 3.14159265358979323846
#define pi32 3.14159265359f
#define deg180 180.0
#define deg18032 180.0f
#define turnhalf 0.5
#define turnhalf32 0.5f
#define radtodeg ((float)(deg180/pi))
#define radtoturn ((float)(turnhalf/pi))
#define degtorad ((float)(pi/deg180))
#define degtoturn ((float)(turnhalf/deg180))
#define turntorad ((float)(pi/turnhalf))
#define turntodeg ((float)(deg180/turnhalf))

#if defined(HANDMADE_MATH_USE_RADIANS)
# define anglerad(a) (a)
# define angledeg(a) ((a)*degtorad)
# define angleturn(a) ((a)*turntorad)
#elif defined(HANDMADE_MATH_USE_DEGREES)
# define anglerad(a) ((a)*radtodeg)
# define angledeg(a) (a)
# define angleturn(a) ((a)*turntodeg)
#elif defined(HANDMADE_MATH_USE_TURNS)
# define anglerad(a) ((a)*radtoturn)
# define angledeg(a) ((a)*degtoturn)
# define angleturn(a) (a)
#endif

#if !defined(HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS)
# include <math.h>
# define sinf sinf
# define cosf cosf
# define tanf tanf
# define sqrtf sqrtf
# define acosf acosf
#endif

#if !defined(angle_user_to_internal)
# define angle_user_to_internal(a) (torad(a))
#endif

#if !defined(angle_internal_to_user)
# if defined(HANDMADE_MATH_USE_RADIANS)
#  define angle_internal_to_user(a) (a)
# elif defined(HANDMADE_MATH_USE_DEGREES)
#  define angle_internal_to_user(a) ((a)*radtodeg)
# elif defined(HANDMADE_MATH_USE_TURNS)
#  define angle_internal_to_user(a) ((a)*radtoturn)
# endif
#endif

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) < (b) ? (b) : (a))
#define abs(a) ((a) > 0 ? (a) : -(a))
#define mod(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define square(x) ((x) * (x))

typedef union vec2
{
    struct
    {
        float X, Y;
    };

    struct
    {
        float U, V;
    };

    struct
    {
        float Left, Right;
    };

    struct
    {
        float Width, Height;
    };

    float Elements[2];

#ifdef __cplusplus
    inline float &operator[](int Index) { return Elements[Index]; }
    inline const float& operator[](int Index) const { return Elements[Index]; }
#endif
} vec2;

typedef union vec3
{
    struct
    {
        float X, Y, Z;
    };

    struct
    {
        float U, V, W;
    };

    struct
    {
        float R, G, B;
    };

    struct
    {
        vec2 XY;
        float _Ignored0;
    };

    struct
    {
        float _Ignored1;
        vec2 YZ;
    };

    struct
    {
        vec2 UV;
        float _Ignored2;
    };

    struct
    {
        float _Ignored3;
        vec2 VW;
    };

    float Elements[3];

#ifdef __cplusplus
    inline float &operator[](int Index) { return Elements[Index]; }
    inline const float &operator[](int Index) const { return Elements[Index]; }
#endif
} vec3;

typedef union vec4
{
    struct
    {
        union
        {
            vec3 XYZ;
            struct
            {
                float X, Y, Z;
            };
        };

        float W;
    };
    struct
    {
        union
        {
            vec3 RGB;
            struct
            {
                float R, G, B;
            };
        };

        float A;
    };

    struct
    {
        vec2 XY;
        float _Ignored0;
        float _Ignored1;
    };

    struct
    {
        float _Ignored2;
        vec2 YZ;
        float _Ignored3;
    };

    struct
    {
        float _Ignored4;
        float _Ignored5;
        vec2 ZW;
    };

    float Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSE;
#endif

#ifdef HANDMADE_MATH__USE_NEON
    float32x4_t NEON;
#endif

#ifdef __cplusplus
    inline float &operator[](int Index) { return Elements[Index]; }
    inline const float &operator[](int Index) const { return Elements[Index]; }
#endif
} vec4;

typedef union mat2
{
    float Elements[2][2];
    vec2 Columns[2];

#ifdef __cplusplus
    inline vec2 &operator[](int Index) { return Columns[Index]; }
    inline const vec2 &operator[](int Index) const { return Columns[Index]; }
#endif
} mat2;

typedef union mat3
{
    float Elements[3][3];
    vec3 Columns[3];

#ifdef __cplusplus
    inline vec3 &operator[](int Index) { return Columns[Index]; }
    inline const vec3 &operator[](int Index) const { return Columns[Index]; }
#endif
} mat3;

typedef union mat4
{
    float Elements[4][4];
    vec4 Columns[4];

#ifdef __cplusplus
    inline vec4 &operator[](int Index) { return Columns[Index]; }
    inline const vec4 &operator[](int Index) const { return Columns[Index]; }
#endif
} mat4;

typedef union quat
{
    struct
    {
        union
        {
            vec3 XYZ;
            struct
            {
                float X, Y, Z;
            };
        };

        float W;
    };

    float Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSE;
#endif
#ifdef HANDMADE_MATH__USE_NEON
    float32x4_t NEON;
#endif
} quat;

//typedef signed int bool;

/*
 * Angle unit conversion functions
 */
static inline float torad(float Angle)
{
#if defined(HANDMADE_MATH_USE_RADIANS)
    float Result = Angle;
#elif defined(HANDMADE_MATH_USE_DEGREES)
    float Result = Angle * degtorad;
#elif defined(HANDMADE_MATH_USE_TURNS)
    float Result = Angle * turntorad;
#endif

    return Result;
}

static inline float todeg(float Angle)
{
#if defined(HANDMADE_MATH_USE_RADIANS)
    float Result = Angle * radtodeg;
#elif defined(HANDMADE_MATH_USE_DEGREES)
    float Result = Angle;
#elif defined(HANDMADE_MATH_USE_TURNS)
    float Result = Angle * turntodeg;
#endif

    return Result;
}

static inline float toturn(float Angle)
{
#if defined(HANDMADE_MATH_USE_RADIANS)
    float Result = Angle * radtoturn;
#elif defined(HANDMADE_MATH_USE_DEGREES)
    float Result = Angle * degtoturn;
#elif defined(HANDMADE_MATH_USE_TURNS)
    float Result = Angle;
#endif

    return Result;
}

/*
 * Floating-point math functions
 */

COVERAGE(sinf, 1)
static inline float sinf(float Angle)
{
    ASSERT_COVERED(sinf);
    return sinf(angle_user_to_internal(Angle));
}

COVERAGE(cosf, 1)
static inline float cosf(float Angle)
{
    ASSERT_COVERED(cosf);
    return cosf(angle_user_to_internal(Angle));
}

COVERAGE(tanf, 1)
static inline float tanf(float Angle)
{
    ASSERT_COVERED(tanf);
    return tanf(angle_user_to_internal(Angle));
}

COVERAGE(acosf, 1)
static inline float acosf(float Arg)
{
    ASSERT_COVERED(acosf);
    return angle_internal_to_user(acosf(Arg));
}

COVERAGE(sqrtf, 1)
static inline float sqrtf(float Float)
{
    ASSERT_COVERED(sqrtf);

    float Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 In = _mm_set_ss(Float);
    __m128 Out = _mm_sqrt_ss(In);
    Result = _mm_cvtss_f32(Out);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t In = vdupq_n_f32(Float);
    float32x4_t Out = vsqrtq_f32(In);
    Result = vgetq_lane_f32(Out, 0);
#else
    Result = sqrtf(Float);
#endif

    return Result;
}

COVERAGE(invsqrtf, 1)
static inline float invsqrtf(float Float)
{
    ASSERT_COVERED(invsqrtf);

    float Result;

    Result = 1.0f/sqrtf(Float);

    return Result;
}


/*
 * Utility functions
 */

COVERAGE(lerp, 1)
static inline float lerp(float A, float Time, float B)
{
    ASSERT_COVERED(lerp);
    return (1.0f - Time) * A + Time * B;
}

COVERAGE(clamp, 1)
static inline float clamp(float Min, float Value, float Max)
{
    ASSERT_COVERED(clamp);

    float Result = Value;

    if (Result < Min)
    {
        Result = Min;
    }

    if (Result > Max)
    {
        Result = Max;
    }

    return Result;
}


/*
 * Vector initialization
 */

COVERAGE(v2, 1)
static inline vec2 v2(float X, float Y)
{
    ASSERT_COVERED(v2);

    vec2 Result;
    Result.X = X;
    Result.Y = Y;

    return Result;
}

COVERAGE(v3, 1)
static inline vec3 v3(float X, float Y, float Z)
{
    ASSERT_COVERED(v3);

    vec3 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return Result;
}

COVERAGE(v4, 1)
static inline vec4 v4(float X, float Y, float Z, float W)
{
    ASSERT_COVERED(v4);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_setr_ps(X, Y, Z, W);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t v = {X, Y, Z, W};
    Result.NEON = v;
#else
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
#endif

    return Result;
}

COVERAGE(v4v, 1)
static inline vec4 v4v(vec3 Vector, float W)
{
    ASSERT_COVERED(v4v);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_setr_ps(Vector.X, Vector.Y, Vector.Z, W);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t v = {Vector.X, Vector.Y, Vector.Z, W};
    Result.NEON = v;
#else
    Result.XYZ = Vector;
    Result.W = W;
#endif

    return Result;
}


/*
 * Binary vector operations
 */

COVERAGE(addv2, 1)
static inline vec2 addv2(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(addv2);

    vec2 Result;
    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;

    return Result;
}

COVERAGE(addv3, 1)
static inline vec3 addv3(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(addv3);

    vec3 Result;
    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;

    return Result;
}

COVERAGE(addv4, 1)
static inline vec4 addv4(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(addv4);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_add_ps(Left.SSE, Right.SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vaddq_f32(Left.NEON, Right.NEON);
#else
    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;
    Result.W = Left.W + Right.W;
#endif

    return Result;
}

COVERAGE(subv2, 1)
static inline vec2 subv2(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(subv2);

    vec2 Result;
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;

    return Result;
}

COVERAGE(subv3, 1)
static inline vec3 subv3(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(subv3);

    vec3 Result;
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;

    return Result;
}

COVERAGE(subv4, 1)
static inline vec4 subv4(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(subv4);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_sub_ps(Left.SSE, Right.SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vsubq_f32(Left.NEON, Right.NEON);
#else
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;
    Result.W = Left.W - Right.W;
#endif

    return Result;
}

COVERAGE(mulv2, 1)
static inline vec2 mulv2(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(mulv2);

    vec2 Result;
    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;

    return Result;
}

COVERAGE(mulv2f, 1)
static inline vec2 mulv2f(vec2 Left, float Right)
{
    ASSERT_COVERED(mulv2f);

    vec2 Result;
    Result.X = Left.X * Right;
    Result.Y = Left.Y * Right;

    return Result;
}

COVERAGE(mulv3, 1)
static inline vec3 mulv3(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(mulv3);

    vec3 Result;
    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;

    return Result;
}

COVERAGE(mulv3f, 1)
static inline vec3 mulv3f(vec3 Left, float Right)
{
    ASSERT_COVERED(mulv3f);

    vec3 Result;
    Result.X = Left.X * Right;
    Result.Y = Left.Y * Right;
    Result.Z = Left.Z * Right;

    return Result;
}

COVERAGE(mulv4, 1)
static inline vec4 mulv4(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(mulv4);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_mul_ps(Left.SSE, Right.SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vmulq_f32(Left.NEON, Right.NEON);
#else
    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;
    Result.W = Left.W * Right.W;
#endif

    return Result;
}

COVERAGE(mulv4f, 1)
static inline vec4 mulv4f(vec4 Left, float Right)
{
    ASSERT_COVERED(mulv4f);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Right);
    Result.SSE = _mm_mul_ps(Left.SSE, Scalar);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vmulq_n_f32(Left.NEON, Right);
#else
    Result.X = Left.X * Right;
    Result.Y = Left.Y * Right;
    Result.Z = Left.Z * Right;
    Result.W = Left.W * Right;
#endif

    return Result;
}

COVERAGE(divv2, 1)
static inline vec2 divv2(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(divv2);

    vec2 Result;
    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;

    return Result;
}

COVERAGE(divv2f, 1)
static inline vec2 divv2f(vec2 Left, float Right)
{
    ASSERT_COVERED(divv2f);

    vec2 Result;
    Result.X = Left.X / Right;
    Result.Y = Left.Y / Right;

    return Result;
}

COVERAGE(divv3, 1)
static inline vec3 divv3(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(divv3);

    vec3 Result;
    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;

    return Result;
}

COVERAGE(divv3f, 1)
static inline vec3 divv3f(vec3 Left, float Right)
{
    ASSERT_COVERED(divv3f);

    vec3 Result;
    Result.X = Left.X / Right;
    Result.Y = Left.Y / Right;
    Result.Z = Left.Z / Right;

    return Result;
}

COVERAGE(divv4, 1)
static inline vec4 divv4(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(divv4);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_div_ps(Left.SSE, Right.SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vdivq_f32(Left.NEON, Right.NEON);
#else
    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;
    Result.W = Left.W / Right.W;
#endif

    return Result;
}

COVERAGE(divv4f, 1)
static inline vec4 divv4f(vec4 Left, float Right)
{
    ASSERT_COVERED(divv4f);

    vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Right);
    Result.SSE = _mm_div_ps(Left.SSE, Scalar);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t Scalar = vdupq_n_f32(Right);
    Result.NEON = vdivq_f32(Left.NEON, Scalar);
#else
    Result.X = Left.X / Right;
    Result.Y = Left.Y / Right;
    Result.Z = Left.Z / Right;
    Result.W = Left.W / Right;
#endif

    return Result;
}

COVERAGE(eqv2, 1)
static inline bool eqv2(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(eqv2);
    return Left.X == Right.X && Left.Y == Right.Y;
}

COVERAGE(eqv3, 1)
static inline bool eqv3(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(eqv3);
    return Left.X == Right.X && Left.Y == Right.Y && Left.Z == Right.Z;
}

COVERAGE(eqv4, 1)
static inline bool eqv4(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(eqv4);
    return Left.X == Right.X && Left.Y == Right.Y && Left.Z == Right.Z && Left.W == Right.W;
}

COVERAGE(dotv2, 1)
static inline float dotv2(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(dotv2);
    return (Left.X * Right.X) + (Left.Y * Right.Y);
}

COVERAGE(dotv3, 1)
static inline float dotv3(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(dotv3);
    return (Left.X * Right.X) + (Left.Y * Right.Y) + (Left.Z * Right.Z);
}

COVERAGE(dotv4, 1)
static inline float dotv4(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(dotv4);

    float Result;

    // NOTE(zak): IN the future if we wanna check what version SSE is support
    // we can use _mm_dp_ps (4.3) but for now we will use the old way.
    // Or a r = _mm_mul_ps(v1, v2), r = _mm_hadd_ps(r, r), r = _mm_hadd_ps(r, r) for SSE3
#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_mul_ps(Left.SSE, Right.SSE);
    __m128 SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    _mm_store_ss(&Result, SSEResultOne);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t NEONMultiplyResult = vmulq_f32(Left.NEON, Right.NEON);
    float32x4_t NEONHalfAdd = vpaddq_f32(NEONMultiplyResult, NEONMultiplyResult);
    float32x4_t NEONFullAdd = vpaddq_f32(NEONHalfAdd, NEONHalfAdd);
    Result = vgetq_lane_f32(NEONFullAdd, 0);
#else
    Result = ((Left.X * Right.X) + (Left.Z * Right.Z)) + ((Left.Y * Right.Y) + (Left.W * Right.W));
#endif

    return Result;
}

COVERAGE(cross, 1)
static inline vec3 cross(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(cross);

    vec3 Result;
    Result.X = (Left.Y * Right.Z) - (Left.Z * Right.Y);
    Result.Y = (Left.Z * Right.X) - (Left.X * Right.Z);
    Result.Z = (Left.X * Right.Y) - (Left.Y * Right.X);

    return Result;
}


/*
 * Unary vector operations
 */

COVERAGE(lensqrv2, 1)
static inline float lensqrv2(vec2 A)
{
    ASSERT_COVERED(lensqrv2);
    return dotv2(A, A);
}

COVERAGE(lensqrv3, 1)
static inline float lensqrv3(vec3 A)
{
    ASSERT_COVERED(lensqrv3);
    return dotv3(A, A);
}

COVERAGE(lensqrv4, 1)
static inline float lensqrv4(vec4 A)
{
    ASSERT_COVERED(lensqrv4);
    return dotv4(A, A);
}

COVERAGE(lenv2, 1)
static inline float lenv2(vec2 A)
{
    ASSERT_COVERED(lenv2);
    return sqrtf(lensqrv2(A));
}

COVERAGE(lenv3, 1)
static inline float lenv3(vec3 A)
{
    ASSERT_COVERED(lenv3);
    return sqrtf(lensqrv3(A));
}

COVERAGE(lenv4, 1)
static inline float lenv4(vec4 A)
{
    ASSERT_COVERED(lenv4);
    return sqrtf(lensqrv4(A));
}

COVERAGE(normv2, 1)
static inline vec2 normv2(vec2 A)
{
    ASSERT_COVERED(normv2);
    return mulv2f(A, invsqrtf(dotv2(A, A)));
}

COVERAGE(normv3, 1)
static inline vec3 normv3(vec3 A)
{
    ASSERT_COVERED(normv3);
    return mulv3f(A, invsqrtf(dotv3(A, A)));
}

COVERAGE(normv4, 1)
static inline vec4 normv4(vec4 A)
{
    ASSERT_COVERED(normv4);
    return mulv4f(A, invsqrtf(dotv4(A, A)));
}

/*
 * Utility vector functions
 */

COVERAGE(lerpv2, 1)
static inline vec2 lerpv2(vec2 A, float Time, vec2 B)
{
    ASSERT_COVERED(lerpv2);
    return addv2(mulv2f(A, 1.0f - Time), mulv2f(B, Time));
}

COVERAGE(lerpv3, 1)
static inline vec3 lerpv3(vec3 A, float Time, vec3 B)
{
    ASSERT_COVERED(lerpv3);
    return addv3(mulv3f(A, 1.0f - Time), mulv3f(B, Time));
}

COVERAGE(lerpv4, 1)
static inline vec4 lerpv4(vec4 A, float Time, vec4 B)
{
    ASSERT_COVERED(lerpv4);
    return addv4(mulv4f(A, 1.0f - Time), mulv4f(B, Time));
}

/*
 * SSE stuff
 */

COVERAGE(linearcombinev4m4, 1)
static inline vec4 linearcombinev4m4(vec4 Left, mat4 Right)
{
    ASSERT_COVERED(linearcombinev4m4);

    vec4 Result;
#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0x00), Right.Columns[0].SSE);
    Result.SSE = _mm_add_ps(Result.SSE, _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0x55), Right.Columns[1].SSE));
    Result.SSE = _mm_add_ps(Result.SSE, _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0xaa), Right.Columns[2].SSE));
    Result.SSE = _mm_add_ps(Result.SSE, _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0xff), Right.Columns[3].SSE));
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vmulq_laneq_f32(Right.Columns[0].NEON, Left.NEON, 0);
    Result.NEON = vfmaq_laneq_f32(Result.NEON, Right.Columns[1].NEON, Left.NEON, 1);
    Result.NEON = vfmaq_laneq_f32(Result.NEON, Right.Columns[2].NEON, Left.NEON, 2);
    Result.NEON = vfmaq_laneq_f32(Result.NEON, Right.Columns[3].NEON, Left.NEON, 3);
#else
    Result.X = Left.Elements[0] * Right.Columns[0].X;
    Result.Y = Left.Elements[0] * Right.Columns[0].Y;
    Result.Z = Left.Elements[0] * Right.Columns[0].Z;
    Result.W = Left.Elements[0] * Right.Columns[0].W;

    Result.X += Left.Elements[1] * Right.Columns[1].X;
    Result.Y += Left.Elements[1] * Right.Columns[1].Y;
    Result.Z += Left.Elements[1] * Right.Columns[1].Z;
    Result.W += Left.Elements[1] * Right.Columns[1].W;

    Result.X += Left.Elements[2] * Right.Columns[2].X;
    Result.Y += Left.Elements[2] * Right.Columns[2].Y;
    Result.Z += Left.Elements[2] * Right.Columns[2].Z;
    Result.W += Left.Elements[2] * Right.Columns[2].W;

    Result.X += Left.Elements[3] * Right.Columns[3].X;
    Result.Y += Left.Elements[3] * Right.Columns[3].Y;
    Result.Z += Left.Elements[3] * Right.Columns[3].Z;
    Result.W += Left.Elements[3] * Right.Columns[3].W;
#endif

    return Result;
}

/*
 * 2x2 Matrices
 */

COVERAGE(m2, 1)
static inline mat2 m2(void)
{
    ASSERT_COVERED(m2);
    mat2 Result = {0};
    return Result;
}

COVERAGE(m2d, 1)
static inline mat2 m2d(float Diagonal)
{
    ASSERT_COVERED(m2d);

    mat2 Result = {0};
    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;

    return Result;
}

COVERAGE(transposem2, 1)
static inline mat2 transposem2(mat2 Matrix)
{
    ASSERT_COVERED(transposem2);

    mat2 Result = Matrix;

    Result.Elements[0][1] = Matrix.Elements[1][0];
    Result.Elements[1][0] = Matrix.Elements[0][1];

    return Result;
}

COVERAGE(addm2, 1)
static inline mat2 addm2(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(addm2);

    mat2 Result;

    Result.Elements[0][0] = Left.Elements[0][0] + Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] + Right.Elements[0][1];
    Result.Elements[1][0] = Left.Elements[1][0] + Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] + Right.Elements[1][1];

    return Result;
}

COVERAGE(subm2, 1)
static inline mat2 subm2(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(subm2);

    mat2 Result;

    Result.Elements[0][0] = Left.Elements[0][0] - Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] - Right.Elements[0][1];
    Result.Elements[1][0] = Left.Elements[1][0] - Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] - Right.Elements[1][1];

    return Result;
}

COVERAGE(mulm2v2, 1)
static inline vec2 mulm2v2(mat2 Matrix, vec2 Vector)
{
    ASSERT_COVERED(mulm2v2);

    vec2 Result;

    Result.X = Vector.Elements[0] * Matrix.Columns[0].X;
    Result.Y = Vector.Elements[0] * Matrix.Columns[0].Y;

    Result.X += Vector.Elements[1] * Matrix.Columns[1].X;
    Result.Y += Vector.Elements[1] * Matrix.Columns[1].Y;

    return Result;
}

COVERAGE(mulm2, 1)
static inline mat2 mulm2(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(mulm2);

    mat2 Result;
    Result.Columns[0] = mulm2v2(Left, Right.Columns[0]);
    Result.Columns[1] = mulm2v2(Left, Right.Columns[1]);

    return Result;
}

COVERAGE(mulm2f, 1)
static inline mat2 mulm2f(mat2 Matrix, float Scalar)
{
    ASSERT_COVERED(mulm2f);

    mat2 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] * Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] * Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] * Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] * Scalar;

    return Result;
}

COVERAGE(divm2f, 1)
static inline mat2 divm2f(mat2 Matrix, float Scalar)
{
    ASSERT_COVERED(divm2f);

    mat2 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] / Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] / Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] / Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] / Scalar;

    return Result;
}

COVERAGE(determinantm2, 1)
static inline float determinantm2(mat2 Matrix)
{
    ASSERT_COVERED(determinantm2);
    return Matrix.Elements[0][0]*Matrix.Elements[1][1] - Matrix.Elements[0][1]*Matrix.Elements[1][0];
}


COVERAGE(invgeneralm2, 1)
static inline mat2 invgeneralm2(mat2 Matrix)
{
    ASSERT_COVERED(invgeneralm2);

    mat2 Result;
    float InvDeterminant = 1.0f / determinantm2(Matrix);
    Result.Elements[0][0] = InvDeterminant * +Matrix.Elements[1][1];
    Result.Elements[1][1] = InvDeterminant * +Matrix.Elements[0][0];
    Result.Elements[0][1] = InvDeterminant * -Matrix.Elements[0][1];
    Result.Elements[1][0] = InvDeterminant * -Matrix.Elements[1][0];

    return Result;
}

/*
 * 3x3 Matrices
 */

COVERAGE(m3, 1)
static inline mat3 m3(void)
{
    ASSERT_COVERED(m3);
    mat3 Result = {0};
    return Result;
}

COVERAGE(m3d, 1)
static inline mat3 m3d(float Diagonal)
{
    ASSERT_COVERED(m3d);

    mat3 Result = {0};
    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;
    Result.Elements[2][2] = Diagonal;

    return Result;
}

COVERAGE(transposem3, 1)
static inline mat3 transposem3(mat3 Matrix)
{
    ASSERT_COVERED(transposem3);

    mat3 Result = Matrix;

    Result.Elements[0][1] = Matrix.Elements[1][0];
    Result.Elements[0][2] = Matrix.Elements[2][0];
    Result.Elements[1][0] = Matrix.Elements[0][1];
    Result.Elements[1][2] = Matrix.Elements[2][1];
    Result.Elements[2][1] = Matrix.Elements[1][2];
    Result.Elements[2][0] = Matrix.Elements[0][2];

    return Result;
}

COVERAGE(addm3, 1)
static inline mat3 addm3(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(addm3);

    mat3 Result;

    Result.Elements[0][0] = Left.Elements[0][0] + Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] + Right.Elements[0][1];
    Result.Elements[0][2] = Left.Elements[0][2] + Right.Elements[0][2];
    Result.Elements[1][0] = Left.Elements[1][0] + Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] + Right.Elements[1][1];
    Result.Elements[1][2] = Left.Elements[1][2] + Right.Elements[1][2];
    Result.Elements[2][0] = Left.Elements[2][0] + Right.Elements[2][0];
    Result.Elements[2][1] = Left.Elements[2][1] + Right.Elements[2][1];
    Result.Elements[2][2] = Left.Elements[2][2] + Right.Elements[2][2];

    return Result;
}

COVERAGE(subm3, 1)
static inline mat3 subm3(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(subm3);

    mat3 Result;

    Result.Elements[0][0] = Left.Elements[0][0] - Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] - Right.Elements[0][1];
    Result.Elements[0][2] = Left.Elements[0][2] - Right.Elements[0][2];
    Result.Elements[1][0] = Left.Elements[1][0] - Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] - Right.Elements[1][1];
    Result.Elements[1][2] = Left.Elements[1][2] - Right.Elements[1][2];
    Result.Elements[2][0] = Left.Elements[2][0] - Right.Elements[2][0];
    Result.Elements[2][1] = Left.Elements[2][1] - Right.Elements[2][1];
    Result.Elements[2][2] = Left.Elements[2][2] - Right.Elements[2][2];

    return Result;
}

COVERAGE(mulm3v3, 1)
static inline vec3 mulm3v3(mat3 Matrix, vec3 Vector)
{
    ASSERT_COVERED(mulm3v3);

    vec3 Result;

    Result.X = Vector.Elements[0] * Matrix.Columns[0].X;
    Result.Y = Vector.Elements[0] * Matrix.Columns[0].Y;
    Result.Z = Vector.Elements[0] * Matrix.Columns[0].Z;

    Result.X += Vector.Elements[1] * Matrix.Columns[1].X;
    Result.Y += Vector.Elements[1] * Matrix.Columns[1].Y;
    Result.Z += Vector.Elements[1] * Matrix.Columns[1].Z;

    Result.X += Vector.Elements[2] * Matrix.Columns[2].X;
    Result.Y += Vector.Elements[2] * Matrix.Columns[2].Y;
    Result.Z += Vector.Elements[2] * Matrix.Columns[2].Z;

    return Result;
}

COVERAGE(mulm3, 1)
static inline mat3 mulm3(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(mulm3);

    mat3 Result;
    Result.Columns[0] = mulm3v3(Left, Right.Columns[0]);
    Result.Columns[1] = mulm3v3(Left, Right.Columns[1]);
    Result.Columns[2] = mulm3v3(Left, Right.Columns[2]);

    return Result;
}

COVERAGE(mulm3f, 1)
static inline mat3 mulm3f(mat3 Matrix, float Scalar)
{
    ASSERT_COVERED(mulm3f);

    mat3 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] * Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] * Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] * Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] * Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] * Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] * Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] * Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] * Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] * Scalar;

    return Result;
}

COVERAGE(divm3, 1)
static inline mat3 divm3f(mat3 Matrix, float Scalar)
{
    ASSERT_COVERED(divm3);

    mat3 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] / Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] / Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] / Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] / Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] / Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] / Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] / Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] / Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] / Scalar;

    return Result;
}

COVERAGE(determinantm3, 1)
static inline float determinantm3(mat3 Matrix)
{
    ASSERT_COVERED(determinantm3);

    mat3 Cross;
    Cross.Columns[0] = cross(Matrix.Columns[1], Matrix.Columns[2]);
    Cross.Columns[1] = cross(Matrix.Columns[2], Matrix.Columns[0]);
    Cross.Columns[2] = cross(Matrix.Columns[0], Matrix.Columns[1]);

    return dotv3(Cross.Columns[2], Matrix.Columns[2]);
}

COVERAGE(invgeneralm3, 1)
static inline mat3 invgeneralm3(mat3 Matrix)
{
    ASSERT_COVERED(invgeneralm3);

    mat3 Cross;
    Cross.Columns[0] = cross(Matrix.Columns[1], Matrix.Columns[2]);
    Cross.Columns[1] = cross(Matrix.Columns[2], Matrix.Columns[0]);
    Cross.Columns[2] = cross(Matrix.Columns[0], Matrix.Columns[1]);

    float InvDeterminant = 1.0f / dotv3(Cross.Columns[2], Matrix.Columns[2]);

    mat3 Result;
    Result.Columns[0] = mulv3f(Cross.Columns[0], InvDeterminant);
    Result.Columns[1] = mulv3f(Cross.Columns[1], InvDeterminant);
    Result.Columns[2] = mulv3f(Cross.Columns[2], InvDeterminant);

    return transposem3(Result);
}

/*
 * 4x4 Matrices
 */

COVERAGE(m4, 1)
static inline mat4 m4(void)
{
    ASSERT_COVERED(m4);
    mat4 Result = {0};
    return Result;
}

COVERAGE(m4d, 1)
static inline mat4 m4d(float Diagonal)
{
    ASSERT_COVERED(m4d);

    mat4 Result = {0};
    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;
    Result.Elements[2][2] = Diagonal;
    Result.Elements[3][3] = Diagonal;

    return Result;
}

COVERAGE(transposem4, 1)
static inline mat4 transposem4(mat4 Matrix)
{
    ASSERT_COVERED(transposem4);

    mat4 Result;
#ifdef HANDMADE_MATH__USE_SSE
    Result = Matrix;
    _MM_TRANSPOSE4_PS(Result.Columns[0].SSE, Result.Columns[1].SSE, Result.Columns[2].SSE, Result.Columns[3].SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4x4_t Transposed = vld4q_f32((float*)Matrix.Columns);
    Result.Columns[0].NEON = Transposed.val[0];
    Result.Columns[1].NEON = Transposed.val[1];
    Result.Columns[2].NEON = Transposed.val[2];
    Result.Columns[3].NEON = Transposed.val[3];
#else
    Result.Elements[0][0] = Matrix.Elements[0][0];
    Result.Elements[0][1] = Matrix.Elements[1][0];
    Result.Elements[0][2] = Matrix.Elements[2][0];
    Result.Elements[0][3] = Matrix.Elements[3][0];
    Result.Elements[1][0] = Matrix.Elements[0][1];
    Result.Elements[1][1] = Matrix.Elements[1][1];
    Result.Elements[1][2] = Matrix.Elements[2][1];
    Result.Elements[1][3] = Matrix.Elements[3][1];
    Result.Elements[2][0] = Matrix.Elements[0][2];
    Result.Elements[2][1] = Matrix.Elements[1][2];
    Result.Elements[2][2] = Matrix.Elements[2][2];
    Result.Elements[2][3] = Matrix.Elements[3][2];
    Result.Elements[3][0] = Matrix.Elements[0][3];
    Result.Elements[3][1] = Matrix.Elements[1][3];
    Result.Elements[3][2] = Matrix.Elements[2][3];
    Result.Elements[3][3] = Matrix.Elements[3][3];
#endif

    return Result;
}

COVERAGE(addm4, 1)
static inline mat4 addm4(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(addm4);

    mat4 Result;

    Result.Columns[0] = addv4(Left.Columns[0], Right.Columns[0]);
    Result.Columns[1] = addv4(Left.Columns[1], Right.Columns[1]);
    Result.Columns[2] = addv4(Left.Columns[2], Right.Columns[2]);
    Result.Columns[3] = addv4(Left.Columns[3], Right.Columns[3]);

    return Result;
}

COVERAGE(subm4, 1)
static inline mat4 subm4(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(subm4);

    mat4 Result;

    Result.Columns[0] = subv4(Left.Columns[0], Right.Columns[0]);
    Result.Columns[1] = subv4(Left.Columns[1], Right.Columns[1]);
    Result.Columns[2] = subv4(Left.Columns[2], Right.Columns[2]);
    Result.Columns[3] = subv4(Left.Columns[3], Right.Columns[3]);

    return Result;
}

COVERAGE(mulm4, 1)
static inline mat4 mulm4(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(mulm4);

    mat4 Result;
    Result.Columns[0] = linearcombinev4m4(Right.Columns[0], Left);
    Result.Columns[1] = linearcombinev4m4(Right.Columns[1], Left);
    Result.Columns[2] = linearcombinev4m4(Right.Columns[2], Left);
    Result.Columns[3] = linearcombinev4m4(Right.Columns[3], Left);

    return Result;
}

COVERAGE(mulm4f, 1)
static inline mat4 mulm4f(mat4 Matrix, float Scalar)
{
    ASSERT_COVERED(mulm4f);

    mat4 Result;


#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEScalar = _mm_set1_ps(Scalar);
    Result.Columns[0].SSE = _mm_mul_ps(Matrix.Columns[0].SSE, SSEScalar);
    Result.Columns[1].SSE = _mm_mul_ps(Matrix.Columns[1].SSE, SSEScalar);
    Result.Columns[2].SSE = _mm_mul_ps(Matrix.Columns[2].SSE, SSEScalar);
    Result.Columns[3].SSE = _mm_mul_ps(Matrix.Columns[3].SSE, SSEScalar);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.Columns[0].NEON = vmulq_n_f32(Matrix.Columns[0].NEON, Scalar);
    Result.Columns[1].NEON = vmulq_n_f32(Matrix.Columns[1].NEON, Scalar);
    Result.Columns[2].NEON = vmulq_n_f32(Matrix.Columns[2].NEON, Scalar);
    Result.Columns[3].NEON = vmulq_n_f32(Matrix.Columns[3].NEON, Scalar);
#else
    Result.Elements[0][0] = Matrix.Elements[0][0] * Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] * Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] * Scalar;
    Result.Elements[0][3] = Matrix.Elements[0][3] * Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] * Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] * Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] * Scalar;
    Result.Elements[1][3] = Matrix.Elements[1][3] * Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] * Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] * Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] * Scalar;
    Result.Elements[2][3] = Matrix.Elements[2][3] * Scalar;
    Result.Elements[3][0] = Matrix.Elements[3][0] * Scalar;
    Result.Elements[3][1] = Matrix.Elements[3][1] * Scalar;
    Result.Elements[3][2] = Matrix.Elements[3][2] * Scalar;
    Result.Elements[3][3] = Matrix.Elements[3][3] * Scalar;
#endif

    return Result;
}

COVERAGE(mulm4v4, 1)
static inline vec4 mulm4v4(mat4 Matrix, vec4 Vector)
{
    ASSERT_COVERED(mulm4v4);
    return linearcombinev4m4(Vector, Matrix);
}

COVERAGE(divm4f, 1)
static inline mat4 divm4f(mat4 Matrix, float Scalar)
{
    ASSERT_COVERED(divm4f);

    mat4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEScalar = _mm_set1_ps(Scalar);
    Result.Columns[0].SSE = _mm_div_ps(Matrix.Columns[0].SSE, SSEScalar);
    Result.Columns[1].SSE = _mm_div_ps(Matrix.Columns[1].SSE, SSEScalar);
    Result.Columns[2].SSE = _mm_div_ps(Matrix.Columns[2].SSE, SSEScalar);
    Result.Columns[3].SSE = _mm_div_ps(Matrix.Columns[3].SSE, SSEScalar);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t NEONScalar = vdupq_n_f32(Scalar);
    Result.Columns[0].NEON = vdivq_f32(Matrix.Columns[0].NEON, NEONScalar);
    Result.Columns[1].NEON = vdivq_f32(Matrix.Columns[1].NEON, NEONScalar);
    Result.Columns[2].NEON = vdivq_f32(Matrix.Columns[2].NEON, NEONScalar);
    Result.Columns[3].NEON = vdivq_f32(Matrix.Columns[3].NEON, NEONScalar);
#else
    Result.Elements[0][0] = Matrix.Elements[0][0] / Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] / Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] / Scalar;
    Result.Elements[0][3] = Matrix.Elements[0][3] / Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] / Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] / Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] / Scalar;
    Result.Elements[1][3] = Matrix.Elements[1][3] / Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] / Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] / Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] / Scalar;
    Result.Elements[2][3] = Matrix.Elements[2][3] / Scalar;
    Result.Elements[3][0] = Matrix.Elements[3][0] / Scalar;
    Result.Elements[3][1] = Matrix.Elements[3][1] / Scalar;
    Result.Elements[3][2] = Matrix.Elements[3][2] / Scalar;
    Result.Elements[3][3] = Matrix.Elements[3][3] / Scalar;
#endif

    return Result;
}

COVERAGE(determinantm4, 1)
static inline float determinantm4(mat4 Matrix)
{
    ASSERT_COVERED(determinantm4);

    vec3 C01 = cross(Matrix.Columns[0].XYZ, Matrix.Columns[1].XYZ);
    vec3 C23 = cross(Matrix.Columns[2].XYZ, Matrix.Columns[3].XYZ);
    vec3 B10 = subv3(mulv3f(Matrix.Columns[0].XYZ, Matrix.Columns[1].W), mulv3f(Matrix.Columns[1].XYZ, Matrix.Columns[0].W));
    vec3 B32 = subv3(mulv3f(Matrix.Columns[2].XYZ, Matrix.Columns[3].W), mulv3f(Matrix.Columns[3].XYZ, Matrix.Columns[2].W));

    return dotv3(C01, B32) + dotv3(C23, B10);
}

COVERAGE(invgeneralm4, 1)
// Returns a general-purpose inverse of an mat4. Note that special-purpose inverses of many transformations
// are available and will be more efficient.
static inline mat4 invgeneralm4(mat4 Matrix)
{
    ASSERT_COVERED(invgeneralm4);

    vec3 C01 = cross(Matrix.Columns[0].XYZ, Matrix.Columns[1].XYZ);
    vec3 C23 = cross(Matrix.Columns[2].XYZ, Matrix.Columns[3].XYZ);
    vec3 B10 = subv3(mulv3f(Matrix.Columns[0].XYZ, Matrix.Columns[1].W), mulv3f(Matrix.Columns[1].XYZ, Matrix.Columns[0].W));
    vec3 B32 = subv3(mulv3f(Matrix.Columns[2].XYZ, Matrix.Columns[3].W), mulv3f(Matrix.Columns[3].XYZ, Matrix.Columns[2].W));

    float InvDeterminant = 1.0f / (dotv3(C01, B32) + dotv3(C23, B10));
    C01 = mulv3f(C01, InvDeterminant);
    C23 = mulv3f(C23, InvDeterminant);
    B10 = mulv3f(B10, InvDeterminant);
    B32 = mulv3f(B32, InvDeterminant);

    mat4 Result;
    Result.Columns[0] = v4v(addv3(cross(Matrix.Columns[1].XYZ, B32), mulv3f(C23, Matrix.Columns[1].W)), -dotv3(Matrix.Columns[1].XYZ, C23));
    Result.Columns[1] = v4v(subv3(cross(B32, Matrix.Columns[0].XYZ), mulv3f(C23, Matrix.Columns[0].W)), +dotv3(Matrix.Columns[0].XYZ, C23));
    Result.Columns[2] = v4v(addv3(cross(Matrix.Columns[3].XYZ, B10), mulv3f(C01, Matrix.Columns[3].W)), -dotv3(Matrix.Columns[3].XYZ, C01));
    Result.Columns[3] = v4v(subv3(cross(B10, Matrix.Columns[2].XYZ), mulv3f(C01, Matrix.Columns[2].W)), +dotv3(Matrix.Columns[2].XYZ, C01));

    return transposem4(Result);
}

/*
 * Common graphics transformations
 */

COVERAGE(orthographic_rh_no, 1)
// Produces a right-handed orthographic projection matrix with Z ranging from -1 to 1 (the GL convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline mat4 orthographic_rh_no(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    ASSERT_COVERED(orthographic_rh_no);

    mat4 Result = {0};

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 2.0f / (Near - Far);
    Result.Elements[3][3] = 1.0f;

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Near + Far) / (Near - Far);

    return Result;
}

COVERAGE(orthographic_rh_zo, 1)
// Produces a right-handed orthographic projection matrix with Z ranging from 0 to 1 (the DirectX convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline mat4 orthographic_rh_zo(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    ASSERT_COVERED(orthographic_rh_zo);

    mat4 Result = {0};

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 1.0f / (Near - Far);
    Result.Elements[3][3] = 1.0f;

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Near) / (Near - Far);

    return Result;
}

COVERAGE(orthographic_lh_no, 1)
// Produces a left-handed orthographic projection matrix with Z ranging from -1 to 1 (the GL convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline mat4 orthographic_lh_no(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    ASSERT_COVERED(orthographic_lh_no);

    mat4 Result = orthographic_rh_no(Left, Right, Bottom, Top, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];

    return Result;
}

COVERAGE(orthographic_lh_zo, 1)
// Produces a left-handed orthographic projection matrix with Z ranging from 0 to 1 (the DirectX convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline mat4 orthographic_lh_zo(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    ASSERT_COVERED(orthographic_lh_zo);

    mat4 Result = orthographic_rh_zo(Left, Right, Bottom, Top, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];

    return Result;
}

COVERAGE(invorthographic, 1)
// Returns an inverse for the given orthographic projection matrix. Works for all orthographic
// projection matrices, regardless of handedness or NDC convention.
static inline mat4 invorthographic(mat4 OrthoMatrix)
{
    ASSERT_COVERED(invorthographic);

    mat4 Result = {0};
    Result.Elements[0][0] = 1.0f / OrthoMatrix.Elements[0][0];
    Result.Elements[1][1] = 1.0f / OrthoMatrix.Elements[1][1];
    Result.Elements[2][2] = 1.0f / OrthoMatrix.Elements[2][2];
    Result.Elements[3][3] = 1.0f;

    Result.Elements[3][0] = -OrthoMatrix.Elements[3][0] * Result.Elements[0][0];
    Result.Elements[3][1] = -OrthoMatrix.Elements[3][1] * Result.Elements[1][1];
    Result.Elements[3][2] = -OrthoMatrix.Elements[3][2] * Result.Elements[2][2];

    return Result;
}

COVERAGE(perspective_rh_no, 1)
static inline mat4 perspective_rh_no(float FOV, float AspectRatio, float Near, float Far)
{
    ASSERT_COVERED(perspective_rh_no);

    mat4 Result = {0};

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    float Cotangent = 1.0f / tanf(FOV / 2.0f);
    Result.Elements[0][0] = Cotangent / AspectRatio;
    Result.Elements[1][1] = Cotangent;
    Result.Elements[2][3] = -1.0f;

    Result.Elements[2][2] = (Near + Far) / (Near - Far);
    Result.Elements[3][2] = (2.0f * Near * Far) / (Near - Far);

    return Result;
}

COVERAGE(perspective_rh_zo, 1)
static inline mat4 perspective_rh_zo(float FOV, float AspectRatio, float Near, float Far)
{
    ASSERT_COVERED(perspective_rh_zo);

    mat4 Result = {0};

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    float Cotangent = 1.0f / tanf(FOV / 2.0f);
    Result.Elements[0][0] = Cotangent / AspectRatio;
    Result.Elements[1][1] = Cotangent;
    Result.Elements[2][3] = -1.0f;

    Result.Elements[2][2] = (Far) / (Near - Far);
    Result.Elements[3][2] = (Near * Far) / (Near - Far);

    return Result;
}

COVERAGE(perspective_lh_no, 1)
static inline mat4 perspective_lh_no(float FOV, float AspectRatio, float Near, float Far)
{
    ASSERT_COVERED(perspective_lh_no);

    mat4 Result = perspective_rh_no(FOV, AspectRatio, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];
    Result.Elements[2][3] = -Result.Elements[2][3];

    return Result;
}

COVERAGE(perspective_lh_zo, 1)
static inline mat4 perspective_lh_zo(float FOV, float AspectRatio, float Near, float Far)
{
    ASSERT_COVERED(perspective_lh_zo);

    mat4 Result = perspective_rh_zo(FOV, AspectRatio, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];
    Result.Elements[2][3] = -Result.Elements[2][3];

    return Result;
}

COVERAGE(invperspective_rh, 1)
static inline mat4 invperspective_rh(mat4 PerspectiveMatrix)
{
    ASSERT_COVERED(invperspective_rh);

    mat4 Result = {0};
    Result.Elements[0][0] = 1.0f / PerspectiveMatrix.Elements[0][0];
    Result.Elements[1][1] = 1.0f / PerspectiveMatrix.Elements[1][1];
    Result.Elements[2][2] = 0.0f;

    Result.Elements[2][3] = 1.0f / PerspectiveMatrix.Elements[3][2];
    Result.Elements[3][3] = PerspectiveMatrix.Elements[2][2] * Result.Elements[2][3];
    Result.Elements[3][2] = PerspectiveMatrix.Elements[2][3];

    return Result;
}

COVERAGE(invperspective_lh, 1)
static inline mat4 invperspective_lh(mat4 PerspectiveMatrix)
{
    ASSERT_COVERED(invperspective_lh);

    mat4 Result = {0};
    Result.Elements[0][0] = 1.0f / PerspectiveMatrix.Elements[0][0];
    Result.Elements[1][1] = 1.0f / PerspectiveMatrix.Elements[1][1];
    Result.Elements[2][2] = 0.0f;

    Result.Elements[2][3] = 1.0f / PerspectiveMatrix.Elements[3][2];
    Result.Elements[3][3] = PerspectiveMatrix.Elements[2][2] * -Result.Elements[2][3];
    Result.Elements[3][2] = PerspectiveMatrix.Elements[2][3];

    return Result;
}

COVERAGE(translate, 1)
static inline mat4 translate(vec3 Translation)
{
    ASSERT_COVERED(translate);

    mat4 Result = m4d(1.0f);
    Result.Elements[3][0] = Translation.X;
    Result.Elements[3][1] = Translation.Y;
    Result.Elements[3][2] = Translation.Z;

    return Result;
}

COVERAGE(invtranslate, 1)
static inline mat4 invtranslate(mat4 TranslationMatrix)
{
    ASSERT_COVERED(invtranslate);

    mat4 Result = TranslationMatrix;
    Result.Elements[3][0] = -Result.Elements[3][0];
    Result.Elements[3][1] = -Result.Elements[3][1];
    Result.Elements[3][2] = -Result.Elements[3][2];

    return Result;
}

COVERAGE(rotate_rh, 1)
static inline mat4 rotate_rh(float Angle, vec3 Axis)
{
    ASSERT_COVERED(rotate_rh);

    mat4 Result = m4d(1.0f);

    Axis = normv3(Axis);

    float SinTheta = sinf(Angle);
    float CosTheta = cosf(Angle);
    float CosValue = 1.0f - CosTheta;

    Result.Elements[0][0] = (Axis.X * Axis.X * CosValue) + CosTheta;
    Result.Elements[0][1] = (Axis.X * Axis.Y * CosValue) + (Axis.Z * SinTheta);
    Result.Elements[0][2] = (Axis.X * Axis.Z * CosValue) - (Axis.Y * SinTheta);

    Result.Elements[1][0] = (Axis.Y * Axis.X * CosValue) - (Axis.Z * SinTheta);
    Result.Elements[1][1] = (Axis.Y * Axis.Y * CosValue) + CosTheta;
    Result.Elements[1][2] = (Axis.Y * Axis.Z * CosValue) + (Axis.X * SinTheta);

    Result.Elements[2][0] = (Axis.Z * Axis.X * CosValue) + (Axis.Y * SinTheta);
    Result.Elements[2][1] = (Axis.Z * Axis.Y * CosValue) - (Axis.X * SinTheta);
    Result.Elements[2][2] = (Axis.Z * Axis.Z * CosValue) + CosTheta;

    return Result;
}

COVERAGE(rotate_lh, 1)
static inline mat4 rotate_lh(float Angle, vec3 Axis)
{
    ASSERT_COVERED(rotate_lh);
    /* NOTE(lcf): Matrix will be inverse/transpose of RH. */
    return rotate_rh(-Angle, Axis);
}

COVERAGE(invrotate, 1)
static inline mat4 invrotate(mat4 RotationMatrix)
{
    ASSERT_COVERED(invrotate);
    return transposem4(RotationMatrix);
}

COVERAGE(scale, 1)
static inline mat4 scale(vec3 Scale)
{
    ASSERT_COVERED(scale);

    mat4 Result = m4d(1.0f);
    Result.Elements[0][0] = Scale.X;
    Result.Elements[1][1] = Scale.Y;
    Result.Elements[2][2] = Scale.Z;

    return Result;
}

COVERAGE(invscale, 1)
static inline mat4 invscale(mat4 ScaleMatrix)
{
    ASSERT_COVERED(invscale);

    mat4 Result = ScaleMatrix;
    Result.Elements[0][0] = 1.0f / Result.Elements[0][0];
    Result.Elements[1][1] = 1.0f / Result.Elements[1][1];
    Result.Elements[2][2] = 1.0f / Result.Elements[2][2];

    return Result;
}

static inline mat4 _lookat(vec3 F,  vec3 S, vec3 U,  vec3 Eye)
{
    mat4 Result;

    Result.Elements[0][0] = S.X;
    Result.Elements[0][1] = U.X;
    Result.Elements[0][2] = -F.X;
    Result.Elements[0][3] = 0.0f;

    Result.Elements[1][0] = S.Y;
    Result.Elements[1][1] = U.Y;
    Result.Elements[1][2] = -F.Y;
    Result.Elements[1][3] = 0.0f;

    Result.Elements[2][0] = S.Z;
    Result.Elements[2][1] = U.Z;
    Result.Elements[2][2] = -F.Z;
    Result.Elements[2][3] = 0.0f;

    Result.Elements[3][0] = -dotv3(S, Eye);
    Result.Elements[3][1] = -dotv3(U, Eye);
    Result.Elements[3][2] = dotv3(F, Eye);
    Result.Elements[3][3] = 1.0f;

    return Result;
}

COVERAGE(lookat_rh, 1)
static inline mat4 lookat_rh(vec3 Eye, vec3 Center, vec3 Up)
{
    ASSERT_COVERED(lookat_rh);

    vec3 F = normv3(subv3(Center, Eye));
    vec3 S = normv3(cross(F, Up));
    vec3 U = cross(S, F);

    return _lookat(F, S, U, Eye);
}

COVERAGE(lookat_lh, 1)
static inline mat4 lookat_lh(vec3 Eye, vec3 Center, vec3 Up)
{
    ASSERT_COVERED(lookat_lh);

    vec3 F = normv3(subv3(Eye, Center));
    vec3 S = normv3(cross(F, Up));
    vec3 U = cross(S, F);

    return _lookat(F, S, U, Eye);
}

COVERAGE(invlookat, 1)
static inline mat4 invlookat(mat4 Matrix)
{
    ASSERT_COVERED(invlookat);
    mat4 Result;

    mat3 Rotation = {0};
    Rotation.Columns[0] = Matrix.Columns[0].XYZ;
    Rotation.Columns[1] = Matrix.Columns[1].XYZ;
    Rotation.Columns[2] = Matrix.Columns[2].XYZ;
    Rotation = transposem3(Rotation);

    Result.Columns[0] = v4v(Rotation.Columns[0], 0.0f);
    Result.Columns[1] = v4v(Rotation.Columns[1], 0.0f);
    Result.Columns[2] = v4v(Rotation.Columns[2], 0.0f);
    Result.Columns[3] = mulv4f(Matrix.Columns[3], -1.0f);
    Result.Elements[3][0] = -1.0f * Matrix.Elements[3][0] /
        (Rotation.Elements[0][0] + Rotation.Elements[0][1] + Rotation.Elements[0][2]);
    Result.Elements[3][1] = -1.0f * Matrix.Elements[3][1] /
        (Rotation.Elements[1][0] + Rotation.Elements[1][1] + Rotation.Elements[1][2]);
    Result.Elements[3][2] = -1.0f * Matrix.Elements[3][2] /
        (Rotation.Elements[2][0] + Rotation.Elements[2][1] + Rotation.Elements[2][2]);
    Result.Elements[3][3] = 1.0f;

    return Result;
}

/*
 * Quaternion operations
 */

COVERAGE(q, 1)
static inline quat q(float X, float Y, float Z, float W)
{
    ASSERT_COVERED(q);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_setr_ps(X, Y, Z, W);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t v = { X, Y, Z, W };
    Result.NEON = v;
#else
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
#endif

    return Result;
}

COVERAGE(qv4, 1)
static inline quat qv4(vec4 Vector)
{
    ASSERT_COVERED(qv4);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = Vector.SSE;
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = Vector.NEON;
#else
    Result.X = Vector.X;
    Result.Y = Vector.Y;
    Result.Z = Vector.Z;
    Result.W = Vector.W;
#endif

    return Result;
}

COVERAGE(addq, 1)
static inline quat addq(quat Left, quat Right)
{
    ASSERT_COVERED(addq);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_add_ps(Left.SSE, Right.SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vaddq_f32(Left.NEON, Right.NEON);
#else

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;
    Result.W = Left.W + Right.W;
#endif

    return Result;
}

COVERAGE(subq, 1)
static inline quat subq(quat Left, quat Right)
{
    ASSERT_COVERED(subq);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_sub_ps(Left.SSE, Right.SSE);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vsubq_f32(Left.NEON, Right.NEON);
#else
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;
    Result.W = Left.W - Right.W;
#endif

    return Result;
}

COVERAGE(mulq, 1)
static inline quat mulq(quat Left, quat Right)
{
    ASSERT_COVERED(mulq);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(0, 0, 0, 0)), _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
    __m128 SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(0, 1, 2, 3));
    __m128 SSEResultThree = _mm_mul_ps(SSEResultTwo, SSEResultOne);

    SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(1, 1, 1, 1)) , _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
    SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(1, 0, 3, 2));
    SSEResultThree = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));

    SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(2, 2, 2, 2)), _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
    SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultThree = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));

    SSEResultOne = _mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(3, 3, 3, 3));
    SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(3, 2, 1, 0));
    Result.SSE = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t Right1032 = vrev64q_f32(Right.NEON);
    float32x4_t Right3210 = vcombine_f32(vget_high_f32(Right1032), vget_low_f32(Right1032));
    float32x4_t Right2301 = vrev64q_f32(Right3210);

    float32x4_t FirstSign = {1.0f, -1.0f, 1.0f, -1.0f};
    Result.NEON = vmulq_f32(Right3210, vmulq_f32(vdupq_laneq_f32(Left.NEON, 0), FirstSign));
    float32x4_t SecondSign = {1.0f, 1.0f, -1.0f, -1.0f};
    Result.NEON = vfmaq_f32(Result.NEON, Right2301, vmulq_f32(vdupq_laneq_f32(Left.NEON, 1), SecondSign));
    float32x4_t ThirdSign = {-1.0f, 1.0f, 1.0f, -1.0f};
    Result.NEON = vfmaq_f32(Result.NEON, Right1032, vmulq_f32(vdupq_laneq_f32(Left.NEON, 2), ThirdSign));
    Result.NEON = vfmaq_laneq_f32(Result.NEON, Right.NEON, Left.NEON, 3);

#else
    Result.X =  Right.Elements[3] * +Left.Elements[0];
    Result.Y =  Right.Elements[2] * -Left.Elements[0];
    Result.Z =  Right.Elements[1] * +Left.Elements[0];
    Result.W =  Right.Elements[0] * -Left.Elements[0];

    Result.X += Right.Elements[2] * +Left.Elements[1];
    Result.Y += Right.Elements[3] * +Left.Elements[1];
    Result.Z += Right.Elements[0] * -Left.Elements[1];
    Result.W += Right.Elements[1] * -Left.Elements[1];

    Result.X += Right.Elements[1] * -Left.Elements[2];
    Result.Y += Right.Elements[0] * +Left.Elements[2];
    Result.Z += Right.Elements[3] * +Left.Elements[2];
    Result.W += Right.Elements[2] * -Left.Elements[2];

    Result.X += Right.Elements[0] * +Left.Elements[3];
    Result.Y += Right.Elements[1] * +Left.Elements[3];
    Result.Z += Right.Elements[2] * +Left.Elements[3];
    Result.W += Right.Elements[3] * +Left.Elements[3];
#endif

    return Result;
}

COVERAGE(mulqf, 1)
static inline quat mulqf(quat Left, float Multiplicative)
{
    ASSERT_COVERED(mulqf);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Multiplicative);
    Result.SSE = _mm_mul_ps(Left.SSE, Scalar);
#elif defined(HANDMADE_MATH__USE_NEON)
    Result.NEON = vmulq_n_f32(Left.NEON, Multiplicative);
#else
    Result.X = Left.X * Multiplicative;
    Result.Y = Left.Y * Multiplicative;
    Result.Z = Left.Z * Multiplicative;
    Result.W = Left.W * Multiplicative;
#endif

    return Result;
}

COVERAGE(divqf, 1)
static inline quat divqf(quat Left, float Divnd)
{
    ASSERT_COVERED(divqf);

    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Divnd);
    Result.SSE = _mm_div_ps(Left.SSE, Scalar);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t Scalar = vdupq_n_f32(Divnd);
    Result.NEON = vdivq_f32(Left.NEON, Scalar);
#else
    Result.X = Left.X / Divnd;
    Result.Y = Left.Y / Divnd;
    Result.Z = Left.Z / Divnd;
    Result.W = Left.W / Divnd;
#endif

    return Result;
}

COVERAGE(dotq, 1)
static inline float dotq(quat Left, quat Right)
{
    ASSERT_COVERED(dotq);

    float Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_mul_ps(Left.SSE, Right.SSE);
    __m128 SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    _mm_store_ss(&Result, SSEResultOne);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t NEONMultiplyResult = vmulq_f32(Left.NEON, Right.NEON);
    float32x4_t NEONHalfAdd = vpaddq_f32(NEONMultiplyResult, NEONMultiplyResult);
    float32x4_t NEONFullAdd = vpaddq_f32(NEONHalfAdd, NEONHalfAdd);
    Result = vgetq_lane_f32(NEONFullAdd, 0);
#else
    Result = ((Left.X * Right.X) + (Left.Z * Right.Z)) + ((Left.Y * Right.Y) + (Left.W * Right.W));
#endif

    return Result;
}

COVERAGE(invq, 1)
static inline quat invq(quat Left)
{
    ASSERT_COVERED(invq);

    quat Result;
    Result.X = -Left.X;
    Result.Y = -Left.Y;
    Result.Z = -Left.Z;
    Result.W = Left.W;

    return divqf(Result, (dotq(Left, Left)));
}

COVERAGE(normq, 1)
static inline quat normq(quat Quat)
{
    ASSERT_COVERED(normq);

    /* NOTE(lcf): Take advantage of SSE implementation in normv4 */
    vec4 Vec = {Quat.X, Quat.Y, Quat.Z, Quat.W};
    Vec = normv4(Vec);
    quat Result = {Vec.X, Vec.Y, Vec.Z, Vec.W};

    return Result;
}

static inline quat _mixq(quat Left, float MixLeft, quat Right, float MixRight) {
    quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 ScalarLeft = _mm_set1_ps(MixLeft);
    __m128 ScalarRight = _mm_set1_ps(MixRight);
    __m128 SSEResultOne = _mm_mul_ps(Left.SSE, ScalarLeft);
    __m128 SSEResultTwo = _mm_mul_ps(Right.SSE, ScalarRight);
    Result.SSE = _mm_add_ps(SSEResultOne, SSEResultTwo);
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t ScaledLeft = vmulq_n_f32(Left.NEON, MixLeft);
    float32x4_t ScaledRight = vmulq_n_f32(Right.NEON, MixRight);
    Result.NEON = vaddq_f32(ScaledLeft, ScaledRight);
#else
    Result.X = Left.X*MixLeft + Right.X*MixRight;
    Result.Y = Left.Y*MixLeft + Right.Y*MixRight;
    Result.Z = Left.Z*MixLeft + Right.Z*MixRight;
    Result.W = Left.W*MixLeft + Right.W*MixRight;
#endif

    return Result;
}

COVERAGE(nlerp, 1)
static inline quat nlerp(quat Left, float Time, quat Right)
{
    ASSERT_COVERED(nlerp);

    quat Result = _mixq(Left, 1.0f-Time, Right, Time);
    Result = normq(Result);

    return Result;
}

COVERAGE(slerp, 1)
static inline quat slerp(quat Left, float Time, quat Right)
{
    ASSERT_COVERED(slerp);

    quat Result;

    float Cos_Theta = dotq(Left, Right);

    if (Cos_Theta < 0.0f) { /* NOTE(lcf): Take shortest path on Hyper-sphere */
        Cos_Theta = -Cos_Theta;
        Right = q(-Right.X, -Right.Y, -Right.Z, -Right.W);
    }

    /* NOTE(lcf): Use Normalized Linear interpolation when vectors are roughly not L.I. */
    if (Cos_Theta > 0.9995f) {
        Result = nlerp(Left, Time, Right);
    } else {
        float Angle = acosf(Cos_Theta);
        float MixLeft = sinf((1.0f - Time) * Angle);
        float MixRight = sinf(Time * Angle);

        Result = _mixq(Left, MixLeft, Right, MixRight);
        Result = normq(Result);
    }

    return Result;
}

COVERAGE(qtom4, 1)
static inline mat4 qtom4(quat Left)
{
    ASSERT_COVERED(qtom4);

    mat4 Result;

    quat NormalizedQ = normq(Left);

    float XX, YY, ZZ,
          XY, XZ, YZ,
          WX, WY, WZ;

    XX = NormalizedQ.X * NormalizedQ.X;
    YY = NormalizedQ.Y * NormalizedQ.Y;
    ZZ = NormalizedQ.Z * NormalizedQ.Z;
    XY = NormalizedQ.X * NormalizedQ.Y;
    XZ = NormalizedQ.X * NormalizedQ.Z;
    YZ = NormalizedQ.Y * NormalizedQ.Z;
    WX = NormalizedQ.W * NormalizedQ.X;
    WY = NormalizedQ.W * NormalizedQ.Y;
    WZ = NormalizedQ.W * NormalizedQ.Z;

    Result.Elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    Result.Elements[0][1] = 2.0f * (XY + WZ);
    Result.Elements[0][2] = 2.0f * (XZ - WY);
    Result.Elements[0][3] = 0.0f;

    Result.Elements[1][0] = 2.0f * (XY - WZ);
    Result.Elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    Result.Elements[1][2] = 2.0f * (YZ + WX);
    Result.Elements[1][3] = 0.0f;

    Result.Elements[2][0] = 2.0f * (XZ + WY);
    Result.Elements[2][1] = 2.0f * (YZ - WX);
    Result.Elements[2][2] = 1.0f - 2.0f * (XX + YY);
    Result.Elements[2][3] = 0.0f;

    Result.Elements[3][0] = 0.0f;
    Result.Elements[3][1] = 0.0f;
    Result.Elements[3][2] = 0.0f;
    Result.Elements[3][3] = 1.0f;

    return Result;
}

// This method taken from Mike Day at Insomniac Games.
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
//
// Note that as mentioned at the top of the paper, the paper assumes the matrix
// would be *post*-multiplied to a vector to rotate it, meaning the matrix is
// the transpose of what we're dealing with. But, because our matrices are
// stored in column-major order, the indices *appear* to match the paper.
//
// For example, m12 in the paper is row 1, column 2. We need to transpose it to
// row 2, column 1. But, because the column comes first when referencing
// elements, it looks like M.Elements[1][2].
//
// Don't be confused! Or if you must be confused, at least trust this
// comment. :)
COVERAGE(m4toq_rh, 4)
static inline quat m4toq_rh(mat4 M)
{
    float T;
    quat Q;

    if (M.Elements[2][2] < 0.0f) {
        if (M.Elements[0][0] > M.Elements[1][1]) {
            ASSERT_COVERED(m4toq_rh);

            T = 1 + M.Elements[0][0] - M.Elements[1][1] - M.Elements[2][2];
            Q = q(
                T,
                M.Elements[0][1] + M.Elements[1][0],
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[1][2] - M.Elements[2][1]
            );
        } else {
            ASSERT_COVERED(m4toq_rh);

            T = 1 - M.Elements[0][0] + M.Elements[1][1] - M.Elements[2][2];
            Q = q(
                M.Elements[0][1] + M.Elements[1][0],
                T,
                M.Elements[1][2] + M.Elements[2][1],
                M.Elements[2][0] - M.Elements[0][2]
            );
        }
    } else {
        if (M.Elements[0][0] < -M.Elements[1][1]) {
            ASSERT_COVERED(m4toq_rh);

            T = 1 - M.Elements[0][0] - M.Elements[1][1] + M.Elements[2][2];
            Q = q(
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[1][2] + M.Elements[2][1],
                T,
                M.Elements[0][1] - M.Elements[1][0]
            );
        } else {
            ASSERT_COVERED(m4toq_rh);

            T = 1 + M.Elements[0][0] + M.Elements[1][1] + M.Elements[2][2];
            Q = q(
                M.Elements[1][2] - M.Elements[2][1],
                M.Elements[2][0] - M.Elements[0][2],
                M.Elements[0][1] - M.Elements[1][0],
                T
            );
        }
    }

    Q = mulqf(Q, 0.5f / sqrtf(T));

    return Q;
}

COVERAGE(m4toq_lh, 4)
static inline quat m4toq_lh(mat4 M)
{
    float T;
    quat Q;

    if (M.Elements[2][2] < 0.0f) {
        if (M.Elements[0][0] > M.Elements[1][1]) {
            ASSERT_COVERED(m4toq_lh);

            T = 1 + M.Elements[0][0] - M.Elements[1][1] - M.Elements[2][2];
            Q = q(
                T,
                M.Elements[0][1] + M.Elements[1][0],
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[2][1] - M.Elements[1][2]
            );
        } else {
            ASSERT_COVERED(m4toq_lh);

            T = 1 - M.Elements[0][0] + M.Elements[1][1] - M.Elements[2][2];
            Q = q(
                M.Elements[0][1] + M.Elements[1][0],
                T,
                M.Elements[1][2] + M.Elements[2][1],
                M.Elements[0][2] - M.Elements[2][0]
            );
        }
    } else {
        if (M.Elements[0][0] < -M.Elements[1][1]) {
            ASSERT_COVERED(m4toq_lh);

            T = 1 - M.Elements[0][0] - M.Elements[1][1] + M.Elements[2][2];
            Q = q(
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[1][2] + M.Elements[2][1],
                T,
                M.Elements[1][0] - M.Elements[0][1]
            );
        } else {
            ASSERT_COVERED(m4toq_lh);

            T = 1 + M.Elements[0][0] + M.Elements[1][1] + M.Elements[2][2];
            Q = q(
                M.Elements[2][1] - M.Elements[1][2],
                M.Elements[0][2] - M.Elements[2][0],
                M.Elements[1][0] - M.Elements[0][2],
                T
            );
        }
    }

    Q = mulqf(Q, 0.5f / sqrtf(T));

    return Q;
}


COVERAGE(qfromaxisangle_rh, 1)
static inline quat qfromaxisangle_rh(vec3 Axis, float Angle)
{
    ASSERT_COVERED(qfromaxisangle_rh);

    quat Result;

    vec3 AxisNormalized = normv3(Axis);
    float SineOfRotation = sinf(Angle / 2.0f);

    Result.XYZ = mulv3f(AxisNormalized, SineOfRotation);
    Result.W = cosf(Angle / 2.0f);

    return Result;
}

COVERAGE(qfromaxisangle_lh, 1)
static inline quat qfromaxisangle_lh(vec3 Axis, float Angle)
{
    ASSERT_COVERED(qfromaxisangle_lh);

    return qfromaxisangle_rh(Axis, -Angle);
}

COVERAGE(qfromnormpair, 1)
static inline quat qfromnormpair(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(qfromnormpair);

    quat Result;

    Result.XYZ = cross(Left, Right);
    Result.W = 1.0f + dotv3(Left, Right);

    return normq(Result);
}

COVERAGE(qfromvecpair, 1)
static inline quat qfromvecpair(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(qfromvecpair);

    return qfromnormpair(normv3(Left), normv3(Right));
}

COVERAGE(rotatev2, 1)
static inline vec2 rotatev2(vec2 V, float Angle)
{
    ASSERT_COVERED(rotatev2)

    float sinA = sinf(Angle);
    float cosA = cosf(Angle);

    return v2(V.X * cosA - V.Y * sinA, V.X * sinA + V.Y * cosA);
}

// implementation from
// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
COVERAGE(rotatev3q, 1)
static inline vec3 rotatev3q(vec3 V, quat Q)
{
    ASSERT_COVERED(rotatev3q);

    vec3 t = mulv3f(cross(Q.XYZ, V), 2);
    return addv3(V, addv3(mulv3f(t, Q.W), cross(Q.XYZ, t)));
}

COVERAGE(rotatev3axisangle_lh, 1)
static inline vec3 rotatev3axisangle_lh(vec3 V, vec3 Axis, float Angle) {
    ASSERT_COVERED(rotatev3axisangle_lh);

    return rotatev3q(V, qfromaxisangle_lh(Axis, Angle));
}

COVERAGE(rotatev3axisangle_rh, 1)
static inline vec3 rotatev3axisangle_rh(vec3 V, vec3 Axis, float Angle) {
    ASSERT_COVERED(rotatev3axisangle_rh);

    return rotatev3q(V, qfromaxisangle_rh(Axis, Angle));
}


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

COVERAGE(lenv2cpp, 1)
static inline float len(vec2 A)
{
    ASSERT_COVERED(lenv2cpp);
    return lenv2(A);
}

COVERAGE(lenv3cpp, 1)
static inline float len(vec3 A)
{
    ASSERT_COVERED(lenv3cpp);
    return lenv3(A);
}

COVERAGE(lenv4cpp, 1)
static inline float len(vec4 A)
{
    ASSERT_COVERED(lenv4cpp);
    return lenv4(A);
}

COVERAGE(lensqrv2cpp, 1)
static inline float lensqr(vec2 A)
{
    ASSERT_COVERED(lensqrv2cpp);
    return lensqrv2(A);
}

COVERAGE(lensqrv3cpp, 1)
static inline float lensqr(vec3 A)
{
    ASSERT_COVERED(lensqrv3cpp);
    return lensqrv3(A);
}

COVERAGE(lensqrv4cpp, 1)
static inline float lensqr(vec4 A)
{
    ASSERT_COVERED(lensqrv4cpp);
    return lensqrv4(A);
}

COVERAGE(normv2cpp, 1)
static inline vec2 norm(vec2 A)
{
    ASSERT_COVERED(normv2cpp);
    return normv2(A);
}

COVERAGE(normv3cpp, 1)
static inline vec3 norm(vec3 A)
{
    ASSERT_COVERED(normv3cpp);
    return normv3(A);
}

COVERAGE(normv4cpp, 1)
static inline vec4 norm(vec4 A)
{
    ASSERT_COVERED(normv4cpp);
    return normv4(A);
}

COVERAGE(normqcpp, 1)
static inline quat norm(quat A)
{
    ASSERT_COVERED(normqcpp);
    return normq(A);
}

COVERAGE(dotv2cpp, 1)
static inline float dot(vec2 Left, vec2 VecTwo)
{
    ASSERT_COVERED(dotv2cpp);
    return dotv2(Left, VecTwo);
}

COVERAGE(dotv3cpp, 1)
static inline float dot(vec3 Left, vec3 VecTwo)
{
    ASSERT_COVERED(dotv3cpp);
    return dotv3(Left, VecTwo);
}

COVERAGE(dotv4cpp, 1)
static inline float dot(vec4 Left, vec4 VecTwo)
{
    ASSERT_COVERED(dotv4cpp);
    return dotv4(Left, VecTwo);
}

COVERAGE(lerpv2cpp, 1)
static inline vec2 lerp(vec2 Left, float Time, vec2 Right)
{
    ASSERT_COVERED(lerpv2cpp);
    return lerpv2(Left, Time, Right);
}

COVERAGE(lerpv3cpp, 1)
static inline vec3 lerp(vec3 Left, float Time, vec3 Right)
{
    ASSERT_COVERED(lerpv3cpp);
    return lerpv3(Left, Time, Right);
}

COVERAGE(lerpv4cpp, 1)
static inline vec4 lerp(vec4 Left, float Time, vec4 Right)
{
    ASSERT_COVERED(lerpv4cpp);
    return lerpv4(Left, Time, Right);
}

COVERAGE(transposem2cpp, 1)
static inline mat2 transpose(mat2 Matrix)
{
    ASSERT_COVERED(transposem2cpp);
    return transposem2(Matrix);
}

COVERAGE(transposem3cpp, 1)
static inline mat3 transpose(mat3 Matrix)
{
    ASSERT_COVERED(transposem3cpp);
    return transposem3(Matrix);
}

COVERAGE(transposem4cpp, 1)
static inline mat4 transpose(mat4 Matrix)
{
    ASSERT_COVERED(transposem4cpp);
    return transposem4(Matrix);
}

COVERAGE(determinantm2cpp, 1)
static inline float determinant(mat2 Matrix)
{
    ASSERT_COVERED(determinantm2cpp);
    return determinantm2(Matrix);
}

COVERAGE(determinantm3cpp, 1)
static inline float determinant(mat3 Matrix)
{
    ASSERT_COVERED(determinantm3cpp);
    return determinantm3(Matrix);
}

COVERAGE(determinantm4cpp, 1)
static inline float determinant(mat4 Matrix)
{
    ASSERT_COVERED(determinantm4cpp);
    return determinantm4(Matrix);
}

COVERAGE(invgeneralm2cpp, 1)
static inline mat2 invgeneral(mat2 Matrix)
{
    ASSERT_COVERED(invgeneralm2cpp);
    return invgeneralm2(Matrix);
}

COVERAGE(invgeneralm3cpp, 1)
static inline mat3 invgeneral(mat3 Matrix)
{
    ASSERT_COVERED(invgeneralm3cpp);
    return invgeneralm3(Matrix);
}

COVERAGE(invgeneralm4cpp, 1)
static inline mat4 invgeneral(mat4 Matrix)
{
    ASSERT_COVERED(invgeneralm4cpp);
    return invgeneralm4(Matrix);
}

COVERAGE(dotqcpp, 1)
static inline float dot(quat QuatOne, quat QuatTwo)
{
    ASSERT_COVERED(dotqcpp);
    return dotq(QuatOne, QuatTwo);
}

COVERAGE(addv2cpp, 1)
static inline vec2 add(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(addv2cpp);
    return addv2(Left, Right);
}

COVERAGE(addv3cpp, 1)
static inline vec3 add(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(addv3cpp);
    return addv3(Left, Right);
}

COVERAGE(addv4cpp, 1)
static inline vec4 add(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(addv4cpp);
    return addv4(Left, Right);
}

COVERAGE(addm2cpp, 1)
static inline mat2 add(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(addm2cpp);
    return addm2(Left, Right);
}

COVERAGE(addm3cpp, 1)
static inline mat3 add(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(addm3cpp);
    return addm3(Left, Right);
}

COVERAGE(addm4cpp, 1)
static inline mat4 add(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(addm4cpp);
    return addm4(Left, Right);
}

COVERAGE(addqcpp, 1)
static inline quat add(quat Left, quat Right)
{
    ASSERT_COVERED(addqcpp);
    return addq(Left, Right);
}

COVERAGE(subv2cpp, 1)
static inline vec2 sub(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(subv2cpp);
    return subv2(Left, Right);
}

COVERAGE(subv3cpp, 1)
static inline vec3 sub(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(subv3cpp);
    return subv3(Left, Right);
}

COVERAGE(subv4cpp, 1)
static inline vec4 sub(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(subv4cpp);
    return subv4(Left, Right);
}

COVERAGE(subm2cpp, 1)
static inline mat2 sub(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(subm2cpp);
    return subm2(Left, Right);
}

COVERAGE(subm3cpp, 1)
static inline mat3 sub(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(subm3cpp);
    return subm3(Left, Right);
}

COVERAGE(subm4cpp, 1)
static inline mat4 sub(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(subm4cpp);
    return subm4(Left, Right);
}

COVERAGE(subqcpp, 1)
static inline quat sub(quat Left, quat Right)
{
    ASSERT_COVERED(subqcpp);
    return subq(Left, Right);
}

COVERAGE(mulv2cpp, 1)
static inline vec2 mul(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(mulv2cpp);
    return mulv2(Left, Right);
}

COVERAGE(mulv2fcpp, 1)
static inline vec2 mul(vec2 Left, float Right)
{
    ASSERT_COVERED(mulv2fcpp);
    return mulv2f(Left, Right);
}

COVERAGE(mulv3cpp, 1)
static inline vec3 mul(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(mulv3cpp);
    return mulv3(Left, Right);
}

COVERAGE(mulv3fcpp, 1)
static inline vec3 mul(vec3 Left, float Right)
{
    ASSERT_COVERED(mulv3fcpp);
    return mulv3f(Left, Right);
}

COVERAGE(mulv4cpp, 1)
static inline vec4 mul(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(mulv4cpp);
    return mulv4(Left, Right);
}

COVERAGE(mulv4fcpp, 1)
static inline vec4 mul(vec4 Left, float Right)
{
    ASSERT_COVERED(mulv4fcpp);
    return mulv4f(Left, Right);
}

COVERAGE(mulm2cpp, 1)
static inline mat2 mul(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(mulm2cpp);
    return mulm2(Left, Right);
}

COVERAGE(mulm3cpp, 1)
static inline mat3 mul(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(mulm3cpp);
    return mulm3(Left, Right);
}

COVERAGE(mulm4cpp, 1)
static inline mat4 mul(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(mulm4cpp);
    return mulm4(Left, Right);
}

COVERAGE(mulm2fcpp, 1)
static inline mat2 mul(mat2 Left, float Right)
{
    ASSERT_COVERED(mulm2fcpp);
    return mulm2f(Left, Right);
}

COVERAGE(mulm3fcpp, 1)
static inline mat3 mul(mat3 Left, float Right)
{
    ASSERT_COVERED(mulm3fcpp);
    return mulm3f(Left, Right);
}

COVERAGE(mulm4fcpp, 1)
static inline mat4 mul(mat4 Left, float Right)
{
    ASSERT_COVERED(mulm4fcpp);
    return mulm4f(Left, Right);
}

COVERAGE(mulm2v2cpp, 1)
static inline vec2 mul(mat2 Matrix, vec2 Vector)
{
    ASSERT_COVERED(mulm2v2cpp);
    return mulm2v2(Matrix, Vector);
}

COVERAGE(mulm3v3cpp, 1)
static inline vec3 mul(mat3 Matrix, vec3 Vector)
{
    ASSERT_COVERED(mulm3v3cpp);
    return mulm3v3(Matrix, Vector);
}

COVERAGE(mulm4v4cpp, 1)
static inline vec4 mul(mat4 Matrix, vec4 Vector)
{
    ASSERT_COVERED(mulm4v4cpp);
    return mulm4v4(Matrix, Vector);
}

COVERAGE(mulqcpp, 1)
static inline quat mul(quat Left, quat Right)
{
    ASSERT_COVERED(mulqcpp);
    return mulq(Left, Right);
}

COVERAGE(mulqfcpp, 1)
static inline quat mul(quat Left, float Right)
{
    ASSERT_COVERED(mulqfcpp);
    return mulqf(Left, Right);
}

COVERAGE(divv2cpp, 1)
static inline vec2 div(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(divv2cpp);
    return divv2(Left, Right);
}

COVERAGE(divv2fcpp, 1)
static inline vec2 div(vec2 Left, float Right)
{
    ASSERT_COVERED(divv2fcpp);
    return divv2f(Left, Right);
}

COVERAGE(divv3cpp, 1)
static inline vec3 div(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(divv3cpp);
    return divv3(Left, Right);
}

COVERAGE(divv3fcpp, 1)
static inline vec3 div(vec3 Left, float Right)
{
    ASSERT_COVERED(divv3fcpp);
    return divv3f(Left, Right);
}

COVERAGE(divv4cpp, 1)
static inline vec4 div(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(divv4cpp);
    return divv4(Left, Right);
}

COVERAGE(divv4fcpp, 1)
static inline vec4 div(vec4 Left, float Right)
{
    ASSERT_COVERED(divv4fcpp);
    return divv4f(Left, Right);
}

COVERAGE(divm2fcpp, 1)
static inline mat2 div(mat2 Left, float Right)
{
    ASSERT_COVERED(divm2fcpp);
    return divm2f(Left, Right);
}

COVERAGE(divm3fcpp, 1)
static inline mat3 div(mat3 Left, float Right)
{
    ASSERT_COVERED(divm3fcpp);
    return divm3f(Left, Right);
}

COVERAGE(divm4fcpp, 1)
static inline mat4 div(mat4 Left, float Right)
{
    ASSERT_COVERED(divm4fcpp);
    return divm4f(Left, Right);
}

COVERAGE(divqfcpp, 1)
static inline quat div(quat Left, float Right)
{
    ASSERT_COVERED(divqfcpp);
    return divqf(Left, Right);
}

COVERAGE(eqv2cpp, 1)
static inline bool eq(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(eqv2cpp);
    return eqv2(Left, Right);
}

COVERAGE(eqv3cpp, 1)
static inline bool eq(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(eqv3cpp);
    return eqv3(Left, Right);
}

COVERAGE(eqv4cpp, 1)
static inline bool eq(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(eqv4cpp);
    return eqv4(Left, Right);
}

COVERAGE(addv2op, 1)
static inline vec2 operator+(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(addv2op);
    return addv2(Left, Right);
}

COVERAGE(addv3op, 1)
static inline vec3 operator+(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(addv3op);
    return addv3(Left, Right);
}

COVERAGE(addv4op, 1)
static inline vec4 operator+(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(addv4op);
    return addv4(Left, Right);
}

COVERAGE(addm2op, 1)
static inline mat2 operator+(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(addm2op);
    return addm2(Left, Right);
}

COVERAGE(addm3op, 1)
static inline mat3 operator+(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(addm3op);
    return addm3(Left, Right);
}

COVERAGE(addm4op, 1)
static inline mat4 operator+(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(addm4op);
    return addm4(Left, Right);
}

COVERAGE(addqop, 1)
static inline quat operator+(quat Left, quat Right)
{
    ASSERT_COVERED(addqop);
    return addq(Left, Right);
}

COVERAGE(subv2op, 1)
static inline vec2 operator-(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(subv2op);
    return subv2(Left, Right);
}

COVERAGE(subv3op, 1)
static inline vec3 operator-(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(subv3op);
    return subv3(Left, Right);
}

COVERAGE(subv4op, 1)
static inline vec4 operator-(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(subv4op);
    return subv4(Left, Right);
}

COVERAGE(subm2op, 1)
static inline mat2 operator-(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(subm2op);
    return subm2(Left, Right);
}

COVERAGE(subm3op, 1)
static inline mat3 operator-(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(subm3op);
    return subm3(Left, Right);
}

COVERAGE(subm4op, 1)
static inline mat4 operator-(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(subm4op);
    return subm4(Left, Right);
}

COVERAGE(subqop, 1)
static inline quat operator-(quat Left, quat Right)
{
    ASSERT_COVERED(subqop);
    return subq(Left, Right);
}

COVERAGE(mulv2op, 1)
static inline vec2 operator*(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(mulv2op);
    return mulv2(Left, Right);
}

COVERAGE(mulv3op, 1)
static inline vec3 operator*(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(mulv3op);
    return mulv3(Left, Right);
}

COVERAGE(mulv4op, 1)
static inline vec4 operator*(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(mulv4op);
    return mulv4(Left, Right);
}

COVERAGE(mulm2op, 1)
static inline mat2 operator*(mat2 Left, mat2 Right)
{
    ASSERT_COVERED(mulm2op);
    return mulm2(Left, Right);
}

COVERAGE(mulm3op, 1)
static inline mat3 operator*(mat3 Left, mat3 Right)
{
    ASSERT_COVERED(mulm3op);
    return mulm3(Left, Right);
}

COVERAGE(mulm4op, 1)
static inline mat4 operator*(mat4 Left, mat4 Right)
{
    ASSERT_COVERED(mulm4op);
    return mulm4(Left, Right);
}

COVERAGE(mulqop, 1)
static inline quat operator*(quat Left, quat Right)
{
    ASSERT_COVERED(mulqop);
    return mulq(Left, Right);
}

COVERAGE(mulv2fop, 1)
static inline vec2 operator*(vec2 Left, float Right)
{
    ASSERT_COVERED(mulv2fop);
    return mulv2f(Left, Right);
}

COVERAGE(mulv3fop, 1)
static inline vec3 operator*(vec3 Left, float Right)
{
    ASSERT_COVERED(mulv3fop);
    return mulv3f(Left, Right);
}

COVERAGE(mulv4fop, 1)
static inline vec4 operator*(vec4 Left, float Right)
{
    ASSERT_COVERED(mulv4fop);
    return mulv4f(Left, Right);
}

COVERAGE(mulm2fop, 1)
static inline mat2 operator*(mat2 Left, float Right)
{
    ASSERT_COVERED(mulm2fop);
    return mulm2f(Left, Right);
}

COVERAGE(mulm3fop, 1)
static inline mat3 operator*(mat3 Left, float Right)
{
    ASSERT_COVERED(mulm3fop);
    return mulm3f(Left, Right);
}

COVERAGE(mulm4fop, 1)
static inline mat4 operator*(mat4 Left, float Right)
{
    ASSERT_COVERED(mulm4fop);
    return mulm4f(Left, Right);
}

COVERAGE(mulqfop, 1)
static inline quat operator*(quat Left, float Right)
{
    ASSERT_COVERED(mulqfop);
    return mulqf(Left, Right);
}

COVERAGE(mulv2fopleft, 1)
static inline vec2 operator*(float Left, vec2 Right)
{
    ASSERT_COVERED(mulv2fopleft);
    return mulv2f(Right, Left);
}

COVERAGE(mulv3fopleft, 1)
static inline vec3 operator*(float Left, vec3 Right)
{
    ASSERT_COVERED(mulv3fopleft);
    return mulv3f(Right, Left);
}

COVERAGE(mulv4fopleft, 1)
static inline vec4 operator*(float Left, vec4 Right)
{
    ASSERT_COVERED(mulv4fopleft);
    return mulv4f(Right, Left);
}

COVERAGE(mulm2fopleft, 1)
static inline mat2 operator*(float Left, mat2 Right)
{
    ASSERT_COVERED(mulm2fopleft);
    return mulm2f(Right, Left);
}

COVERAGE(mulm3fopleft, 1)
static inline mat3 operator*(float Left, mat3 Right)
{
    ASSERT_COVERED(mulm3fopleft);
    return mulm3f(Right, Left);
}

COVERAGE(mulm4fopleft, 1)
static inline mat4 operator*(float Left, mat4 Right)
{
    ASSERT_COVERED(mulm4fopleft);
    return mulm4f(Right, Left);
}

COVERAGE(mulqfopleft, 1)
static inline quat operator*(float Left, quat Right)
{
    ASSERT_COVERED(mulqfopleft);
    return mulqf(Right, Left);
}

COVERAGE(mulm2v2op, 1)
static inline vec2 operator*(mat2 Matrix, vec2 Vector)
{
    ASSERT_COVERED(mulm2v2op);
    return mulm2v2(Matrix, Vector);
}

COVERAGE(mulm3v3op, 1)
static inline vec3 operator*(mat3 Matrix, vec3 Vector)
{
    ASSERT_COVERED(mulm3v3op);
    return mulm3v3(Matrix, Vector);
}

COVERAGE(mulm4v4op, 1)
static inline vec4 operator*(mat4 Matrix, vec4 Vector)
{
    ASSERT_COVERED(mulm4v4op);
    return mulm4v4(Matrix, Vector);
}

COVERAGE(divv2op, 1)
static inline vec2 operator/(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(divv2op);
    return divv2(Left, Right);
}

COVERAGE(divv3op, 1)
static inline vec3 operator/(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(divv3op);
    return divv3(Left, Right);
}

COVERAGE(divv4op, 1)
static inline vec4 operator/(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(divv4op);
    return divv4(Left, Right);
}

COVERAGE(divv2fop, 1)
static inline vec2 operator/(vec2 Left, float Right)
{
    ASSERT_COVERED(divv2fop);
    return divv2f(Left, Right);
}

COVERAGE(divv3fop, 1)
static inline vec3 operator/(vec3 Left, float Right)
{
    ASSERT_COVERED(divv3fop);
    return divv3f(Left, Right);
}

COVERAGE(divv4fop, 1)
static inline vec4 operator/(vec4 Left, float Right)
{
    ASSERT_COVERED(divv4fop);
    return divv4f(Left, Right);
}

COVERAGE(divm4fop, 1)
static inline mat4 operator/(mat4 Left, float Right)
{
    ASSERT_COVERED(divm4fop);
    return divm4f(Left, Right);
}

COVERAGE(divm3fop, 1)
static inline mat3 operator/(mat3 Left, float Right)
{
    ASSERT_COVERED(divm3fop);
    return divm3f(Left, Right);
}

COVERAGE(divm2fop, 1)
static inline mat2 operator/(mat2 Left, float Right)
{
    ASSERT_COVERED(divm2fop);
    return divm2f(Left, Right);
}

COVERAGE(divqfop, 1)
static inline quat operator/(quat Left, float Right)
{
    ASSERT_COVERED(divqfop);
    return divqf(Left, Right);
}

COVERAGE(addv2assign, 1)
static inline vec2 &operator+=(vec2 &Left, vec2 Right)
{
    ASSERT_COVERED(addv2assign);
    return Left = Left + Right;
}

COVERAGE(addv3assign, 1)
static inline vec3 &operator+=(vec3 &Left, vec3 Right)
{
    ASSERT_COVERED(addv3assign);
    return Left = Left + Right;
}

COVERAGE(addv4assign, 1)
static inline vec4 &operator+=(vec4 &Left, vec4 Right)
{
    ASSERT_COVERED(addv4assign);
    return Left = Left + Right;
}

COVERAGE(addm2assign, 1)
static inline mat2 &operator+=(mat2 &Left, mat2 Right)
{
    ASSERT_COVERED(addm2assign);
    return Left = Left + Right;
}

COVERAGE(addm3assign, 1)
static inline mat3 &operator+=(mat3 &Left, mat3 Right)
{
    ASSERT_COVERED(addm3assign);
    return Left = Left + Right;
}

COVERAGE(addm4assign, 1)
static inline mat4 &operator+=(mat4 &Left, mat4 Right)
{
    ASSERT_COVERED(addm4assign);
    return Left = Left + Right;
}

COVERAGE(addqassign, 1)
static inline quat &operator+=(quat &Left, quat Right)
{
    ASSERT_COVERED(addqassign);
    return Left = Left + Right;
}

COVERAGE(subv2assign, 1)
static inline vec2 &operator-=(vec2 &Left, vec2 Right)
{
    ASSERT_COVERED(subv2assign);
    return Left = Left - Right;
}

COVERAGE(subv3assign, 1)
static inline vec3 &operator-=(vec3 &Left, vec3 Right)
{
    ASSERT_COVERED(subv3assign);
    return Left = Left - Right;
}

COVERAGE(subv4assign, 1)
static inline vec4 &operator-=(vec4 &Left, vec4 Right)
{
    ASSERT_COVERED(subv4assign);
    return Left = Left - Right;
}

COVERAGE(subm2assign, 1)
static inline mat2 &operator-=(mat2 &Left, mat2 Right)
{
    ASSERT_COVERED(subm2assign);
    return Left = Left - Right;
}

COVERAGE(subm3assign, 1)
static inline mat3 &operator-=(mat3 &Left, mat3 Right)
{
    ASSERT_COVERED(subm3assign);
    return Left = Left - Right;
}

COVERAGE(subm4assign, 1)
static inline mat4 &operator-=(mat4 &Left, mat4 Right)
{
    ASSERT_COVERED(subm4assign);
    return Left = Left - Right;
}

COVERAGE(subqassign, 1)
static inline quat &operator-=(quat &Left, quat Right)
{
    ASSERT_COVERED(subqassign);
    return Left = Left - Right;
}

COVERAGE(mulv2assign, 1)
static inline vec2 &operator*=(vec2 &Left, vec2 Right)
{
    ASSERT_COVERED(mulv2assign);
    return Left = Left * Right;
}

COVERAGE(mulv3assign, 1)
static inline vec3 &operator*=(vec3 &Left, vec3 Right)
{
    ASSERT_COVERED(mulv3assign);
    return Left = Left * Right;
}

COVERAGE(mulv4assign, 1)
static inline vec4 &operator*=(vec4 &Left, vec4 Right)
{
    ASSERT_COVERED(mulv4assign);
    return Left = Left * Right;
}

COVERAGE(mulv2fassign, 1)
static inline vec2 &operator*=(vec2 &Left, float Right)
{
    ASSERT_COVERED(mulv2fassign);
    return Left = Left * Right;
}

COVERAGE(mulv3fassign, 1)
static inline vec3 &operator*=(vec3 &Left, float Right)
{
    ASSERT_COVERED(mulv3fassign);
    return Left = Left * Right;
}

COVERAGE(mulv4fassign, 1)
static inline vec4 &operator*=(vec4 &Left, float Right)
{
    ASSERT_COVERED(mulv4fassign);
    return Left = Left * Right;
}

COVERAGE(mulm2fassign, 1)
static inline mat2 &operator*=(mat2 &Left, float Right)
{
    ASSERT_COVERED(mulm2fassign);
    return Left = Left * Right;
}

COVERAGE(mulm3fassign, 1)
static inline mat3 &operator*=(mat3 &Left, float Right)
{
    ASSERT_COVERED(mulm3fassign);
    return Left = Left * Right;
}

COVERAGE(mulm4fassign, 1)
static inline mat4 &operator*=(mat4 &Left, float Right)
{
    ASSERT_COVERED(mulm4fassign);
    return Left = Left * Right;
}

COVERAGE(mulqfassign, 1)
static inline quat &operator*=(quat &Left, float Right)
{
    ASSERT_COVERED(mulqfassign);
    return Left = Left * Right;
}

COVERAGE(divv2assign, 1)
static inline vec2 &operator/=(vec2 &Left, vec2 Right)
{
    ASSERT_COVERED(divv2assign);
    return Left = Left / Right;
}

COVERAGE(divv3assign, 1)
static inline vec3 &operator/=(vec3 &Left, vec3 Right)
{
    ASSERT_COVERED(divv3assign);
    return Left = Left / Right;
}

COVERAGE(divv4assign, 1)
static inline vec4 &operator/=(vec4 &Left, vec4 Right)
{
    ASSERT_COVERED(divv4assign);
    return Left = Left / Right;
}

COVERAGE(divv2fassign, 1)
static inline vec2 &operator/=(vec2 &Left, float Right)
{
    ASSERT_COVERED(divv2fassign);
    return Left = Left / Right;
}

COVERAGE(divv3fassign, 1)
static inline vec3 &operator/=(vec3 &Left, float Right)
{
    ASSERT_COVERED(divv3fassign);
    return Left = Left / Right;
}

COVERAGE(divv4fassign, 1)
static inline vec4 &operator/=(vec4 &Left, float Right)
{
    ASSERT_COVERED(divv4fassign);
    return Left = Left / Right;
}

COVERAGE(divm4fassign, 1)
static inline mat4 &operator/=(mat4 &Left, float Right)
{
    ASSERT_COVERED(divm4fassign);
    return Left = Left / Right;
}

COVERAGE(divqfassign, 1)
static inline quat &operator/=(quat &Left, float Right)
{
    ASSERT_COVERED(divqfassign);
    return Left = Left / Right;
}

COVERAGE(eqv2op, 1)
static inline bool operator==(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(eqv2op);
    return eqv2(Left, Right);
}

COVERAGE(eqv3op, 1)
static inline bool operator==(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(eqv3op);
    return eqv3(Left, Right);
}

COVERAGE(eqv4op, 1)
static inline bool operator==(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(eqv4op);
    return eqv4(Left, Right);
}

COVERAGE(eqv2opnot, 1)
static inline bool operator!=(vec2 Left, vec2 Right)
{
    ASSERT_COVERED(eqv2opnot);
    return !eqv2(Left, Right);
}

COVERAGE(eqv3opnot, 1)
static inline bool operator!=(vec3 Left, vec3 Right)
{
    ASSERT_COVERED(eqv3opnot);
    return !eqv3(Left, Right);
}

COVERAGE(eqv4opnot, 1)
static inline bool operator!=(vec4 Left, vec4 Right)
{
    ASSERT_COVERED(eqv4opnot);
    return !eqv4(Left, Right);
}

COVERAGE(unaryminusv2, 1)
static inline vec2 operator-(vec2 In)
{
    ASSERT_COVERED(unaryminusv2);

    vec2 Result;
    Result.X = -In.X;
    Result.Y = -In.Y;

    return Result;
}

COVERAGE(unaryminusv3, 1)
static inline vec3 operator-(vec3 In)
{
    ASSERT_COVERED(unaryminusv3);

    vec3 Result;
    Result.X = -In.X;
    Result.Y = -In.Y;
    Result.Z = -In.Z;

    return Result;
}

COVERAGE(unaryminusv4, 1)
static inline vec4 operator-(vec4 In)
{
    ASSERT_COVERED(unaryminusv4);

    vec4 Result;
#if HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_xor_ps(In.SSE, _mm_set1_ps(-0.0f));
#elif defined(HANDMADE_MATH__USE_NEON)
    float32x4_t Zero = vdupq_n_f32(0.0f);
    Result.NEON = vsubq_f32(Zero, In.NEON);
#else
    Result.X = -In.X;
    Result.Y = -In.Y;
    Result.Z = -In.Z;
    Result.W = -In.W;
#endif

    return Result;
}

#endif /* __cplusplus*/

#ifdef HANDMADE_MATH__USE_C11_GENERICS
#define add(A, B) _Generic((A), \
        vec2: addv2, \
        vec3: addv3, \
        vec4: addv4, \
        mat2: addm2, \
        mat3: addm3, \
        mat4: addm4, \
        quat: addq \
)(A, B)

#define sub(A, B) _Generic((A), \
        vec2: subv2, \
        vec3: subv3, \
        vec4: subv4, \
        mat2: subm2, \
        mat3: subm3, \
        mat4: subm4, \
        quat: subq \
)(A, B)

#define mul(A, B) _Generic((B), \
     float: _Generic((A), \
        vec2: mulv2f, \
        vec3: mulv3f, \
        vec4: mulv4f, \
        mat2: mulm2f, \
        mat3: mulm3f, \
        mat4: mulm4f, \
        quat: mulqf \
     ), \
     mat2: mulm2, \
     mat3: mulm3, \
     mat4: mulm4, \
     quat: mulq, \
     default: _Generic((A), \
        vec2: mulv2, \
        vec3: mulv3, \
        vec4: mulv4, \
        mat2: mulm2v2, \
        mat3: mulm3v3, \
        mat4: mulm4v4 \
    ) \
)(A, B)

#define div(A, B) _Generic((B), \
     float: _Generic((A), \
        mat2: divm2f, \
        mat3: divm3f, \
        mat4: divm4f, \
        vec2: divv2f, \
        vec3: divv3f, \
        vec4: divv4f, \
        quat: divqf  \
     ), \
     mat2: divm2, \
     mat3: divm3, \
     mat4: divm4, \
     quat: divq, \
     default: _Generic((A), \
        vec2: divv2, \
        vec3: divv3, \
        vec4: divv4  \
    ) \
)(A, B)

#define len(A) _Generic((A), \
        vec2: lenv2, \
        vec3: lenv3, \
        vec4: lenv4  \
)(A)

#define lensqr(A) _Generic((A), \
        vec2: lensqrv2, \
        vec3: lensqrv3, \
        vec4: lensqrv4  \
)(A)

#define norm(A) _Generic((A), \
        vec2: normv2, \
        vec3: normv3, \
        vec4: normv4  \
)(A)

#define dot(A, B) _Generic((A), \
        vec2: dotv2, \
        vec3: dotv3, \
        vec4: dotv4  \
)(A, B)

#define lerp(A, T, B) _Generic((A), \
        float: lerp, \
        vec2: lerpv2, \
        vec3: lerpv3, \
        vec4: lerpv4 \
)(A, T, B)

#define eq(A, B) _Generic((A), \
        vec2: eqv2, \
        vec3: eqv3, \
        vec4: eqv4  \
)(A, B)

#define transpose(M) _Generic((M), \
        mat2: transposem2, \
        mat3: transposem3, \
        mat4: transposem4  \
)(M)

#define determinant(M) _Generic((M), \
        mat2: determinantm2, \
        mat3: determinantm3, \
        mat4: determinantm4  \
)(M)

#define invgeneral(M) _Generic((M), \
        mat2: invgeneralm2, \
        mat3: invgeneralm3, \
        mat4: invgeneralm4  \
)(M)

#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif /* HANDMADE_MATH_H */
