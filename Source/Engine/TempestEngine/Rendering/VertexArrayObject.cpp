/*!***************************************************************************************
\file       VertexArrayObject.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      The CPU repersentation of the VAO that holds all shaders for pipeline
*****************************************************************************************/

//========Self Include==================================================================//
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

    ///////========================================================================///////
    ///////   Private                                                              ///////
    ///////========================================================================///////

        //////======================================================================//////
        //////    Non-Static                                                        //////
        //////======================================================================//////

        /////========================================================================/////
        /////     Functions                                                          /////
        /////========================================================================/////

        ////==========================================================================////
        ////      Non-Virtual                                                         ////
        ////==========================================================================////

        //==============================================================================//
        //        Helper                                                                //
        //==============================================================================//


#include "../Precompiled.h"
#include "VertexArrayObject.hpp"
#include "Mesh.hpp"

#include <GL/glew.h>



vertexArrayObject::vertexArrayObject(size_t p_vertexCount, size_t p_primitiveCount, size_t p_vertexSize, topology p_topology) : m_handle(0),
                                                                                                                        m_ibo(p_topology, p_primitiveCount),
                                                                                                                        m_vbo(p_vertexCount, p_vertexSize)

{
}

vertexArrayObject::~vertexArrayObject()
{
    m_ibo.shutdown();
    m_vbo.shutdown();
    glDeleteVertexArrays(1, &m_handle);
}

void vertexArrayObject::build(const mesh * p_mesh)
{
	if (p_mesh == m_mesh) return;

	m_mesh = p_mesh;

    //generate Vertex array on gpu side
    glGenVertexArrays(1, &m_handle);
    bind();

    size_t l_offset = 0;
    size_t l_attributeCount = p_mesh->getAttributeCount();
    size_t const l_vertexSize = p_mesh->getVertexSize();

    m_vbo.build();

    for (size_t i = 0; i < l_attributeCount; ++i)
    {
        glEnableVertexAttribArray(static_cast<GLuint>(i));

        // send the attribute data to the VAO
        glVertexAttribPointer(static_cast<GLuint>(i),
            static_cast<GLint>(p_mesh->getAttributeElementCounts()[i]), GL_FLOAT, GL_FALSE,
            static_cast<GLuint>(l_vertexSize), reinterpret_cast<GLvoid *>(l_offset));

        l_offset += p_mesh->getAttributeElementSize()[i]; // skip to the next attribute

        }

    m_ibo.build();

    unbind();

}

void vertexArrayObject::bind()
{
    glBindVertexArray(m_handle);
}

void vertexArrayObject::render()
{
    switch(m_ibo.getTopology())
    {
        case topology::enm_lines:
            glDrawElements(GL_LINES, static_cast<GLsizei>(m_ibo.getIndexCount()), GL_UNSIGNED_INT, nullptr);
            break;
        case topology::enm_triangle:
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_ibo.getIndexCount()), GL_UNSIGNED_INT, nullptr);
            break;
    }
}

void vertexArrayObject::unbind()
{
    glBindVertexArray(0);
}

mesh const * vertexArrayObject::lastBuilt() const
{
	return m_mesh;
}




