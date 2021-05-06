/*!***************************************************************************************
\file       MeshDebug.cpp
\author     Henry Brobeck
\date       8/15/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Decorator for Mesh for extra debug info
*****************************************************************************************/

#include "MeshDebug.hpp"

meshDebug::meshDebug(const triangleMesh& p_parent) : triangleMesh(p_parent)
{
	//create the debug info
	generateData();
}

meshDebug& meshDebug::operator=(const triangleMesh& p_parent)
{
	//call parent operator
	triangleMesh::operator=(p_parent);
	//carete the debug info
	generateData();

	return *this;
}

glm::vec3 meshDebug::getMinPos()
{
	return m_min_pos;
}

glm::vec3 meshDebug::getMaxPos()
{
	return m_max_pos;
}

std::tuple<glm::vec3, glm::vec3> meshDebug::getMinMax()
{
	return std::make_tuple(m_min_pos, m_max_pos);
}

glm::vec3 meshDebug::getBoxSize()
{
	return m_box_size;
}


void meshDebug::generateData()
{
	//set up default values
	m_line_length = 0.1f;
	m_box_color = { 1.0f, 0.0f, 0.0f };
	
	//get minimum and maximum
	float l_min_float = std::numeric_limits<float>::min();
	float l_max_float = std::numeric_limits<float>::max();

	//create values with initial mins/max
	m_min_pos = { l_max_float,l_max_float, l_max_float }; 
	m_max_pos = { l_min_float,l_min_float, l_min_float };

	for(auto &iter : m_vertices)
	{
		float l_x = iter.m_position.x;
		float l_y = iter.m_position.y;
		float l_z = iter.m_position.z;

		//update minimum
		if(l_x < m_min_pos.x)
		{
			m_min_pos.x = l_x;
		}
		if(l_y < m_min_pos.y)
		{
			m_min_pos.y = l_y;
		}
		if(l_z < m_min_pos.z)
		{
			m_min_pos.z = l_z;
		}

		//update maximum
		if(l_x > m_max_pos.x)
		{
			m_max_pos.x = l_x;
		}
		if(l_y > m_max_pos.y)
		{
			m_max_pos.y = l_y;
		}
		if(l_z > m_max_pos.z)
		{
			m_max_pos.z = l_z;
		}



	}

	//calculate box size
	m_box_size = { m_max_pos.x - m_min_pos.x,m_max_pos.y - m_min_pos.y, m_max_pos.z - m_min_pos.z };

}
