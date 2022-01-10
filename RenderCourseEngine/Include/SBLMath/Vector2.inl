#pragma once

#include <SBLMath/Vector2.hpp>

namespace SBL
{
namespace Math
{
inline Vector2::Vector2(float32 _x, float32 _y) : x(_x), y(_y)
{
}

inline float32& Vector2::operator[](std::size_t index)
{
	return values[index];
}

inline const float32& Vector2::operator[](std::size_t index) const
{
	return values[index];
}

inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{
	return Equals(lhs.x, rhs.x) && Equals(lhs.y, rhs.y);
}

inline bool operator!=(const Vector2& lhs, const Vector2& rhs)
{
	return !(lhs == rhs);
}

inline Vector2 operator-(const Vector2& vec)
{
	return Vector2{-vec.x, -vec.y};
}

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2{lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2{lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2{lhs.x * rhs.x, lhs.y * rhs.y};
}

inline Vector2 operator/(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2{lhs.x / rhs.x, lhs.y / rhs.y};
}

inline Vector2& operator+=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline Vector2& operator-=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline Vector2& operator*=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline Vector2& operator/=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}

inline Vector2 operator+(const Vector2& lhs, float32 rhs)
{
	return Vector2{lhs.x + rhs, lhs.y + rhs};
}

inline Vector2 operator-(const Vector2& lhs, float32 rhs)
{
	return Vector2{lhs.x - rhs, lhs.y - rhs};
}

inline Vector2 operator*(const Vector2& lhs, float32 rhs)
{
	return Vector2{lhs.x * rhs, lhs.y * rhs};
}

inline Vector2 operator/(const Vector2& lhs, float32 rhs)
{
	return Vector2{lhs.x / rhs, lhs.y / rhs};
}

inline Vector2 operator+(float32 lhs, const Vector2& rhs)
{
	return Vector2{lhs + rhs.x, lhs + rhs.y};
}

inline Vector2 operator-(float32 lhs, const Vector2& rhs)
{
	return Vector2{lhs - rhs.x, lhs - rhs.y};
}

inline Vector2 operator*(float32 lhs, const Vector2& rhs)
{
	return Vector2{lhs * rhs.x, lhs * rhs.y};
}

inline Vector2 operator/(float32 lhs, const Vector2& rhs)
{
	return Vector2{lhs / rhs.x, lhs / rhs.y};
}

inline Vector2& operator+=(Vector2& lhs, float32 rhs)
{
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

inline Vector2& operator-=(Vector2& lhs, float32 rhs)
{
	lhs.x -= rhs;
	lhs.y -= rhs;
	return lhs;
}

inline Vector2& operator*=(Vector2& lhs, float32 rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

inline Vector2& operator/=(Vector2& lhs, float32 rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

inline float32 Sum(const Vector2& vec)
{
	return vec.x + vec.y;
}

inline float32 LengthSquared(const Vector2& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

inline float32 Length(const Vector2& vec)
{
	return sqrtf(LengthSquared(vec));
}

inline Vector2 Normalized(const Vector2& vec)
{
	return vec / Length(vec);
}

inline float32 DotProduct(const Vector2& lhs, const Vector2& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline Radians AngleBetween(const Vector2& lhs, const Vector2& rhs)
{
	// lhs . rhs = |lhs|*|rhs|*cos(t)
	// cos(t) = lhs . rhs / (|lhs| * |rhs|)
	// t = acos(lhs . rhs / (|lhs| * |rhs|))
	return acos(DotProduct(lhs, rhs)/(Length(lhs)*Length(rhs)));
}

inline Vector2 Reflection(const Vector2& direction, const Vector2& normal)
{
	// See Vector3 reflection for more derivation 
	return direction - 2 * DotProduct(direction, normal) * normal;
}

inline Vector2 Projection(const Vector2& source, const Vector2& target)
{
	return DotProduct(source, target) * target;
}

inline Vector2 Rejection(const Vector2& source, const Vector2& target)
{
	return source - Projection(source, target);
}

inline Vector2 Lerp(const Vector2& source, const Vector2& target,
					float32 portion)
{
	return source + ((target - source) * portion);
}

inline Vector2 Min(const Vector2& a, const Vector2& b)
{
	return {std::min(a.x, b.x), std::min(a.y, b.y)};
}

inline Vector2 Max(const Vector2& a, const Vector2& b)
{
	return {std::max(a.x, b.x), std::max(a.y, b.y)};
}

inline Vector2 Clamp(const Vector2& val, float32 min, float32 max)
{
	return {
		Clamp(val.x, min, max), Clamp(val.y, min, max),
	};
}

inline Vector2 Clamp(const Vector2& val, const Vector2& min, const Vector2& max)
{
	return {
		Clamp(val.x, min.x, max.x), Clamp(val.y, min.y, max.y),
	};
}
}
}
