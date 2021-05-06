#include "LineSegment.hpp"

lineSegment::lineSegment(vector3 const& p_begin, vector3 const& p_end,
	vector3 const& p_color, float p_weight) noexcept
	: begin{p_begin}
	, end{p_end}
	, color{p_color}
	, weight{p_weight}
{

}
