#include <SBLMath/Vector3.hpp>

namespace SBL
{
namespace Math
{
const Vector3 Vector3::Zero{0.0f, 0.0f, 0.0f};
const Vector3 Vector3::One{1.0f, 1.0f, 1.0f};
const Vector3 Vector3::Up{0.0f, 1.0f, 0.0f};
const Vector3 Vector3::Down{0.0f, -1.0f, 0.0f};
const Vector3 Vector3::Left{-1.0f, 0.0f, 0.0f};
const Vector3 Vector3::Right{1.0f, 0.0f, 0.0f};
const Vector3 Vector3::In{0.0f, 0.0f, -1.0f};
const Vector3 Vector3::Out{0.0f, 0.0f, 1.0f};
const Vector3 Vector3::Unit{0.577350f, 0.577350f, 0.577350f};
}
}