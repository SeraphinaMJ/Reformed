#pragma once

#include "Vector3.hpp"

struct lineSegment
{
	lineSegment(vector3 const& p_begin, vector3 const& p_end,
		vector3 const& p_color = { 0,0,0 }, float p_weight = 0.05f) noexcept;
	lineSegment(lineSegment const&) = default;
	lineSegment(lineSegment&&) = default;
	~lineSegment() noexcept = default;
	lineSegment& operator=(lineSegment const&) = default;
	lineSegment& operator=(lineSegment&&) = default;

	vector3 begin;	//!< beginning of the segment
	vector3 end;	//!< end of the segment
	vector3 color;	//!< display color
	float weight;	//!< width of the line when displayed
};
