/*!**********************************************************************************************************
\file       Shapes.cpp
\author     Henry Brobeck
\date       3/8/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "Shapes.hpp"

#include <limits>
#include <algorithm>

#include <cmath>


//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Static Deceleration  ==========================================================================//
//========  Static Class Methods  =========================================================================//

plane::plane() : m_data(0, 0, 0, 0)
{

}

plane::plane(vec3param p0, vec3param p1, vec3param p2)
{
	set(p0, p1, p2);
}

plane::plane(vec3param normal, vec3param point)
{
	set(normal, point);
}

void plane::set(vec3param normal, vec3param point)
{
	const vector3 norm = (normal == vector3::Zero) ? normal : normal.normalize();
	const float d = norm.dotP(point);
	m_data.x = norm.x;
	m_data.y = norm.y;
	m_data.z = norm.z;
	m_data.w = d;
}

void plane::set(vec3param p0, vec3param p1, vec3param p2)
{
	vector3 norm = (p1 - p0).crossP(p2 - p0);
	set(norm, p0);
}

vector3 plane::getNormal() const
{
	return vector3(m_data.x, m_data.y, m_data.z);
}

float plane::getDistance()
{
	return m_data.w;
}

intersection_type plane::checkPoint(const vector3& p_point) const
{
	float distance = vector4(p_point, -1).dotP(m_data);

	float next = std::nextafterf(0.0f, distance);
	if (next == distance)
	{
		return int_coplanar;
	}
	if (next < 0)
	{
		return int_outside;
	}
	return int_inside;
}

aabb::aabb()
{
	m_min.splat(-std::numeric_limits<float>::max());
	m_max.splat(std::numeric_limits<float>::max());
}

aabb::aabb(vec3param min, vec3param max)
{
	m_min = min;
	m_max = max;
}

float aabb::getVolume() const
{
	vector3 dim = m_max - m_min;
	return dim.x * dim.y * dim.z;
}

float aabb::getSurfaceArea() const
{
	vector3 dim = m_max - m_min;
	return dim.x * dim.y * 2 + dim.x * dim.x * 2 + dim.y * dim.z * 2;
}

void aabb::Compute(const std::vector<vector3>& p_points)
{
	m_min.splat(std::numeric_limits<float>::max());
	m_max.splat(-std::numeric_limits<float>::max());
	for (auto iter : p_points)
	{
		m_min.x = std::min<float>(m_min.x, iter.x);
		m_min.y = std::min<float>(m_min.y, iter.y);
		m_min.z = std::min<float>(m_min.z, iter.z);
		m_max.x = std::max<float>(m_max.x, iter.x);
		m_max.y = std::max<float>(m_max.y, iter.y);
		m_max.z = std::max<float>(m_max.z, iter.z);
	}
}

vector3 aabb::getMin() const
{
	return m_min;
}

vector3 aabb::getMax() const
{
	return m_max;
}

vector3 aabb::getCenter() const
{
	vector3 size = m_max - m_min;
	return m_min + size / 2.0f;
}

vector3 aabb::getHalfSize() const
{
	return (m_max - m_min) / 2.0f;
}

intersection_type aabb::checkPlane(const plane& p_plane) const
{
	vector3 normal = p_plane.getNormal();
	vector3 half = getHalfSize();
	vector3 center = getCenter();

	vector3 f = half;

	if (normal.x < 0)
	{
		f.x = -half.x;
	}
	if (normal.y < 0)
	{
		f.y = -half.y;
	}
	if (normal.z < 0)
	{
		f.z = -half.z;
	}

	auto far_result = p_plane.checkPoint(center + f);
	auto near_result = p_plane.checkPoint(center - f);

	if (far_result == int_coplanar || near_result == int_coplanar)
	{
		return int_overlaps;
	}
	return intersection_type(far_result | near_result);

}

void frustum::set(vec3param lbn, vec3param rbn, vec3param rtn, vec3param ltn, vec3param lbf, vec3param rbf,
	vec3param rtf, vec3param ltf)
{
	//left
	m_planes[0].set(lbf, ltf, lbn);
	//right
	m_planes[1].set(rbn, rtf, rbf);
	//top
	m_planes[2].set(ltn, ltf, rtn);
	//bot
	m_planes[3].set(rbn, lbf, lbn);
	//near
	m_planes[4].set(lbn, ltn, rbn);
	//far
	m_planes[5].set(rbf, rtf, lbf);
}

vector4* frustum::getPlanes() const
{
	return (vector4*)m_planes;
}

bool frustum::collides(const aabb& p_aabb)
{
	for (unsigned i = 0; i < 6; ++i)
	{
		auto plane_result = p_aabb.checkPlane(m_planes[i]);
		if (plane_result == int_outside)
		{
			return false;
		}
	}
	return true;
}
