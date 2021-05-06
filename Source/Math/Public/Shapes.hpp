/*!**********************************************************************************************************
\file       Shapes.hpp
\author     Henry Brobeck
\date       3/8/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#pragma once
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <vector>

typedef const vector3& vec3param;

enum intersection_type
{
	int_coplanar = 0,
	int_outside,
	int_inside,
	int_overlaps
};


class plane
{
public:
	plane();
	plane(vec3param p0, vec3param p1, vec3param p2);
	plane(vec3param normal, vec3param point);
	void set(vec3param normal, vec3param point);
	void set(vec3param p0, vec3param p1, vec3param p2);
	vector3 getNormal() const;
	float getDistance();
	intersection_type checkPoint(const vector3& p_point) const;

	vector4 m_data;
};
class aabb
{
public:

	aabb();
	aabb(vec3param min, vec3param max);

	float getVolume() const;
	float getSurfaceArea() const;
	void Compute(const std::vector<vector3>& p_points);

	vector3 getMin() const;
	vector3 getMax() const;
	vector3 getCenter() const;
	vector3 getHalfSize() const;

	intersection_type checkPlane(const plane& p_plane) const;

private:
	vector3 m_min;
	vector3 m_max;

};

class frustum
{
public:
	void set(vec3param lbn, vec3param rbn, vec3param rtn, vec3param ltn,
		vec3param lbf, vec3param rbf, vec3param rtf, vec3param ltf);

	vector4* getPlanes() const;
	bool collides(const aabb& p_aabb);
private:
	plane m_planes[6];


};



// namespace <namespace name>

