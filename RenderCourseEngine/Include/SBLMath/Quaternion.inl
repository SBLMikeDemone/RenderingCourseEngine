#pragma once

#include <SBLMath/Quaternion.hpp>

namespace SBL
{
namespace Math
{
inline Quaternion::Quaternion(float32 _x, float32 _y, float32 _z, float32 _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}

inline Quaternion::Quaternion(const Vector4& rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
}

inline Quaternion::operator Vector4() const
{
	return Vector4{x, y, z, w};
}

inline Quaternion Quaternion::Euler(Radians x, Radians y, Radians z)
{
	// TODO: Need to revisit this; I don't think it's working as intended. <not for SBL university course>
	float32 cosX = Cos(x);
	float32 cosY = Cos(y);
	float32 cosZ = Cos(z);
	float32 sinX = Sin(x);
	float32 sinY = Sin(y);
	float32 sinZ = Sin(z);
	return Quaternion{(cosX * cosY * sinZ) - (sinX * sinY * cosZ),
					  (sinX * cosY * cosZ) - (cosX * sinY * sinZ),
					  (cosX * sinY * cosZ) - (sinX * cosY * sinZ),
					  (cosX * cosY * cosZ) - (sinX * sinY * sinZ)};
}

inline Quaternion Quaternion::EulerYXZ(Radians x, Radians y, Radians z)
{
	float32 cosX = Cos(x * 0.5f);
	float32 cosY = Cos(y * 0.5f);
	float32 cosZ = Cos(z * 0.5f);
	float32 sinX = Sin(x * 0.5f);
	float32 sinY = Sin(y * 0.5f);
	float32 sinZ = Sin(z * 0.5f);
	return Quaternion{(cosZ * sinX * cosY) + (sinZ * cosX * sinY),
					  (cosZ * cosX * sinY) - (sinZ * sinX * cosY),
					  -(cosZ * sinX * sinY) + (sinZ * cosX * cosY),
					  (cosZ * cosX * cosY) + (sinZ * sinX * sinY)};
}

inline Quaternion Quaternion::AxisAngle(const Vector3& axis, Radians angle)
{
	return Quaternion{axis.x * Sin(angle / 2.0f), axis.y * Sin(angle / 2.0f),
					  axis.z * Sin(angle / 2.0f), Cos(angle / 2.0f)};
}

inline bool operator==(const Quaternion& lhs, const Quaternion& rhs)
{
	return Equals(lhs.x, rhs.x) && Equals(lhs.y, rhs.y) &&
		   Equals(lhs.z, rhs.z) && Equals(lhs.w, rhs.w);
}

inline bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
{
	return !(lhs == rhs);
}

inline Quaternion operator*(const Quaternion& q2, const Quaternion& q1)
{
	return {q1.w * q2.x + q1.x * q2.w - q1.y * q2.z + q1.z * q2.y,
			q1.w * q2.y + q1.x * q2.z + q1.y * q2.w - q1.z * q2.x,
			q1.w * q2.z - q1.x * q2.y + q1.y * q2.x + q1.z * q2.w,
			q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z};
}

inline Quaternion Normalized(const Quaternion& q)
{
	const float32 length = Length(q);
	return {q.x / length, q.y / length, q.z / length, q.w / length};
}

inline Quaternion Slerp(const Quaternion& source, const Quaternion& target,
						float32 portion)
{
	Vector4 sourceV = Normalized(source);
	Vector4 targetV = Normalized(target);
	float32 dot = DotProduct(sourceV, targetV);
	if (dot > 0.999f)
	{
		return (Quaternion)Normalized(Lerp(sourceV, targetV, portion));
	}

	Radians targetAngle = ArcCos(dot) * portion;
	Vector4 orthonormal = Normalized(targetV - (sourceV * dot));
	return (Quaternion)((sourceV * Cos(targetAngle)) +
						(orthonormal * Sin(targetAngle)));
}

inline Quaternion Nlerp(const Quaternion& source, const Quaternion& target,
						float32 portion)
{
	return (Quaternion)Normalized(Lerp(source, target, portion));
}
}
}