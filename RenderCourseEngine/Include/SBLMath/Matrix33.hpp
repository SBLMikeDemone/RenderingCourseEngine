#pragma once

#include <SBLMath/Common.hpp>
#include <SBLMath/Matrix22.hpp>
#include <SBLMath/Quaternion.hpp>
#include <SBLMath/Vector2.hpp>
#include <SBLMath/Vector3.hpp>

#include <cmath>
#include <type_traits>

namespace SBL
{
namespace Math
{
struct Matrix33
{
	Matrix33() = default;
	~Matrix33() = default;
	Matrix33(const Matrix33& rhs) = default;
	Matrix33(Matrix33&& rhs) = default;
	Matrix33& operator=(const Matrix33& rhs) = default;
	Matrix33& operator=(Matrix33&& rhs) = default;
	inline Matrix33(float32 _00, float32 _01, float32 _02, float32 _10,
					float32 _11, float32 _12, float32 _20, float32 _21,
					float32 _22);
	union {
		struct
		{
			float32 m00;
			float32 m01;
			float32 m02;
			float32 m10;
			float32 m11;
			float32 m12;
			float32 m20;
			float32 m21;
			float32 m22;
		};
		Vector3 rows[3];
		float32 values[9];
	};

	inline Vector3& operator[](std::size_t rowIndex);
	inline const Vector3& operator[](std::size_t rowIndex) const;
	inline Vector3 Column(std::size_t columnIndex) const;
	inline Vector3 Row(std::size_t rowIndex) const;

	static const Matrix33 Identity;
	static const Matrix33 Zero;

	static inline Matrix33 Translation(const Vector2& translation);
	static inline Matrix33 Scale(const Vector2& scale);
	static inline Matrix33 Rotation(Radians rotation);
	static inline Matrix33 Rotation(const Quaternion& rotation);
  static inline Matrix33 Rotation(const Vector3& right, const Vector3& up, const Vector3& forward);
  static inline Matrix33 Rotation(const Vector3& forward, const Vector3& up);
};

inline bool operator==(const Matrix33& lhs, const Matrix33& rhs);
inline bool operator!=(const Matrix33& lhs, const Matrix33& rhs);

inline Matrix33 operator+(const Matrix33& lhs, float32 rhs);
inline Matrix33 operator-(const Matrix33& lhs, float32 rhs);
inline Matrix33 operator*(const Matrix33& lhs, float32 rhs);
inline Matrix33 operator/(const Matrix33& lhs, float32 rhs);

inline Matrix33 operator*(const Matrix33& lhs, const Matrix33& rhs);
inline Vector3 operator*(const Matrix33& lhs, const Vector3& rhs);
inline Vector2 operator*(const Matrix33& lhs, const Vector2& rhs);

inline float32 SignOfElement(const Matrix33& mat, std::size_t column,
							 std::size_t row);
inline float32 CofactorOfElement(const Matrix33& mat, std::size_t column,
								 std::size_t row);
inline Matrix33 Cofactor(const Matrix33& mat);
inline Matrix33 Transpose(const Matrix33& mat);
inline Matrix33 Adjoint(const Matrix33& mat);
inline Matrix33 Inverse(const Matrix33& mat);
inline float32 Determinant(const Matrix33& mat);

inline Quaternion ToQuaternion(const Matrix33& mat);
}
}

#include <SBLMath/Matrix33.inl>
