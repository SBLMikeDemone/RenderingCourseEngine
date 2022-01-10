#pragma once

#include <SBLMath/Common.hpp>
#include <SBLMath/Vector3.hpp>

#include <cmath>
#include <type_traits>

namespace SBL
{
namespace Math
{
struct Vector4
{
	Vector4() = default;
	~Vector4() = default;
	Vector4(const Vector4& rhs) = default;
	Vector4(Vector4&& rhs) = default;
	Vector4& operator=(const Vector4& rhs) = default;
	Vector4& operator=(Vector4&& rhs) = default;
	Vector4(const Vector3& rhs, float _w);
	constexpr inline Vector4(float32 _x, float32 _y, float32 _z, float32 _w);

	// In C++, POD types cannot have member initializers
	union {
		struct
		{
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
		float32 values[4];
	};

	inline float32& operator[](std::size_t index);
	inline const float32& operator[](std::size_t index) const;

	static const Vector4 Zero;
	static const Vector4 One;
	static const Vector4 Up;
	static const Vector4 Down;
	static const Vector4 Left;
	static const Vector4 Right;
	static const Vector4 In;
	static const Vector4 Out;
	static const Vector4 Unit;
};

inline bool operator==(const Vector4& lhs, const Vector4& rhs);
inline bool operator!=(const Vector4& lhs, const Vector4& rhs);

inline Vector4 operator-(const Vector4& vec);

inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs);
inline Vector4 operator/(const Vector4& lhs, const Vector4& rhs);

inline Vector4& operator+=(Vector4& lhs, const Vector4& rhs);
inline Vector4& operator-=(Vector4& lhs, const Vector4& rhs);
inline Vector4& operator*=(Vector4& lhs, const Vector4& rhs);
inline Vector4& operator/=(Vector4& lhs, const Vector4& rhs);

inline Vector4 operator+(const Vector4& lhs, float32 rhs);
inline Vector4 operator-(const Vector4& lhs, float32 rhs);
inline Vector4 operator*(const Vector4& lhs, float32 rhs);
inline Vector4 operator/(const Vector4& lhs, float32 rhs);

inline Vector4 operator+(float32 lhs, const Vector4& rhs);
inline Vector4 operator-(float32 lhs, const Vector4& rhs);
inline Vector4 operator*(float32 lhs, const Vector4& rhs);
inline Vector4 operator/(float32 lhs, const Vector4& rhs);

inline Vector4& operator+=(Vector4& lhs, float32 rhs);
inline Vector4& operator-=(Vector4& lhs, float32 rhs);
inline Vector4& operator*=(Vector4& lhs, float32 rhs);
inline Vector4& operator/=(Vector4& lhs, float32 rhs);

inline float32 Sum(const Vector4& vec);
inline float32 LengthSquared(const Vector4& vec);
inline float32 Length(const Vector4& vec);
inline Vector4 Normalized(const Vector4& vec);
inline float32 DotProduct(const Vector4& lhs, const Vector4& rhs);
inline Radians AngleBetween(const Vector4& lhs, const Vector4& rhs);
inline Vector4 Reflection(const Vector4& direction, const Vector4& normal);
inline Vector4 Projection(const Vector4& source, const Vector4& target);
inline Vector4 Rejection(const Vector4& source, const Vector4& target);
inline Vector4 Lerp(const Vector4& source, const Vector4& target,
					float32 portion);
inline Vector4 Min(const Vector4& a, const Vector4& b);
inline Vector4 Max(const Vector4& a, const Vector4& b);
inline Vector4 Clamp(const Vector4& val, float32 min, float32 max);
inline Vector4 Clamp(const Vector4& val, const Vector4& min,
					 const Vector4& max);
}
}

#include <SBLMath/Vector4.inl>
