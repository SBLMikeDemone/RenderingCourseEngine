#pragma once

#include <SBLMath/Common.hpp>

namespace SBL
{
namespace Math
{
struct Rectangle
{
	int32 m_left;
	int32 m_top;
	int32 m_right;
	int32 m_bottom;
};

inline int32 Width(const Rectangle& rect);
inline int32 Height(const Rectangle& rect);
}
}

#include <SBLMath/Rectangle.inl>