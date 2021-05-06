/*!***************************************************************************************
\file       Mesh.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      The base class for all mesh types.
*****************************************************************************************/

#pragma once

//========1st Party Includes============================================================//
#include "../Utility/AssetManager.hpp"

//========3rd Party Includes============================================================//
#include <Vector3.hpp>
#include <Vector2.hpp>

#include "GLTypes.hpp"

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//


////////==========================================================================////////
////////    Class                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: mesh
\brief      The base class for all mesh types.
*****************************************************************************************/
class mesh
{
 

    ///////========================================================================///////
    ///////   Public                                                               ///////
    ///////========================================================================///////
public:
     

		//==================================================================================//
		//        Constructor                                                                //
		//==================================================================================//

		mesh(const std::string p_assetPath) : m_label(p_assetPath){}

		/*!***************************************************************************************
		\brief Default mesh constructor
		*****************************************************************************************/
		mesh() = default;

		/*!***************************************************************************************
		\brief Copy constructor
		\param p_other other mesh to copy
		*****************************************************************************************/
		mesh(const mesh& p_other);

    //==================================================================================//
    //        Destructor                                                                //
    //==================================================================================//

    /*!***********************************************************************************
    \brief  destructor (default)
    *************************************************************************************/
    virtual ~mesh() = default;

    ////////======================================================================////////
    ////////  struct                                                              ////////
    ////////======================================================================////////

    /*!***********************************************************************************
    \par struct: vertex
    \brief       holds data about a single vertice
    *************************************************************************************/
#pragma pack(push, 1) 
    struct vertex
    {
        //==============================================================================//
        //        Constructor                                                           //
        //==============================================================================//

        vector3 m_position = { 0,0,0 };       //!< layout(location = 0) position of the vertex
        vector3 m_normal = { 0,0,0 };         //!< layout(location = 1) normal of the vertex
        vector2 m_uv = { 0,0 };               //!< layout(location = 2) in vec2 vUV;           
        vector3 m_tangent = { 0, 0, 0 };      //!< layout(location = 3) in vec3 vTangent;    
        vector3 m_bitangent = { 0, 0, 0 };    //!< layout(location = 4) in vec3 vBitangent;  

        /*!*******************************************************************************
        \brief  constructor(default)
        *********************************************************************************/
        vertex() = default;

        /*!*******************************************************************************
        \brief  parameterized constructor
        \param  p_position - the model position of the vertex
        *********************************************************************************/
        vertex(vector3 const& p_position): m_position(p_position){}

    };

		//======================================================================================//
		//        Operators                                                                     //
		//======================================================================================//

		mesh& operator=(const mesh& p_other);

#pragma pack(pop)
   
    //==================================================================================//
    //        Getters & Setters                                                         //
    //==================================================================================//
    // 
    /*!***************************************************************************************
    \brief  gets how many layout locations there are
    \return the number of layouts
    *****************************************************************************************/
    size_t getAttributeCount() const { return 3; }

    /*!***************************************************************************************
    \brief   get the count for each attribute
    \return  a vector where each element hold the quanity of data in a layout
    *****************************************************************************************/
    std::vector<size_t> getAttributeElementCounts() const { return { 3,3,2 }; }

    /*!***************************************************************************************
    \brief  get the size of each attribute
    \return a vector that hold each attributes data size
    *****************************************************************************************/
    std::vector<size_t> getAttributeElementSize() const;
   
   /*!***************************************************************************************
   \brief  gets a vertex
   \param  p_index - the index of the vertex
   \return a const vertex
   *****************************************************************************************/
    mesh::vertex const& getVertex(int p_index) const;

    /*!***************************************************************************************
    \brief  set the name of the mesh
    \param  p_name - the name of the mesh
    *****************************************************************************************/
    void setLabel(std::string const& p_name) { m_label = p_name; }

    /////============================================================================/////
    /////     Functions                                                              /////
    /////============================================================================/////

    /*!***************************************************************************************
    \brief  Adds a vertex to the mesh 
    \param  p_x - the x value
    \param  p_y - the y value
    \param  p_z - the z value
    *****************************************************************************************/
    void addVertexToMesh(float p_x, float p_y, float p_z);

    /*!***************************************************************************************
    \brief  load in all data into buffers
    *****************************************************************************************/
    void preProcess();
    
    /*!***************************************************************************************
    \brief  the size of a vertice
    \return the size of the vertex struct
    *****************************************************************************************/
    size_t getVertexSize() const;

    /*!***************************************************************************************
    \brief  get all the vertices 
    \return a vector holding all vertice data
    *****************************************************************************************/
    std::vector<vertex> getVertexData() { return m_vertices; }

    /*!***************************************************************************************
    \brief  how many vertices there are
    \return vertex count
    *****************************************************************************************/
    GLint getVertCount() const;


    ///////========================================================================///////
    ///////   Protected                                                            ///////
    ///////========================================================================///////
protected:

    /////============================================================================/////
    /////     Data                                                                   /////
    /////============================================================================/////

    std::vector<vertex>     m_vertices; //!< The vectors of all the vertices
    std::string             m_label;    //!< The name of the mesh
};





