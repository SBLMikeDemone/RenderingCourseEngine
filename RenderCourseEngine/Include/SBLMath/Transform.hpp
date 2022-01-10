#pragma once

#include <SBLMath/Matrix44.hpp>
#include <SBLMath/Quaternion.hpp>
#include <SBLMath/Vector3.hpp>

namespace SBL
{
namespace Math
{
struct UniformTransform final
{
	Vector3 m_translation;
	Quaternion m_rotation;
	float m_uniformScale;
};

Matrix44 BuildWorldTransformMatrix(const UniformTransform& transform);
}
}
