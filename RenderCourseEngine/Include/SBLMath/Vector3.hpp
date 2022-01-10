#pragma once

#include <SBLMath/Common.hpp>

#include <cmath>
#include <type_traits>

namespace SBL
{
namespace Math
{
struct Vector3
{
	Vector3() = default;
	~Vector3() = default;
	Vector3(const Vector3& rhs) = default;
	Vector3(Vector3&& rhs) = default;
	Vector3& operator=(const Vector3& rhs) = default;
	Vector3& operator=(Vector3&& rhs) = default;
	inline Vector3(float32 _x, float32 _y, float32 _z);

	// In C++, POD types cannot have member initializers
	union {
		struct
		{
			float32 x;
			float32 y;
			float32 z;
		};
		float32 values[3];
	};

	inline float32& operator[](std::size_t index);
	inline const float32& operator[](std::size_t index) const;

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Left;
	static const Vector3 Right;
	static const Vector3 In;
	static const Vector3 Out;
	static const Vector3 Unit;
};

inline bool operator==(const Vector3& lhs, const Vector3& rhs);
inline bool operator!=(const Vector3& lhs, const Vector3& rhs);

inline Vector3 operator-(const Vector3& vec);

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs);
inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs);

inline Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
inline Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
inline Vector3& operator*=(Vector3& lhs, const Vector3& rhs);
inline Vector3& operator/=(Vector3& lhs, const Vector3& rhs);

inline Vector3 operator+(const Vector3& lhs, float32 rhs);
inline Vector3 operator-(const Vector3& lhs, float32 rhs);
inline Vector3 operator*(const Vector3& lhs, float32 rhs);
inline Vector3 operator/(const Vector3& lhs, float32 rhs);

inline Vector3 operator+(float32 lhs, const Vector3& rhs);
inline Vector3 operator-(float32 lhs, const Vector3& rhs);
inline Vector3 operator*(float32 lhs, const Vector3& rhs);
inline Vector3 operator/(float32 lhs, const Vector3& rhs);

inline Vector3& operator+=(Vector3& lhs, float32 rhs);
inline Vector3& operator-=(Vector3& lhs, float32 rhs);
inline Vector3& operator*=(Vector3& lhs, float32 rhs);
inline Vector3& operator/=(Vector3& lhs, float32 rhs);

inline float32 Sum(const Vector3& vec);
inline float32 LengthSquared(const Vector3& vec);
inline float32 Length(const Vector3& vec);
inline Vector3 Normalized(const Vector3& vec);
inline float32 DotProduct(const Vector3& lhs, const Vector3& rhs);
inline Vector3 CrossProduct(const Vector3& lhs, const Vector3& rhs);
inline Radians AngleBetween(const Vector3& lhs, const Vector3& rhs);
inline Vector3 Reflection(const Vector3& direction, const Vector3& normal);
inline Vector3 Projection(const Vector3& source, const Vector3& target);
inline Vector3 Rejection(const Vector3& source, const Vector3& target);
inline Vector3 Lerp(const Vector3& source, const Vector3& target,
					float32 portion);
inline Vector3 Min(const Vector3& a, const Vector3& b);
inline Vector3 Max(const Vector3& a, const Vector3& b);
inline Vector3 Clamp(const Vector3& val, float32 min, float32 max);
inline Vector3 Clamp(const Vector3& val, const Vector3& min,
					 const Vector3& max);
}
}

#include <SBLMath/Vector3.inl>
