#pragma once

#include <SBLMath/Common.hpp>

namespace SBL
{
namespace Math
{
inline bool Equals(float32 lhs, float32 rhs)
{
	return IsZero(rhs - lhs);
}

inline int32 Abs(int32 value)
{
	return value < 0 ? -value : value;
}

inline float32 Abs(float32 value)
{
	using namespace std;
	return fabsf(value);
}

inline bool IsZero(float32 value)
{
	return Abs(value) < float32_epsilon;
}

inline float32 MakeZero(float32 value)
{
	return IsZero(value) ? 0.0f : value;
}

inline float32 Floor(float32 val)
{
	using namespace std;
	return floorf(val);
}

inline float32 Ceil(float32 val)
{
	using namespace std;
	return ceilf(val);
}

inline float32 Frac(float32 val)
{
	return Abs(val - Floor(val));
}

inline float32 Lerp(float32 valA, float32 valB, float32 t)
{
	return valA + ((valB - valA) * t);
}

inline Radians DegreesToRadians(Degrees degrees)
{
	return static_cast<Radians>(degrees * DegreesToRadiansValue);
}

inline Degrees RadiansToDegrees(Radians radians)
{
	return static_cast<Degrees>(radians * RadiansToDegreesValue);
}

inline float32 Sin(Radians radians)
{
	using namespace std;
	return MakeZero(sin(radians));
}

inline float32 Cos(Radians radians)
{
	using namespace std;
	return MakeZero(cos(radians));
}

inline float32 Tan(Radians radians)
{
	using namespace std;
	return MakeZero(tan(radians));
}

inline Radians ArcSin(float32 val)
{
	using namespace std;
	return static_cast<Radians>(MakeZero(asin(val)));
}

inline Radians ArcCos(float32 val)
{
	using namespace std;
	return static_cast<Radians>(MakeZero(acos(val)));
}

inline Radians ArcTan(float32 val)
{
	using namespace std;
	return static_cast<Radians>(MakeZero(atan(val)));
}

inline Radians ArcTan(float32 x, float32 y)
{
	using namespace std;
	return static_cast<Radians>(MakeZero(atan2(y, x)));
}

inline float32 Pow(float32 val, float32 exponent)
{
	using namespace std;
	return powf(val, exponent);
}

inline float32 Exp(float32 val)
{
	using namespace std;
	return expf(val);
}

constexpr float32 Pow2(float32 val)
{
	return val * val;
}

constexpr float32 Pow3(float32 val)
{
	return val * val * val;
}

template <typename ValueType>
constexpr ValueType Clamp(ValueType val, ValueType min, ValueType max)
{
	return std::min(max, std::max(min, val));
}
}
}
