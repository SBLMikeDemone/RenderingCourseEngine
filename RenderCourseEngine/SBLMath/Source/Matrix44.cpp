#include <SBLMath/Matrix44.hpp>

namespace SBL
{
namespace Math
{
const Matrix44 Matrix44::Identity{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
								  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
								  0.0f, 0.0f, 0.0f, 1.0f};
const Matrix44 Matrix44::Zero{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}
}