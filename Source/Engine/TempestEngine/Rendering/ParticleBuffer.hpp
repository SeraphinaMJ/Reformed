/*!***************************************************************************************
\file       ParticleBuffer.hpp
\author     Seraphina Kim
\date       7/31/18
\copyright  All content @ 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include "GLTypes.hpp"

class particleMesh;


static const GLfloat g_vertex_buffer_data[] = {
                         -0.1f, -0.1f, 0.0f,
                         0.1f, -0.1f, 0.0f,
                         -0.1f, 0.1f, 0.0f,
                         0.1f, 0.1f, 0.0f,
	

};


class particleBuffer
{
public:

    

    particleBuffer(size_t p_particleCount, size_t p_particleSize);
    ~particleBuffer() ;

    template <class TParticle>
    bool addParticle(size_t m_insertOffset, TParticle const& p_vertex);
    
    size_t getBufferSize() const ;
    void build(particleMesh * p_mesh) ;
    void update();
    void render();
    void bind() ;
    void unbind() const ;
    void shutdown();
    int & getParticleCount() { return m_particleCount; }
    size_t getMaxParticles() { return m_maxParticleCount; }
    void resetParticleCount() { m_particleCount = 0; }

private:

    /////================================================================================/////
    /////     Data                                                                       /////
    /////================================================================================/////

    size_t m_maxParticleCount;           //!< The amount of vertex in VBO
    int m_particleCount;
    size_t m_particleSize;
    size_t m_bufferSize;            //!< The size of the buffer
              //!< The offset into buffer
    char * m_buffer;

    GLuint m_billboardBufferHandle;

    GLuint m_vaoHandle;

    GLuint m_handle;
   
};

template <typename TParticle>
bool particleBuffer::addParticle(size_t m_insertOffset, TParticle const& p_vertex)
{

    TParticle * l_vertexBuffer = reinterpret_cast<TParticle *>(m_buffer);
    l_vertexBuffer[m_insertOffset++] = p_vertex;
    return true;

}
