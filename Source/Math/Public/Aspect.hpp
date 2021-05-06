#pragma once
// simple header for doing ratio-based math

#include <utility>
#include <array>

#include <Vector2.hpp>

// bottom-left x and y coordinates
// width and height of viewport rectangle
/***************************
                   (x+w,y+h)
   X-------------------X
   |                   |
 h |                   |
   |                   |
   X ------------------X
(x,y)        w
***************************/
struct viewportGeometry
{
	int x, y, width, height;

	// x1, x2, y1, y2
	[[nodiscard]] constexpr auto extents() const noexcept
	{
		return std::array{ x, x + width, y, y + height };
	}

	[[nodiscard]] constexpr auto toArray() const noexcept
	{
		return std::array{ x, y, width, height };
	}

	[[nodiscard]] auto asVectors() const
	{
		return std::array{ vector2(x,y), vector2(width,height) };
	}
};

[[nodiscard]] constexpr viewportGeometry maintainAspect(const std::pair<int, int> dim, const std::pair<int, int> aspect) noexcept
{
	const auto& [width, height] = dim;
	const auto& [aspectWidth, aspectHeight] = aspect;

	// comparing aspects
	// width1 / height1 <=> width2 / height2
	// can be rewritten as
	// width1 * height2 <=> width2 * height1
	// to avoid integer division

	const auto relativeWidth = width * aspectHeight;
	const auto relativeHeight = height * aspectWidth;

	// reduce width
	if (relativeWidth > relativeHeight)
	{
		const auto newWidth = relativeHeight / aspectHeight;
		const auto offset = (width - newWidth) / 2;
		return { offset, 0, newWidth, height };
	}
	// reduce height
	else
	{
		const auto newHeight = relativeWidth / aspectWidth;
		const auto offset = (height - newHeight) / 2;
		return { 0, offset, width, newHeight };
	}
}
