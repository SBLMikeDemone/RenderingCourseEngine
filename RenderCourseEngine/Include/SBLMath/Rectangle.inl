#pragma once

#include <SBLMath/Rectangle.hpp>

namespace SBL
{
namespace Math
{
inline int32 Width(const Rectangle& rect)
{
	return rect.m_right - rect.m_left;
}

inline int32 Height(const Rectangle& rect)
{
	return rect.m_bottom - rect.m_top;
}
}
}
