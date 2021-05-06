/*!***************************************************************************************
\file       TriangleMesh.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This holds all data for constructing a mesh made of triangles.
*****************************************************************************************/

#include "../Precompiled.h"
//========Self Include==================================================================//
#include "TriangleMesh.hpp"
//========1st Party Includes============================================================//
#include "../Utility/GeneralizedFileLoader.hpp"
//========3rd Party Includes============================================================//  
#include <Vector2.hpp>
#include <Vector3.hpp>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//


void triangleMesh::loadInFile(assetHandle p_assetHandle)
{
	std::vector<float> l_verts;
	std::vector<unsigned> l_indices;
	std::vector<std::vector<unsigned>> l_facedata;


	std::string l_line;

	auto rawData = p_assetHandle->getData<genericAssetData>().getRaw();

	std::stringstream ss(std::string(rawData.begin(), rawData.end()));


	while (std::getline(ss, l_line))
	{
		if (*l_line.c_str() == '#' || l_line.length() == 0)
		{
			continue;
		}

		std::istringstream l_iss(l_line);
		std::string l_word;
		//char * l_word = std::strtok(const_cast<char *>(l_line.c_str() + 1), " ");
		l_verts.clear();

		l_facedata.clear();
		// std::cout << l_line << std::endl;
		l_iss >> l_word;
		while (l_iss >> l_word)
		{
			if (l_line.c_str()[0] == 'f')
			{
				std::istringstream l_iss2(l_word);
				std::string l_number;

				std::vector<unsigned> temp;
				l_indices.clear();
				while (std::getline(l_iss2, l_number, '/'))
				{

					auto l_num = static_cast<unsigned>(std::strtof(l_number.c_str(), NULL));

					if (l_num == 0)
						logger("help").debug() << "help/n";

					l_indices.push_back(l_num);
				}

				l_facedata.push_back(l_indices);
			}
			else
			{
				// std::cout << l_word << std::endl;
				auto l_num = static_cast<float>(std::strtof(l_word.c_str(), NULL));

				l_verts.push_back(l_num);
			}
		}

		if (l_line.c_str()[0] == 'v' && l_line.c_str()[1] == ' ')
		{
			addPositionToMesh(l_verts[0], l_verts[1], l_verts[2]);
		}

		if (l_line.c_str()[0] == 'v' && l_line.c_str()[1] == 't')
		{
			uvsSet = true;
			addUVToMesh(l_verts[0], l_verts[1]);
		}

		if (l_line.c_str()[0] == 'v' && l_line.c_str()[1] == 'n')
		{
			normalsSet = true;
			addNormalToMesh(l_verts[0], l_verts[1], l_verts[2]);

		}

		if (l_line.c_str()[0] == 'f' && l_line.c_str()[1] == ' ')
		{
			m_indices.push_back(l_facedata);

			//addFaceToMesh(static_cast<unsigned>(l_verts[0] - 1), static_cast<unsigned>(l_verts[1] - 1), static_cast<unsigned>(l_verts[2] - 1));
		}
	}

	//get the normals from Vertex data
	preprocess();

	// apply all data from file to vertex
	process();
}

triangleMesh::triangleMesh(const triangleMesh& p_other) : mesh(p_other)
{

}

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
void triangleMesh::addFaceToMesh(unsigned p_a, unsigned p_b, unsigned p_c)
{
	m_faces.emplace_back(p_a, p_b, p_c);
}

void triangleMesh::addUVToMesh(float p_u, float p_v)
{
	m_uvs.emplace_back(p_u, p_v);
}

void triangleMesh::addPositionToMesh(float p_x, float p_y, float p_z)
{
	m_position.emplace_back(p_x, p_y, p_z);
}

void triangleMesh::preprocess()
{
	// if normals did not come from file calculate them
	if (!normalsSet)
	{
		centerMesh();
		// normalizeVertices();
		// Get all the normals for each triangle

		/*
			Nx = UyVz - UzVy
			Ny = UzVx - UxVz
			Nz = UxVy - UyVx
		*/
		m_triangleNormals.clear();

		for (unsigned i = 0; i < m_faces.size(); i++)
		{
			vector3 l_u = m_vertices[m_faces[i].b].m_position - m_vertices[m_faces[i].a].m_position;
			vector3  l_v = m_vertices[m_faces[i].c].m_position - m_vertices[m_faces[i].a].m_position;

			// cross product
			vector3  l_normal(
				l_u.getY() * l_v.getZ() - l_u.getZ() * l_v.getY(),
				l_u.getZ() * l_v.getX() - l_u.getX() * l_v.getZ(),
				l_u.getX() * l_v.getY() - l_u.getY() * l_v.getX()
			);


			// accumulate adjacent normals
			l_normal.normalized();
			m_vertices[m_faces[i].a].m_normal += l_normal;
			m_vertices[m_faces[i].b].m_normal += l_normal;
			m_vertices[m_faces[i].c].m_normal += l_normal;


			m_triangleNormals.push_back(l_normal);
		}

		// normalize all the Vertex normals
		for (auto& i : m_vertices)
		{
			i.m_normal.normalized();
		}

		// Normalize all the triangle normals
		float l_epsilon = 0.000001f;
		float l_epsilonSq = l_epsilon * l_epsilon;

		for (auto & l_i : m_triangleNormals)
		{
			float l_lengthSq = l_i.getX() * l_i.getX() + l_i.getY() * l_i.getY() + l_i.getZ() * l_i.getZ();

			if (l_lengthSq >= l_epsilonSq)
			{
				float l_length = std::sqrt(l_lengthSq);
				l_i /= l_length;
			}
		}
	}
}

