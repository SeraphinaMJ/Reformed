/*!***************************************************************************************
\file       ParticleMesh.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "ParticleMesh.hpp"

#include <vector>

std::vector<size_t> particleMesh::getAttributeElementSize() const
{

    std::vector<size_t> l_eleSizes = {
        sizeof(gpu_particle::m_position),  /* size of a vector     */
        sizeof(gpu_particle::m_color),    /* size of a color4     */
    };
    return l_eleSizes;
}

size_t particleMesh::getParticleSize() const
{
    return sizeof(gpu_particle);
}



