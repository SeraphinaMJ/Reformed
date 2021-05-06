/*!***************************************************************************************
\file       IndexBufferObject.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      The CPU side repersentation of the GPU buffer object that hold index data
*****************************************************************************************/
#include "../Precompiled.h"
//========Self Include==================================================================//
#include "IndexBufferObject.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <GL/glew.h>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

    ///////========================================================================///////
    ///////   Public                                                               ///////
    ///////========================================================================///////

        //==============================================================================//
        //        Constructor                                                           //
        //==============================================================================//
        indexBufferObject::indexBufferObject(topology p_topology, size_t p_primitiveCount)
        : m_topology(p_topology),
          m_indexCount(static_cast<int>(p_topology) * p_primitiveCount),
          m_insertOffset(0),
          m_bufferSize(sizeof(unsigned) * m_indexCount),
          m_buffer(new char[m_bufferSize]),
          m_handle(0)
        {
        }
        //==============================================================================//
        //        Destructor                                                            //
        //==============================================================================//

        indexBufferObject::~indexBufferObject()
        {
            // cleanup on gpu side
            indexBufferObject::shutdown();
            delete[] m_buffer;
        }

        //==============================================================================//
        //          Helper Destructor                                                   //
        //==============================================================================//
        void indexBufferObject::shutdown()
        {
            glDeleteBuffers(1, &m_handle);
        }

        //==============================================================================//
        //        Getters & Setters                                                     //
        //==============================================================================//
        size_t indexBufferObject::getBufferSize() const
        {
            return m_bufferSize;
        }

        ////==========================================================================////
        ////      Non-Virtual                                                         ////
        ////==========================================================================////

        //////======================================================================//////
        //////    Non-Static                                                        //////
        //////======================================================================//////

        /////========================================================================/////
        /////     Functions                                                          /////
        /////========================================================================/////
        void indexBufferObject::build()
        {
            glGenBuffers(1, &m_handle);
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufferSize, m_buffer, GL_STATIC_DRAW);

        }

        void indexBufferObject::bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
        }

        void indexBufferObject::unbind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

       
        void indexBufferObject::addLine(unsigned p_a, unsigned p_b)
        {

            // needs a count check

            unsigned * l_indexBuffer = reinterpret_cast<unsigned*>(m_buffer);
            l_indexBuffer[m_insertOffset++] = p_a;
            l_indexBuffer[m_insertOffset++] = p_b;

        }

        void indexBufferObject::addTriangle(unsigned p_a, unsigned p_b, unsigned p_c)
        {
            //needs a count check

            auto * l_indexBuffer = reinterpret_cast<unsigned*>(m_buffer);

            l_indexBuffer[m_insertOffset++] = p_a;
            l_indexBuffer[m_insertOffset++] = p_b;
            l_indexBuffer[m_insertOffset++] = p_c;

        }
    









