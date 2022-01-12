#pragma once

#include <SBLMath/Common.hpp>
#include <SBLMath/Matrix33.hpp>
#include <SBLMath/Quaternion.hpp>
#include <SBLMath/Vector2.hpp>
#include <SBLMath/Vector3.hpp>
#include <SBLMath/Vector4.hpp>

#include <cmath>
#include <type_traits>

namespace SBL
{
namespace Math
{
struct Matrix44
{
	Matrix44() = default;
	~Matrix44() = default;
	Matrix44(const Matrix44& rhs) = default;
	Matrix44(Matrix44&& rhs) = default;
	Matrix44& operator=(const Matrix44& rhs) = default;
	Matrix44& operator=(Matrix44&& rhs) = default;
	inline Matrix44(float32 _00, float32 _01, float32 _02, float32 _03,
					float32 _10, float32 _11, float32 _12, float32 _13,
					float32 _20, float32 _21, float32 _22, float32 _23,
					float32 _30, float32 _31, float32 _32, float32 _33);
	union {
		struct
		{
			float32 m00;
			float32 m01;
			float32 m02;
			float32 m03;
			float32 m10;
			float32 m11;
			float32 m12;
			float32 m13;
			float32 m20;
			float32 m21;
			float32 m22;
			float32 m23;
			float32 m30;
			float32 m31;
			float32 m32;
			float32 m33;
		};
		Vector4 rows[4];
		float32 values[16];
	};

	inline Vector4& operator[](std::size_t rowIndex);
	inline const Vector4& operator[](std::size_t rowIndex) const;
	inline Vector4 Column(std::size_t columnIndex) const;
	inline Vector4 Row(std::size_t rowIndex) const;

	static const Matrix44 Identity;
	static const Matrix44 Zero;

	static inline Matrix44 Translation(const Vector3& translation);
	static inline Matrix44 Scale(const Vector3& scale);
	static inline Matrix44 Scale(float scale);
	static inline Matrix44 Rotation(const Quaternion& rotation);
	static inline Matrix44 TranslationScale(const Vector3& translation,
											const Vector3& scale);
	static inline Matrix44 TranslationScale(const Vector3& translation,
											float uniformScale);
	static inline Matrix44 TranslationRotation(const Vector3& translation,
											   const Quaternion& rotation);
	static inline Matrix44 TranslationRotationScale(const Vector3& translation,
													const Quaternion& rotation,
													const Vector3& scale);
	static inline Matrix44 TranslationRotationScale(const Vector3& translation,
													const Quaternion& rotation,
													float scale);
};

inline bool operator==(const Matrix44& lhs, const Matrix44& rhs);
inline bool operator!=(const Matrix44& lhs, const Matrix44& rhs);

inline Matrix44 operator+(const Matrix44& lhs, float32 rhs);
inline Matrix44 operator-(const Matrix44& lhs, float32 rhs);
inline Matrix44 operator*(const Matrix44& lhs, float32 rhs);
inline Matrix44 operator/(const Matrix44& lhs, float32 rhs);

inline Matrix44 operator*(const Matrix44& lhs, const Matrix44& rhs);
inline Vector4 operator*(const Matrix44& lhs, const Vector4& rhs);
inline Vector3 operator*(const Matrix44& lhs, const Vector3& rhs);

inline float32 SignOfElement(const Matrix44& mat, std::size_t column,
							 std::size_t row);
inline float32 CofactorOfElement(const Matrix44& mat, std::size_t column,
								 std::size_t row);
inline Matrix44 Cofactor(const Matrix44& mat);
inline Matrix44 Transpose(const Matrix44& mat);
inline Matrix44 Adjoint(const Matrix44& mat);
inline Matrix44 Inverse(const Matrix44& mat);
inline float32 Determinant(const Matrix44& mat);
inline Matrix44 InverseScale(const Vector3 vec);
}
}

#include <SBLMath/Matrix44.inl>