void triangleMesh::process()
{
	m_vertices.resize(m_position.size());

	// checks if the vertex already exist and we need to make a new vertex
	std::vector<bool> setCheck(m_position.size(), 0);

	// an index of normals that need to be added together 

	for (auto faceData : m_indices)
	{
		if (faceData.size() == 3)
		{
			unsigned indexA = faceData[0][0];
			unsigned indexB = faceData[1][0];
			unsigned indexC = faceData[2][0];

			if (!setCheck[faceData[0][0] - 1])
			{
				setCheck[faceData[0][0] - 1] = true;
			}
			else
			{
				m_vertices.emplace_back();
				indexA = static_cast<unsigned>(m_vertices.size());
			}

			if (!setCheck[faceData[1][0] - 1])
			{
				setCheck[faceData[1][0] - 1] = true;
			}
			else
			{
				m_vertices.emplace_back();
				indexB = static_cast<unsigned>(m_vertices.size());
			}

			if (!setCheck[faceData[2][0] - 1])
			{
				setCheck[faceData[2][0] - 1] = true;
			}
			else
			{
				m_vertices.emplace_back();
				indexC = static_cast<unsigned>(m_vertices.size());
			}


			addFaceToMesh(indexA - 1, indexB - 1, indexC - 1);

			m_vertices[indexA - 1].m_position = m_position[faceData[0][0] - 1];
			m_vertices[indexB - 1].m_position = m_position[faceData[1][0] - 1];
			m_vertices[indexC - 1].m_position = m_position[faceData[2][0] - 1];

			m_vertices[indexA - 1].m_uv = m_uvs[faceData[0][1] - 1];
			m_vertices[indexB - 1].m_uv = m_uvs[faceData[1][1] - 1];
			m_vertices[indexC - 1].m_uv = m_uvs[faceData[2][1] - 1];

			m_vertices[indexA - 1].m_normal = m_triangleNormals[faceData[0][2] - 1];
			m_vertices[indexB - 1].m_normal = m_triangleNormals[faceData[1][2] - 1];
			m_vertices[indexC - 1].m_normal = m_triangleNormals[faceData[2][2] - 1];
		}

		if (faceData.size() == 4)
		{

			unsigned indexA = faceData[0][0];
			unsigned indexB = faceData[1][0];
			unsigned indexC = faceData[2][0];
			unsigned indexD = faceData[3][0];


			if (!setCheck[faceData[0][0] - 1])
			{
				setCheck[faceData[0][0] - 1] = true;
			}
			else
			{
				m_vertices.emplace_back();
				indexA = static_cast<unsigned>(m_vertices.size());
			}

			if (!setCheck[faceData[1][0] - 1])
			{
				setCheck[faceData[1][0] - 1] = true;
			}
			else
			{

				m_vertices.emplace_back();
				indexB = static_cast<unsigned>(m_vertices.size());
			}

			if (!setCheck[faceData[2][0] - 1])
			{
				setCheck[faceData[2][0] - 1] = true;
			}
			else
			{

				m_vertices.emplace_back();
				indexC = static_cast<unsigned>(m_vertices.size());
			}

			if (!setCheck[faceData[3][0] - 1])
			{
				setCheck[faceData[3][0] - 1] = true;
			}
			else
			{

				m_vertices.emplace_back();
				indexD = static_cast<unsigned>(m_vertices.size());
			}

			addFaceToMesh(indexA - 1, indexB - 1, indexC - 1);
			addFaceToMesh(indexC - 1, indexD - 1, indexA - 1);

			m_vertices[indexA - 1].m_position = m_position[faceData[0][0] - 1];
			m_vertices[indexB - 1].m_position = m_position[faceData[1][0] - 1];
			m_vertices[indexC - 1].m_position = m_position[faceData[2][0] - 1];
			m_vertices[indexD - 1].m_position = m_position[faceData[3][0] - 1];

			m_vertices[indexA - 1].m_uv = m_uvs[faceData[0][1] - 1];
			m_vertices[indexB - 1].m_uv = m_uvs[faceData[1][1] - 1];
			m_vertices[indexC - 1].m_uv = m_uvs[faceData[2][1] - 1];
			m_vertices[indexD - 1].m_uv = m_uvs[faceData[3][1] - 1];

			m_vertices[indexA - 1].m_normal = m_triangleNormals[faceData[0][2] - 1];
			m_vertices[indexB - 1].m_normal = m_triangleNormals[faceData[1][2] - 1];
			m_vertices[indexC - 1].m_normal = m_triangleNormals[faceData[2][2] - 1];
			m_vertices[indexD - 1].m_normal = m_triangleNormals[faceData[3][2] - 1];


		}
	}
}



