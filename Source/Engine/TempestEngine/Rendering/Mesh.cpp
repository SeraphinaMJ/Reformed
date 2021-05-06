/*!***************************************************************************************
\file       Mesh.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      The base class for all mesh types.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Mesh.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//
		
		mesh::mesh(const mesh& p_other)
		{
			m_vertices = p_other.m_vertices;
			m_label = p_other.m_label;
		}
		
		//==============================================================================//
		//        Destructor                                                            //
		//==============================================================================//

		//======================================================================================//
		//        Operators                                                                     //
		//======================================================================================//



		mesh& mesh::operator=(const mesh& p_other)
		{
			//check for self assignment
			if(this == &p_other)
			{
				return *this;
			}
			
			m_vertices = p_other.m_vertices;
			m_label = p_other.m_label;
			
			return *this;
		}

		//==============================================================================//
		//        Getters & Setters                                                     //
		//==============================================================================//
		std::vector<size_t> mesh::getAttributeElementSize() const
		{
		    std::vector<size_t> l_eleSizes = {
		        sizeof vertex::m_position,  /* size of a vector     */
		        sizeof vertex::m_normal,    /* size of a normal     */
                sizeof vertex::m_uv,        /* size of a uv         */
                sizeof vertex::m_tangent,   /* size of a tangent    */
                sizeof vertex::m_bitangent  /* size of a  bitangent */
		    };
		    return l_eleSizes;
		}

		mesh::vertex const& mesh::getVertex(int p_index) const
		{
		    return m_vertices[p_index];
		}

		size_t mesh::getVertexSize() const 
		{
		    return sizeof(vertex);
		}

		GLint mesh::getVertCount() const 
		{
		    return static_cast<GLint>(m_vertices.size());
		}
		

		void mesh::addVertexToMesh(float p_x, float p_y, float p_z)
		{
		    m_vertices.emplace_back(vertex(vector3(p_x, p_y, p_z)));
		}



