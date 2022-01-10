#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

namespace SBL
{
namespace Math
{
typedef int int32;
typedef float float32;
typedef float32 Radians;
typedef float32 Degrees;

constexpr float32 float32_epsilon = 0.000001f;
constexpr float32 PI = 3.1415926535897932384f;
constexpr float32 DegreesToRadiansValue = PI / 180.0f;
constexpr float32 RadiansToDegreesValue = 180.0f / PI;

// quiet_NaN is not guaranteed to be constexpr
const float32 NaN = std::numeric_limits<float32>::quiet_NaN();

template <typename T> inline bool Equals(T lhs, T rhs)
{
	return lhs == rhs;
}

inline bool Equals(float32 lhs, float32 rhs);

inline int32 Abs(int32 value);
inline float32 Abs(float32 value);

inline bool IsZero(float32 value);
inline float32 MakeZero(float32 value);

inline float32 Floor(float32 val);
inline float32 Ceil(float32 val);

inline float32 Frac(float32 val);

inline float32 Lerp(float32 valA, float32 valB, float32 t);

inline Radians DegreesToRadians(Degrees degrees);
inline Degrees RadiansToDegrees(Radians radians);
inline float32 Sin(Radians radians);
inline float32 Cos(Radians radians);
inline float32 Tan(Radians radians);
inline Radians ArcSin(float32 val);
inline Radians ArcCos(float32 val);
inline Radians ArcTan(float32 val);
inline Radians ArcTan(float32 x, float32 y);

inline float32 Pow(float32 val, float32 exponent);
inline float32 Exp(float32 val);
constexpr float32 Pow2(float32 val);
constexpr float32 Pow3(float32 val);
template <typename ValueType>
constexpr ValueType Clamp(ValueType val, ValueType min, ValueType max);
}
}

#include <SBLMath/Common.inl>