void triangleMesh::addNormalToMesh(float x, float y, float z)
{
	m_triangleNormals.emplace_back(x, y, z);
}

//======================================================================================//
//        Operators                                                                     //
//======================================================================================//

triangleMesh& triangleMesh::operator=(const triangleMesh& p_other)
{
	if (this == &p_other)
	{
		return *this;
	}

	mesh::operator=(p_other);
	m_center = p_other.m_center;
	m_faces = p_other.m_faces;
	m_position = p_other.m_position;
	m_triangleNormals = p_other.m_triangleNormals;
	m_uvs = p_other.m_uvs;
	m_indices = p_other.m_indices;

	normalsSet = p_other.normalsSet;
	uvsSet = p_other.uvsSet;

	return *this;
}

//==============================================================================//
		//        Helper                                                                //
		//==============================================================================//
void triangleMesh::centerMesh()
{
	// find the centroid of the entire mesh 
	for (auto l_vert : m_vertices)
	{
		m_center += l_vert.m_position;
	}

	// get average
	m_center *= 1 / static_cast<float>(m_vertices.size());

	for (auto & l_vert : m_vertices)
	{
		l_vert.m_position += -(m_center);
	}
}



//https://en.wikipedia.org/wiki/Cube_mapping
triangleMesh* triangleMesh::CalcUvBox()
{
	for (auto & v : m_vertices)
	{
		vector3 position = v.m_position * 2;

		float absX = fabs(position.getX());
		float absY = fabs(position.getY());
		float absZ = fabs(position.getZ());

		int isXPositive = position.getX() > 0 ? 1 : 0;
		int isYPositive = position.getY() > 0 ? 1 : 0;
		int isZPositive = position.getZ() > 0 ? 1 : 0;

		float maxAxis = 0, uc = 0, vc = 0, u_ = 0, v_ = 0;


		// POSITIVE X
		if (isXPositive && absX >= absY && absX >= absZ) {
			// u (0 to 1) goes from +z to -z
			// v (0 to 1) goes from -y to +y
			maxAxis = absX;
			uc = -position.getZ();
			vc = position.getY();
		}
		// NEGATIVE X
		else if (!isXPositive && absX >= absY && absX >= absZ) {
			// u (0 to 1) goes from -z to +z
			// v (0 to 1) goes from -y to +y
			maxAxis = absX;
			uc = position.getZ();
			vc = position.getY();

		}
		// POSITIVE Y
		else if (isYPositive && absY >= absX && absY >= absZ) {
			// u (0 to 1) goes from -x to +x
			// v (0 to 1) goes from +z to -z
			maxAxis = absY;
			uc = position.getX();
			vc = -position.getZ();
		}
		// NEGATIVE Y
		else if (!isYPositive && absY >= absX && absY >= absZ) {
			// u (0 to 1) goes from -x to +x
			// v (0 to 1) goes from -z to +z
			maxAxis = absY;
			uc = position.getX();
			vc = position.getZ();
		}
		// POSITIVE Z
		else if (isZPositive && absZ >= absX && absZ >= absY) {
			// u (0 to 1) goes from -x to +x
			// v (0 to 1) goes from -y to +y
			maxAxis = absZ;
			uc = position.getX();
			vc = position.getY();
		}
		// NEGATIVE Z
		else if (!isZPositive && absZ >= absX && absZ >= absY) {
			// u (0 to 1) goes from +x to -x
			// v (0 to 1) goes from -y to +y
			maxAxis = absZ;
			uc = -position.getX();
			vc = position.getY();
		}

		// Convert range from -1 to 1 to 0 to 1
		u_ = 0.5f * (uc / maxAxis + 1.0f);
		v_ = 0.5f * (vc / maxAxis + 1.0f);

		//m_boxUV.push_back(Math::Vec2(u, v));

		v.m_uv = vector2(u_, v_);
	}

	return this;
}





