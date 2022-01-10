#include <SBLMath/Matrix33.hpp>

namespace SBL
{
namespace Math
{
const Matrix33 Matrix33::Identity{1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
								  0.0f, 0.0f, 0.0f, 1.0f};
const Matrix33 Matrix33::Zero{0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f};
}
}