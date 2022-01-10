#pragma once

#include <SBLMath/Vector3.hpp>

namespace SBL
{
namespace Math
{
inline Vector3::Vector3(float32 _x, float32 _y, float32 _z)
	: x(_x), y(_y), z(_z)
{
}

inline float32& Vector3::operator[](std::size_t index)
{
	return values[index];
}

inline const float32& Vector3::operator[](std::size_t index) const
{
	return values[index];
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs)
{
	return Equals(lhs.x, rhs.x) && Equals(lhs.y, rhs.y) && Equals(lhs.z, rhs.z);
}

inline bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
	return !(lhs == rhs);
}

inline Vector3 operator-(const Vector3& vec)
{
	return Vector3{-vec.x, -vec.y, -vec.z};
}

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}

inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
}

inline Vector3& operator+=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

inline Vector3& operator-=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

inline Vector3& operator*=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	return lhs;
}

inline Vector3& operator/=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	return lhs;
}

inline Vector3 operator+(const Vector3& lhs, float32 rhs)
{
	return Vector3{lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};
}

inline Vector3 operator-(const Vector3& lhs, float32 rhs)
{
	return Vector3{lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};
}

inline Vector3 operator*(const Vector3& lhs, float32 rhs)
{
	return Vector3{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

inline Vector3 operator/(const Vector3& lhs, float32 rhs)
{
	return Vector3{lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

inline Vector3 operator+(float32 lhs, const Vector3& rhs)
{
	return Vector3{lhs + rhs.x, lhs + rhs.y, lhs + rhs.z};
}

inline Vector3 operator-(float32 lhs, const Vector3& rhs)
{
	return Vector3{lhs - rhs.x, lhs - rhs.y, lhs - rhs.z};
}

inline Vector3 operator*(float32 lhs, const Vector3& rhs)
{
	return Vector3{lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
}

inline Vector3 operator/(float32 lhs, const Vector3& rhs)
{
	return Vector3{lhs / rhs.x, lhs / rhs.y, lhs / rhs.z};
}

inline Vector3& operator+=(Vector3& lhs, float32 rhs)
{
	lhs.x += rhs;
	lhs.y += rhs;
	lhs.z += rhs;
	return lhs;
}

inline Vector3& operator-=(Vector3& lhs, float32 rhs)
{
	lhs.x -= rhs;
	lhs.y -= rhs;
	lhs.z -= rhs;
	return lhs;
}

inline Vector3& operator*=(Vector3& lhs, float32 rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

inline Vector3& operator/=(Vector3& lhs, float32 rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}

inline float32 Sum(const Vector3& vec)
{
	return vec.x + vec.y + vec.z;
}

inline float32 LengthSquared(const Vector3& vec)
{
	return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}

inline float32 Length(const Vector3& vec)
{
	return sqrtf(LengthSquared(vec));
}

inline Vector3 Normalized(const Vector3& vec)
{
	return vec / Length(vec);
}

inline float32 DotProduct(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline Vector3 CrossProduct(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	);
}

inline Radians AngleBetween(const Vector3& lhs, const Vector3& rhs)
{
	return acosf(DotProduct(lhs, rhs) / (Length(lhs) * Length(rhs)));
}

inline Vector3 Reflection(const Vector3& direction, const Vector3& normal)
{
 	/*d    n    r      
           ^	
	   \   |   /
		\  |  /
		 \ | / 
		   |	
      
	  (d is going down & right, r is going up & right, n is going up)

	  d . n = r . n 
	  
	  d = A + B where B is the projection of d onto n and A is the remaining component
	  r = A - B

	  B = d.n * n

	  d = A + B = A + d.n * n
	  A = d - d.n * n

	  r = A - B = d - d.n * n - d.n * n = d - 2 * d.n
	*/

	return direction - 2 * DotProduct(direction, normal) * normal;
}

inline Vector3 Projection(const Vector3& source, const Vector3& target)
{
	return DotProduct(source, target) * target;
}

inline Vector3 Rejection(const Vector3& source, const Vector3& target)
{
	return source - Projection(source, target);
}

inline Vector3 Lerp(const Vector3& source, const Vector3& target,
					float32 portion)
{
	return source + ((target - source) * portion);
}

inline Vector3 Min(const Vector3& a, const Vector3& b)
{
	return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

inline Vector3 Max(const Vector3& a, const Vector3& b)
{
	return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

inline Vector3 Clamp(const Vector3& val, float32 min, float32 max)
{
	return {
		Clamp(val.x, min, max), Clamp(val.y, min, max), Clamp(val.z, min, max),
	};
}

inline Vector3 Clamp(const Vector3& val, const Vector3& min, const Vector3& max)
{
	return {
		Clamp(val.x, min.x, max.x), Clamp(val.y, min.y, max.y),
		Clamp(val.z, min.z, max.z),
	};
}
}
}