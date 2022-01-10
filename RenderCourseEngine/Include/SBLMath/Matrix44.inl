#pragma once

#include <SBLMath/Matrix44.hpp>

namespace SBL
{
namespace Math
{
inline Matrix44::Matrix44(float32 _00, float32 _01, float32 _02, float32 _03,
						  float32 _10, float32 _11, float32 _12, float32 _13,
						  float32 _20, float32 _21, float32 _22, float32 _23,
						  float32 _30, float32 _31, float32 _32, float32 _33)
	: m00(_00), m01(_01), m02(_02), m03(_03), m10(_10), m11(_11), m12(_12),
	  m13(_13), m20(_20), m21(_21), m22(_22), m23(_23), m30(_30), m31(_31),
	  m32(_32), m33(_33)
{
}

inline Vector4& Matrix44::operator[](std::size_t columnIndex)
{
	return rows[columnIndex];
}

inline const Vector4& Matrix44::operator[](std::size_t columnIndex) const
{
	return rows[columnIndex];
}

inline Vector4 Matrix44::Column(std::size_t columnIndex) const
{
	return Vector4{values[0 + columnIndex], values[4 + columnIndex],
				   values[8 + columnIndex], values[12 + columnIndex]};
}

inline Vector4 Matrix44::Row(std::size_t rowIndex) const
{
	return rows[rowIndex];
}

inline Matrix44 Matrix44::Translation(const Vector3& translation)
{
	return Matrix44(
		1, 0, 0, translation.x,
		0, 1, 0, translation.y,
		0, 0, 1, translation.z,
		0, 0, 0, 1);
}

inline Matrix44 Matrix44::Scale(const Vector3& scale)
{
	return Matrix44(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);
}

inline Matrix44 Matrix44::Scale(float scale)
{
	return Matrix44(
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1);
}

inline Matrix44 Matrix44::Rotation(const Quaternion& rotation)
{
	// See http://www.songho.ca/opengl/gl_quaternion.html
	auto normalizedRotation = Normalized(rotation);
	float x = normalizedRotation.x;
	float y = normalizedRotation.y;
	float z = normalizedRotation.z;
	float s = normalizedRotation.w;
	return Matrix44(
		1 - 2 * y * y - 2 * z * z,	2 * x * y - 2 * s * z,		2 * x * z + 2 * s * y,		0,
		2 * x * y + 2 * s * z,		1 - 2 * x * x - 2 * z * z,	2 * y * z - 2 * s * x,		0,
		2 * x * z - 2 * s * y,		2 * y * z + 2 * s * x,		1 - 2 * x * x - 2 * y * y,	0,
		0,							0,							0,							1
	);
}

inline Matrix44 Matrix44::TranslationScale(const Vector3& translation,
										   const Vector3& scale)
{
	return Matrix44(
		scale.x, 0, 0, translation.x,
		0, scale.y, 0, translation.y,
		0, 0, scale.z, translation.z,
		0, 0, 0, 1);
}

inline Matrix44 Matrix44::TranslationScale(const Vector3& translation,
										   float uniformScale)
{
	return Matrix44(
		uniformScale, 0, 0, translation.x,
		0, uniformScale, 0, translation.y,
		0, 0, uniformScale, translation.z,
		0, 0, 0, 1);
}

inline Matrix44 Matrix44::TranslationRotation(const Vector3& translation,
											  const Quaternion& rotation)
{
	return Translation(translation) * Rotation(rotation);
}

inline Matrix44 Matrix44::TranslationRotationScale(const Vector3& translation,
												   const Quaternion& rotation,
												   const Vector3& scale)
{
	return Translation(translation) * Rotation(rotation) * Scale(scale);
}

inline Matrix44 Matrix44::TranslationRotationScale(const Vector3& translation,
												   const Quaternion& rotation,
												   float scale)
{
	return  Translation(translation) * Rotation(rotation) * Scale(scale);
}

inline bool operator==(const Matrix44& lhs, const Matrix44& rhs)
{
	return Equals(lhs.m00, rhs.m00) && Equals(lhs.m01, rhs.m01) &&
		   Equals(lhs.m02, rhs.m02) && Equals(lhs.m10, rhs.m10) &&
		   Equals(lhs.m11, rhs.m11) && Equals(lhs.m12, rhs.m12) &&
		   Equals(lhs.m20, rhs.m20) && Equals(lhs.m21, rhs.m21) &&
		   Equals(lhs.m22, rhs.m22);
}

inline bool operator!=(const Matrix44& lhs, const Matrix44& rhs)
{
	return !(lhs == rhs);
}

inline Matrix44 operator+(const Matrix44& lhs, float32 rhs)
{
	return Matrix44{lhs.m00 + rhs, lhs.m01 + rhs, lhs.m02 + rhs, lhs.m03 + rhs,
					lhs.m10 + rhs, lhs.m11 + rhs, lhs.m12 + rhs, lhs.m13 + rhs,
					lhs.m20 + rhs, lhs.m21 + rhs, lhs.m22 + rhs, lhs.m23 + rhs,
					lhs.m30 + rhs, lhs.m31 + rhs, lhs.m32 + rhs, lhs.m33 + rhs};
}

inline Matrix44 operator-(const Matrix44& lhs, float32 rhs)
{
	return Matrix44{lhs.m00 - rhs, lhs.m01 - rhs, lhs.m02 - rhs, lhs.m03 - rhs,
					lhs.m10 - rhs, lhs.m11 - rhs, lhs.m12 - rhs, lhs.m13 - rhs,
					lhs.m20 - rhs, lhs.m21 - rhs, lhs.m22 - rhs, lhs.m23 - rhs,
					lhs.m30 - rhs, lhs.m31 - rhs, lhs.m32 - rhs, lhs.m33 - rhs};
}

inline Matrix44 operator*(const Matrix44& lhs, float32 rhs)
{
	return Matrix44{lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs, lhs.m03 * rhs,
					lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs, lhs.m13 * rhs,
					lhs.m20 * rhs, lhs.m21 * rhs, lhs.m22 * rhs, lhs.m23 * rhs,
					lhs.m30 * rhs, lhs.m31 * rhs, lhs.m32 * rhs, lhs.m33 * rhs};
}

inline Matrix44 operator/(const Matrix44& lhs, float32 rhs)
{
	return Matrix44{lhs.m00 / rhs, lhs.m01 / rhs, lhs.m02 / rhs, lhs.m03 / rhs,
					lhs.m10 / rhs, lhs.m11 / rhs, lhs.m12 / rhs, lhs.m13 / rhs,
					lhs.m20 / rhs, lhs.m21 / rhs, lhs.m22 / rhs, lhs.m23 / rhs,
					lhs.m30 / rhs, lhs.m31 / rhs, lhs.m32 / rhs, lhs.m33 / rhs};
}

inline Matrix44 operator*(const Matrix44& lhs, const Matrix44& rhs)
{
	Matrix44 res;
	
	res.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20; // rhs.row0 . lhs.col0
	res.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21; // rhs.row0 . lhs.col1
	res.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22; // rhs.row0 . lhs.col2
	res.m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23; // rhs.row0 . lhs.col3

	res.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20; // rhs.row1 . lhs.col0
	res.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21; // rhs.row1 . lhs.col1
	res.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22; // rhs.row1 . lhs.col2
	res.m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23; // rhs.row1 . lhs.col3

	res.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20; // rhs.row2 . lhs.col0
	res.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21; // rhs.row2 . lhs.col1
	res.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22; // rhs.row2 . lhs.col2
	res.m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23; // rhs.row3 . lhs.col3

	return res;
}

inline Vector4 operator*(const Matrix44& lhs, const Vector4& rhs)
{
	return Vector4(
		lhs.m00 * rhs.x + lhs.m01 * rhs.y + lhs.m02 * rhs.z + lhs.m03 * rhs.w,
		lhs.m10 * rhs.x + lhs.m11 * rhs.y + lhs.m12 * rhs.z + lhs.m13 * rhs.w,
		lhs.m20 * rhs.x + lhs.m21 * rhs.y + lhs.m22 * rhs.z + lhs.m23 * rhs.w,
		lhs.m30 * rhs.x + lhs.m31 * rhs.y + lhs.m32 * rhs.z + lhs.m33 * rhs.w
	);
}

inline Vector3 operator*(const Matrix44& lhs, const Vector3& rhs)
{
	Vector4 homo_rhs = Vector4(
		rhs.x,
		rhs.y,
		rhs.z,
		1
	);
	Vector4 homo_res = lhs * homo_rhs;
	Vector3 res = Vector3(
		homo_res.x / homo_res.w,
		homo_res.y / homo_res.w,
		homo_res.z / homo_res.w
		);
	return res;
}

inline float32 SignOfElement(const Matrix44& mat, std::size_t column,
							 std::size_t row)
{
	return ((column + row) % 2) ? -1.0f : 1.0f;
}

inline float32 CofactorOfElement(const Matrix44& mat, std::size_t column,
								 std::size_t row)
{
	std::size_t resultColumn = 0;
	std::size_t resultRow = 0;

	Matrix33 mat33;

	for (std::size_t x = 0; x < 4; ++x)
	{
		if (x != column)
		{
			resultRow = 0;
			for (std::size_t y = 0; y < 4; ++y)
			{
				if (y != row)
				{
					mat33[resultRow][resultColumn] = mat[y][x];
					++resultRow;
				}
			}
			++resultColumn;
		}
	}

	return SignOfElement(mat, column, row) * Determinant(mat33);
}

inline Matrix44 Cofactor(const Matrix44& mat)
{
	Matrix44 result;
	for (std::size_t x = 0; x < 4; ++x)
	{
		for (std::size_t y = 0; y < 4; ++y)
		{
			result[x][y] = CofactorOfElement(mat, y, x);
		}
	}
	return result;
}

inline Matrix44 Transpose(const Matrix44& mat)
{
	return Matrix44(
		mat.m00, mat.m10, mat.m20, mat.m30,
		mat.m01, mat.m11, mat.m21, mat.m31,
		mat.m02, mat.m12, mat.m22, mat.m32,
		mat.m03, mat.m13, mat.m23, mat.m33
	);
}

inline Matrix44 Adjoint(const Matrix44& mat)
{
	return Transpose(Cofactor(mat));
}

inline Matrix44 Inverse(const Matrix44& mat)
{
	Matrix44 invMat;
	float* __restrict inv = invMat.values;
	const float* __restrict m = mat.values;

	inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
			 m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] +
			 m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] +
			 m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
			 m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] +
			  m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] +
			  m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] +
			 m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] +
			 m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
			 m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
			 m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
			  m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
			 m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
			 m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
			  m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
			  m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
			 m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
			 m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
			  m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
			  m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

	float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	det = 1.0f / det;

	for (int i = 0; i < 16; ++i)
	{
		inv[i] = inv[i] * det;
	}

	return invMat;
}

inline float32 Determinant(const Matrix44& mat)
{
	// TODO: IMPLEMENT ME <not a part of skybox U graphics assignment>
	return 0.0f;
}
}
}