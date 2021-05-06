/*!***************************************************************************************
\file       ParticleMesh.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "Color.hpp"
#include <Vector4.hpp>

#include <vector>

class particleMesh
{
public:
    struct gpu_particle
    {
        //==============================================================================//
        //        Constructor                                                           //
        //==============================================================================//
        vector4 m_position = { 0,0,0,0 };       //!< layout(location = 0) position and size of the particle
        color m_color = { 0,0,0 };            //!< layout(location = 1) color of the particle
		//color m_startColor = { 1,0,0 };       //!< start colorRange for fire particle
		//color m_endColor = { 0,0,1 };         //!< end colorRange for fore particle
		//float t;

        /*!*******************************************************************************
        \brief  constructor(default)
        *********************************************************************************/
        gpu_particle() = default;

        /*!*******************************************************************************
        \brief  parameterized constructor
        \param  p_position - the model position of the vertex
        *********************************************************************************/
        gpu_particle(vector4 const& p_position, color p_color) : m_position(p_position), m_color(p_color){}

    };

    /*!***************************************************************************************
    \brief  gets how many layout locations there are
    \return the number of layouts
    *****************************************************************************************/
    size_t getAttributeCount() const { return 2; }

    /*!***************************************************************************************
    \brief   get the count for each attribute
    \return  a vector where each element hold the quanity of data in a layout
    *****************************************************************************************/
    std::vector<size_t> getAttributeElementCounts() const { return { 4,4 }; }

    /*!***************************************************************************************
    \brief  get the size of each attribute
    \return a vector that hold each attributes data size
    *****************************************************************************************/
    std::vector<size_t> getAttributeElementSize() const;

    /*!***************************************************************************************
   \brief  the size of a vertice
   \return the size of the vertex struct
   *****************************************************************************************/
    size_t getParticleSize() const;
};
