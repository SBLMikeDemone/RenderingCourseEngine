#pragma once

#include <SBLMath/Common.hpp>
#include <SBLMath/Vector2.hpp>

#include <cmath>
#include <type_traits>

namespace SBL
{
namespace Math
{
struct Matrix22
{
	Matrix22() = default;
	~Matrix22() = default;
	Matrix22(const Matrix22& rhs) = default;
	Matrix22(Matrix22&& rhs) = default;
	Matrix22& operator=(const Matrix22& rhs) = default;
	Matrix22& operator=(Matrix22&& rhs) = default;
	inline Matrix22(float32 _00, float32 _01, float32 _10, float32 _11);

	union {
		struct
		{
			float32 m00;
			float32 m01;
			float32 m10;
			float32 m11;
		};
		Vector2 rows[2];
		float32 values[4];
	};

	inline Vector2& operator[](std::size_t rowIndex);
	inline const Vector2& operator[](std::size_t rowIndex) const;
	inline Vector2 Column(std::size_t columnIndex) const;
	inline Vector2 Row(std::size_t rowIndex) const;

	static const Matrix22 Identity;
	static const Matrix22 Zero;

	static inline Matrix22 Translation(const Vector2& translation);
	static inline Matrix22 Scale(const Vector2& scale);
	static inline Matrix22 Rotation(Radians rotation);
};

inline bool operator==(const Matrix22& lhs, const Matrix22& rhs);
inline bool operator!=(const Matrix22& lhs, const Matrix22& rhs);

inline Matrix22 operator*(const Matrix22& lhs, const Matrix22& rhs);
inline Vector2 operator*(const Matrix22& lhs, const Vector2& rhs);
inline Vector2 operator*(const Matrix22& lhs, const Vector2& rhs);

inline float32 SignOfElement(const Matrix22& mat, std::size_t column,
							 std::size_t row);
inline float32 CofactorOfElement(const Matrix22& mat, std::size_t column,
								 std::size_t row);
inline float32 Determinant(const Matrix22& mat);
}
}

#include <SBLMath/Matrix22.inl>
