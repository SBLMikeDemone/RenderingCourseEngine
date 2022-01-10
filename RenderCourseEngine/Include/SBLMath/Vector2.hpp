#pragma once

#include <SBLMath/Common.hpp>

#include <cmath>
#include <type_traits>

namespace SBL
{
namespace Math
{
struct Vector2
{
	Vector2() = default;
	~Vector2() = default;
	Vector2(const Vector2& rhs) = default;
	Vector2(Vector2&& rhs) = default;
	Vector2& operator=(const Vector2& rhs) = default;
	Vector2& operator=(Vector2&& rhs) = default;
	inline Vector2(float32 _x, float32 _y);

	// In C++, POD types cannot have member initializers
	union {
		struct
		{
			float32 x;
			float32 y;
		};
		float32 values[2];
	};

	inline float32& operator[](std::size_t index);
	inline const float32& operator[](std::size_t index) const;

	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Left;
	static const Vector2 Right;
	static const Vector2 Unit;
};

inline bool operator==(const Vector2& lhs, const Vector2& rhs);
inline bool operator!=(const Vector2& lhs, const Vector2& rhs);

inline Vector2 operator-(const Vector2& vec);

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator/(const Vector2& lhs, const Vector2& rhs);

inline Vector2& operator+=(Vector2& lhs, const Vector2& rhs);
inline Vector2& operator-=(Vector2& lhs, const Vector2& rhs);
inline Vector2& operator*=(Vector2& lhs, const Vector2& rhs);
inline Vector2& operator/=(Vector2& lhs, const Vector2& rhs);

inline Vector2 operator+(const Vector2& lhs, float32 rhs);
inline Vector2 operator-(const Vector2& lhs, float32 rhs);
inline Vector2 operator*(const Vector2& lhs, float32 rhs);
inline Vector2 operator/(const Vector2& lhs, float32 rhs);

inline Vector2 operator+(float32 lhs, const Vector2& rhs);
inline Vector2 operator-(float32 lhs, const Vector2& rhs);
inline Vector2 operator*(float32 lhs, const Vector2& rhs);
inline Vector2 operator/(float32 lhs, const Vector2& rhs);

inline Vector2& operator+=(Vector2& lhs, float32 rhs);
inline Vector2& operator-=(Vector2& lhs, float32 rhs);
inline Vector2& operator*=(Vector2& lhs, float32 rhs);
inline Vector2& operator/=(Vector2& lhs, float32 rhs);

inline float32 Sum(const Vector2& vec);
inline float32 LengthSquared(const Vector2& vec);
inline float32 Length(const Vector2& vec);
inline Vector2 Normalized(const Vector2& vec);
inline float32 DotProduct(const Vector2& lhs, const Vector2& rhs);
inline Radians AngleBetween(const Vector2& lhs, const Vector2& rhs);
inline Vector2 Reflection(const Vector2& direction, const Vector2& normal);
inline Vector2 Projection(const Vector2& source, const Vector2& target);
inline Vector2 Rejection(const Vector2& source, const Vector2& target);
inline Vector2 Lerp(const Vector2& source, const Vector2& target,
					float32 portion);
inline Vector2 Min(const Vector2& a, const Vector2& b);
inline Vector2 Max(const Vector2& a, const Vector2& b);
inline Vector2 Clamp(const Vector2& val, float32 min, float32 max);
inline Vector2 Clamp(const Vector2& val, const Vector2& min,
					 const Vector2& max);
}
}

#include <SBLMath/Vector2.inl>
