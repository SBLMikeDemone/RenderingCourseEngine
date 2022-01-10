#pragma once

#include <SBLMath/Vector4.hpp>

namespace SBL
{
namespace Math
{

inline Vector4::Vector4(const Vector3& rhs, float _w)
	: x(rhs.x), y(rhs.y), z(rhs.z), w(_w)
{
}

constexpr inline Vector4::Vector4(float32 _x, float32 _y, float32 _z,
								  float32 _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}

inline float32& Vector4::operator[](std::size_t index)
{
	return values[index];
}

inline const float32& Vector4::operator[](std::size_t index) const
{
	return values[index];
}

inline bool operator==(const Vector4& lhs, const Vector4& rhs)
{
	return Equals(lhs.x, rhs.x) && Equals(lhs.y, rhs.y) &&
		   Equals(lhs.z, rhs.z) && Equals(lhs.w, rhs.w);
}

inline bool operator!=(const Vector4& lhs, const Vector4& rhs)
{
	return !(lhs == rhs);
}

inline Vector4 operator-(const Vector4& vec)
{
	return Vector4{-vec.x, -vec.y, -vec.z, -vec.w};
}

inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
}

inline Vector4 operator/(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4{lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w};
}

inline Vector4& operator+=(Vector4& lhs, const Vector4& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
	return lhs;
}

inline Vector4& operator-=(Vector4& lhs, const Vector4& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
	return lhs;
}

inline Vector4& operator*=(Vector4& lhs, const Vector4& rhs)
{
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	lhs.w *= rhs.w;
	return lhs;
}

inline Vector4& operator/=(Vector4& lhs, const Vector4& rhs)
{
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	lhs.w /= rhs.w;
	return lhs;
}

inline Vector4 operator+(const Vector4& lhs, float32 rhs)
{
	return Vector4{lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs};
}

inline Vector4 operator-(const Vector4& lhs, float32 rhs)
{
	return Vector4{lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs};
}

inline Vector4 operator*(const Vector4& lhs, float32 rhs)
{
	return Vector4{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
}

inline Vector4 operator/(const Vector4& lhs, float32 rhs)
{
	return Vector4{lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs};
}

inline Vector4 operator+(float32 lhs, const Vector4& rhs)
{
	return Vector4{lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w};
}

inline Vector4 operator-(float32 lhs, const Vector4& rhs)
{
	return Vector4{lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w};
}

inline Vector4 operator*(float32 lhs, const Vector4& rhs)
{
	return Vector4{lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w};
}

inline Vector4 operator/(float32 lhs, const Vector4& rhs)
{
	return Vector4{lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w};
}

inline Vector4& operator+=(Vector4& lhs, float32 rhs)
{
	lhs.x += rhs;
	lhs.y += rhs;
	lhs.z += rhs;
	lhs.w += rhs;
	return lhs;
}

inline Vector4& operator-=(Vector4& lhs, float32 rhs)
{
	lhs.x -= rhs;
	lhs.y -= rhs;
	lhs.z -= rhs;
	lhs.w -= rhs;
	return lhs;
}

inline Vector4& operator*=(Vector4& lhs, float32 rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.z *= rhs;
	return lhs;
}

inline Vector4& operator/=(Vector4& lhs, float32 rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	lhs.w /= rhs;
	return lhs;
}

inline float32 Sum(const Vector4& vec)
{
	return vec.x + vec.y + vec.z + vec.w;
}

inline float32 LengthSquared(const Vector4& vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

inline float32 Length(const Vector4& vec)
{
	return sqrt(LengthSquared(vec));
}

inline Vector4 Normalized(const Vector4& vec)
{
	return vec / Length(vec);
}

inline float32 DotProduct(const Vector4& lhs, const Vector4& rhs)
{
	return lhs.x * rhs.x +
		lhs.y * rhs.y +
		lhs.z * rhs.z +
		lhs.w * rhs.w;
}

inline Radians AngleBetween(const Vector4& lhs, const Vector4& rhs)
{
	return acos(DotProduct(lhs, rhs) / (Length(lhs) * Length(rhs)));
}

inline Vector4 Reflection(const Vector4& direction, const Vector4& normal)
{
	return direction - 2 * DotProduct(direction, normal) * normal;
}

inline Vector4 Projection(const Vector4& source, const Vector4& target)
{
	return DotProduct(source, target) * target;
}

inline Vector4 Rejection(const Vector4& source, const Vector4& target)
{
	return source - Projection(source, target);
}

inline Vector4 Lerp(const Vector4& source, const Vector4& target,
					float32 portion)
{
	return source + ((target - source) * portion);
}

inline Vector4 Min(const Vector4& a, const Vector4& b)
{
	return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z),
			std::min(a.w, b.w)};
}

inline Vector4 Max(const Vector4& a, const Vector4& b)
{
	return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z),
			std::max(a.w, b.w)};
}

inline Vector4 Clamp(const Vector4& val, float32 min, float32 max)
{
	return {
		Clamp(val.x, min, max), Clamp(val.y, min, max), Clamp(val.z, min, max),
		Clamp(val.w, min, max),
	};
}

inline Vector4 Clamp(const Vector4& val, const Vector4& min, const Vector4& max)
{
	return {
		Clamp(val.x, min.x, max.x), Clamp(val.y, min.y, max.y),
		Clamp(val.z, min.z, max.z), Clamp(val.w, min.w, max.w),
	};
}
}
}
