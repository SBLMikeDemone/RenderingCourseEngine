#include <SBLMath/Vector4.hpp>

namespace SBL
{
namespace Math
{
const Vector4 Vector4::Zero{0.0f, 0.0f, 0.0f, 0.0f};
const Vector4 Vector4::One{1.0f, 1.0f, 1.0f, 1.0f};
const Vector4 Vector4::Up{0.0f, 1.0f, 0.0f, 0.0f};
const Vector4 Vector4::Down{0.0f, -1.0f, 0.0f, 0.0f};
const Vector4 Vector4::Left{-1.0f, 0.0f, 0.0f, 0.0f};
const Vector4 Vector4::Right{1.0f, 0.0f, 0.0f, 0.0f};
const Vector4 Vector4::In{0.0f, 0.0f, -1.0f, 0.0f};
const Vector4 Vector4::Out{0.0f, 0.0f, 1.0f, 0.0f};
const Vector4 Vector4::Unit{0.5f, 0.5f, 0.5f, 0.5f};
}
}