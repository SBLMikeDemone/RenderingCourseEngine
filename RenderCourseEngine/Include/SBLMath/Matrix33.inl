#pragma once

#include <SBLMath/Matrix33.hpp>

namespace SBL
{
namespace Math
{
inline Matrix33::Matrix33(float32 _00, float32 _01, float32 _02, float32 _10,
						  float32 _11, float32 _12, float32 _20, float32 _21,
						  float32 _22)
	: m00(_00), m01(_01), m02(_02), m10(_10), m11(_11), m12(_12), m20(_20),
	  m21(_21), m22(_22)
{
}

inline Vector3& Matrix33::operator[](std::size_t columnIndex)
{
	return rows[columnIndex];
}

inline const Vector3& Matrix33::operator[](std::size_t columnIndex) const
{
	return rows[columnIndex];
}

inline Vector3 Matrix33::Column(std::size_t columnIndex) const
{
	return Vector3{values[0 + columnIndex], values[3 + columnIndex],
				   values[6 + columnIndex]};
}

inline Vector3 Matrix33::Row(std::size_t rowIndex) const
{
	return rows[rowIndex];
}

Matrix33 Matrix33::Translation(const Vector2& translation)
{
	return Matrix33(
		1, 0, translation.x,
		0, 1, translation.y,
		0, 0, 1
	);
}

Matrix33 Matrix33::Scale(const Vector2& scale)
{
	return Matrix33(
		scale.x, 0, 0,
		0, scale.y, 0,
		0, 0, 1);
}

Matrix33 Matrix33::Rotation(Radians rotation)
{
	// Assuming rotation around z-axis
	return Matrix33(
		cos(rotation), -sin(rotation), 0,
		sin(rotation), cos(rotation), 0,
		0, 0, 1
	);
}

inline Matrix33 Matrix33::Rotation(const Quaternion& rotation)
{
	auto normalizedRotation = Normalized(rotation);
	float x = normalizedRotation.x;
	float y = normalizedRotation.y;
	float z = normalizedRotation.z;
	float s = normalizedRotation.w;
	return Matrix33(
		1-2*y*y-2*z*z, 2*x*y-2*s*z, 2*x*z+2*s*y,
		2*x*y+2*s*z, 1-2*x*x-2*z*z, 2*y*z-2*s*x,
		2*x*z-2*s*y, 2*y*z+2*s*x, 1-2*x*x-2*y*y
	);
}

inline Matrix33 Matrix33::Rotation(const Vector3& right, const Vector3& up, const Vector3& forward)
{
	return Matrix33(
		right.x,	right.y,	right.z,
		up.x,		up.y,		up.z,	
		forward.x,	forward.y,	forward.z
	);
}

inline Matrix33 Matrix33::Rotation(const Vector3& forward, const Vector3& up)
{
	return Rotation(CrossProduct(forward, up), up, forward);
}

inline bool operator==(const Matrix33& lhs, const Matrix33& rhs)
{
	return Equals(lhs.m00, rhs.m00) && Equals(lhs.m01, rhs.m01) &&
		   Equals(lhs.m02, rhs.m02) && Equals(lhs.m10, rhs.m10) &&
		   Equals(lhs.m11, rhs.m11) && Equals(lhs.m12, rhs.m12) &&
		   Equals(lhs.m20, rhs.m20) && Equals(lhs.m21, rhs.m21) &&
		   Equals(lhs.m22, rhs.m22);
}

inline bool operator!=(const Matrix33& lhs, const Matrix33& rhs)
{
	return !(lhs == rhs);
}

inline Matrix33 operator+(const Matrix33& lhs, float32 rhs)
{
	return Matrix33{
		lhs.m00 + rhs, lhs.m01 + rhs, lhs.m02 + rhs,
		lhs.m10 + rhs, lhs.m11 + rhs, lhs.m12 + rhs,
		lhs.m20 + rhs, lhs.m21 + rhs, lhs.m22 + rhs,
	};
}

inline Matrix33 operator-(const Matrix33& lhs, float32 rhs)
{
	return Matrix33{
		lhs.m00 - rhs, lhs.m01 - rhs, lhs.m02 - rhs,
		lhs.m10 - rhs, lhs.m11 - rhs, lhs.m12 - rhs,
		lhs.m20 - rhs, lhs.m21 - rhs, lhs.m22 - rhs,
	};
}

inline Matrix33 operator*(const Matrix33& lhs, float32 rhs)
{
	return Matrix33{
		lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs,
		lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs,
		lhs.m20 * rhs, lhs.m21 * rhs, lhs.m22 * rhs,
	};
}

inline Matrix33 operator/(const Matrix33& lhs, float32 rhs)
{
	return Matrix33{
		lhs.m00 / rhs, lhs.m01 / rhs, lhs.m02 / rhs,
		lhs.m10 / rhs, lhs.m11 / rhs, lhs.m12 / rhs,
		lhs.m20 / rhs, lhs.m21 / rhs, lhs.m22 / rhs,
	};
}

inline Matrix33 operator*(const Matrix33& lhs, const Matrix33& rhs)
{
	return Matrix33{DotProduct(lhs.Row(0), rhs.Column(0)),
					DotProduct(lhs.Row(0), rhs.Column(1)),
					DotProduct(lhs.Row(0), rhs.Column(2)),
					DotProduct(lhs.Row(1), rhs.Column(0)),
					DotProduct(lhs.Row(1), rhs.Column(1)),
					DotProduct(lhs.Row(1), rhs.Column(2)),
					DotProduct(lhs.Row(2), rhs.Column(0)),
					DotProduct(lhs.Row(2), rhs.Column(1)),
					DotProduct(lhs.Row(2), rhs.Column(2))};
}

inline Vector3 operator*(const Matrix33& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.m00 * rhs.x + lhs.m01 * rhs.y + lhs.m02 * rhs.z,
		lhs.m10 * rhs.x + lhs.m11 * rhs.y + lhs.m12 * rhs.z,
		lhs.m20 * rhs.x + lhs.m21 * rhs.y + lhs.m22 * rhs.z
	);
}

inline Vector2 operator*(const Matrix33& lhs, const Vector2& rhs)
{
	Vector3 homogeneous_rhs = Vector3(
		rhs.x, 
		rhs.y, 
		1);
	Vector3 homogeneous_result = lhs * homogeneous_rhs;
	Vector2 result = Vector2(
		homogeneous_result.x / homogeneous_result.z, 
		homogeneous_result.y / homogeneous_result.z);
	return result;
}

inline float32 SignOfElement(const Matrix33& mat, std::size_t column,
							 std::size_t row)
{
	return ((column + row) % 2) ? -1.0f : 1.0f;
}

inline float32 CofactorOfElement(const Matrix33& mat, std::size_t column,
								 std::size_t row)
{
	std::size_t resultColumn = 0;
	std::size_t resultRow = 0;

	Matrix22 mat22;

	for (std::size_t x = 0; x < 3; ++x)
	{
		if (x != column)
		{
			resultRow = 0;
			for (std::size_t y = 0; y < 3; ++y)
			{
				if (y != row)
				{
					mat22[resultRow][resultColumn] = mat[y][x];
					++resultRow;
				}
			}
			++resultColumn;
		}
	}

	return SignOfElement(mat, column, row) * Determinant(mat22);
}

inline Matrix33 Cofactor(const Matrix33& mat)
{
	Matrix33 result;
	for (std::size_t x = 0; x < 3; ++x)
	{
		for (std::size_t y = 0; y < 3; ++y)
		{
			result[x][y] = CofactorOfElement(mat, y, x);
		}
	}
	return result;
}

inline Matrix33 Transpose(const Matrix33& mat)
{
	return Matrix33(
		mat.m00, mat.m10, mat.m20,
		mat.m01, mat.m11, mat.m21,
		mat.m02, mat.m12, mat.m22
		);
}

inline Matrix33 Adjoint(const Matrix33& mat)
{
	return Transpose(Cofactor(mat));
}

inline Matrix33 Inverse(const Matrix33& mat)
{
	return Adjoint(mat) / Determinant(mat);
}

inline float32 Determinant(const Matrix33& mat)
{
	return ((mat.m00 * ((mat.m11 * mat.m22) - (mat.m12 * mat.m21))) -
			(mat.m01 * ((mat.m10 * mat.m22) - (mat.m12 * mat.m20))) +
			(mat.m02 * ((mat.m10 * mat.m21) - (mat.m11 * mat.m20))));
}

inline Quaternion ToQuaternion(const Matrix33& mat)
{
  Quaternion quat;
  const float tr = mat.m00 + mat.m11 + mat.m22;
  if (tr > 0.0f)
  {
    float s = sqrt(tr + 1.0f) * 2.0f;
    quat = 
    {
      (mat.m12 - mat.m21) / s,
      (mat.m20 - mat.m02) / s,
      (mat.m01 - mat.m10) / s,
      s * 0.25f
    };
  }
  else if ((mat.m00 > mat.m11) && (mat.m00 > mat.m22))
  {
    float s = sqrt(1.0f + mat.m00 - mat.m11 - mat.m22) * 2.0f;
    quat =
    {
      s * 0.25f,
      (mat.m10 + mat.m01) / s,
      (mat.m20 + mat.m02) / s,
      (mat.m12 - mat.m21) / s
    };
  }
  else if (mat.m11 > mat.m22)
  {
    float s = sqrt(1.0f + mat.m11 - mat.m00 - mat.m22) * 2.0f;
    quat =
    {
      (mat.m10 + mat.m01) / s,
      s * 0.25f,
      (mat.m21 + mat.m12) / s,
      (mat.m20 - mat.m02) / s
    };
  }
  else
  {
    float s = sqrt(1.0f + mat.m22 - mat.m00 - mat.m11) * 2.0f;
    quat =
    {
      (mat.m20 + mat.m02) / s,
      (mat.m21 + mat.m12) / s,
      s * 0.25f,
      (mat.m01 - mat.m10) / s
    };
  }
  

  return quat;
 
}
}
}