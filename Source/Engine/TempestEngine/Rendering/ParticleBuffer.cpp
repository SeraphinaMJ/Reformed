/*!***************************************************************************************
\file       ParticleBuffer.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "../Precompiled.h"
#include "ParticleBuffer.hpp"
#include "ParticleMesh.hpp"

#include <GL/glew.h>

particleBuffer::particleBuffer(size_t p_maxParticleCount, size_t p_particleSize) : m_maxParticleCount(p_maxParticleCount),
                                                                                   m_particleCount(0),
                                                                                   m_particleSize(p_particleSize),
                                                                                   m_bufferSize(m_maxParticleCount * p_particleSize),
                                                                                   m_buffer(new char[m_bufferSize]),
                                                                                   m_billboardBufferHandle(0),
                                                                                   m_handle(0)
{

}

particleBuffer::~particleBuffer()
{
    shutdown();
    delete[] m_buffer;
}

size_t particleBuffer::getBufferSize() const
{
    return m_bufferSize;
}

void particleBuffer::build(particleMesh * p_mesh)
{

    glGenVertexArrays(1, &m_vaoHandle);
    glBindVertexArray(m_vaoHandle);


    glGenBuffers(1, &m_billboardBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_billboardBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // set up billboardBuffer to be used by all particles (unchanging)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_billboardBufferHandle);
    glVertexAttribPointer(
        0, // attribute. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        nullptr // array buffer offset
    );


 /*   size_t l_offset = 0;*/
   /* size_t l_attributeCount = p_mesh->getAttributeCount();*/

    //generate buffer on gpu side
    glGenBuffers(1, &m_handle);
    bind();
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, m_particleSize * m_maxParticleCount, NULL, GL_STREAM_DRAW);


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glVertexAttribPointer(
        1, // attribute. No particular reason for 1, but must match the layout in the shader.
        static_cast<GLint>(p_mesh->getAttributeElementCounts()[0]), // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        static_cast<GLuint>(m_particleSize), // stride
        0 // array buffer offset
    );


    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glVertexAttribPointer(
        2, // attribute. No particular reason for 1, but must match the layout in the shader.
        static_cast<GLint>(p_mesh->getAttributeElementCounts()[1]), // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        static_cast<GLuint>(m_particleSize), // stride
        reinterpret_cast<GLvoid *>(p_mesh->getAttributeElementSize()[0]) // array buffer offset
    );

    glBindVertexArray(NULL);

}

void particleBuffer::update()
{

    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticleCount * m_particleSize, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleCount * m_particleSize, m_buffer);
}

void particleBuffer::render()
{
    glBindVertexArray(m_vaoHandle);

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1


    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particleCount);

    glBindVertexArray(NULL);
}

void particleBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void particleBuffer::unbind() const
{

}

void particleBuffer::shutdown()
{
	glDeleteBuffers(1, &m_handle);
	glDeleteBuffers(1, &m_billboardBufferHandle);
	glDeleteVertexArrays(1, &m_vaoHandle);
}
