#pragma once

#include <SBLMath/Matrix22.hpp>

namespace SBL
{
namespace Math
{
inline Matrix22::Matrix22(float32 _00, float32 _01, float32 _10, float32 _11)
	: m00(_00), m01(_01), m10(_10), m11(_11)
{
}

inline Vector2& Matrix22::operator[](std::size_t rowIndex)
{
	return rows[rowIndex];
}

inline const Vector2& Matrix22::operator[](std::size_t rowIndex) const
{
	return rows[rowIndex];
}

inline Vector2 Matrix22::Column(std::size_t columnIndex) const
{
	return Vector2{values[0 + columnIndex], values[2 + columnIndex]};
}

inline Vector2 Matrix22::Row(std::size_t rowIndex) const
{
	return rows[rowIndex];
}

inline bool operator==(const Matrix22& lhs, const Matrix22& rhs)
{
	return Equals(lhs.m00, rhs.m00) && Equals(lhs.m01, rhs.m01) &&
		   Equals(lhs.m10, rhs.m10) && Equals(lhs.m11, rhs.m11);
}

inline bool operator!=(const Matrix22& lhs, const Matrix22& rhs)
{
	return !(lhs == rhs);
}

inline Matrix22 operator*(const Matrix22& lhs, const Matrix22& rhs)
{
	return Matrix22{
		DotProduct(lhs.Row(0), rhs.Column(0)),
		DotProduct(lhs.Row(0), rhs.Column(1)),
		DotProduct(lhs.Row(1), rhs.Column(0)),
		DotProduct(lhs.Row(1), rhs.Column(1)),
	};
}

inline Vector2 operator*(const Matrix22& lhs, const Vector2& rhs)
{
	return Vector2{DotProduct(lhs.Row(0), rhs), DotProduct(lhs.Row(1), rhs)};
}

inline float32 Determinant(const Matrix22& mat)
{
	return (mat.m00 * mat.m11) - (mat.m01 * mat.m10);
}
}
}