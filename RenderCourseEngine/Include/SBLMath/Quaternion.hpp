#pragma once

#include <SBLMath/Common.hpp>
#include <SBLMath/Vector3.hpp>
#include <SBLMath/Vector4.hpp>

namespace SBL
{
namespace Math
{
struct Quaternion
{
	Quaternion() = default;
	~Quaternion() = default;
	Quaternion(const Quaternion& rhs) = default;
	Quaternion(Quaternion&& rhs) = default;
	Quaternion& operator=(const Quaternion& rhs) = default;
	Quaternion& operator=(Quaternion&& rhs) = default;
	inline Quaternion(float32 _x, float32 _y, float32 _z, float32 _w);
	explicit inline Quaternion(const Vector4& rhs);
	inline operator Vector4() const;

	float32 x;
	float32 y;
	float32 z;
	float32 w;

	static const Quaternion Zero;
	static const Quaternion Identity;

	static inline Quaternion Euler(Radians x, Radians y, Radians z);
	static inline Quaternion EulerYXZ(Radians x, Radians y, Radians z);
	static inline Quaternion AxisAngle(const Vector3& axis, Radians angle);
};

inline bool operator==(const Quaternion& lhs, const Quaternion& rhs);
inline bool operator!=(const Quaternion& lhs, const Quaternion& rhs);

inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

inline Quaternion Normalized(const Quaternion& vec);
inline Quaternion Slerp(const Quaternion& source, const Quaternion& target,
						float32 portion);
inline Quaternion Nlerp(const Quaternion& source, const Quaternion& target,
						float32 portion);
}
}

#include <SBLMath/Quaternion.inl>
