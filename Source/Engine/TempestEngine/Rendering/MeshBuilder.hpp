/*!***************************************************************************************
\file       MeshBuilder.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Static class that Builds all mesh types on the GPU and creates CPU objects.
*****************************************************************************************/

#pragma once

#include <memory>
#include <string>

enum class DefaultUvType;
//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
class triangleMesh;
class vertexArrayObject;
class shaderProgram;

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: meshBuilder(static)
\brief   builds all mesh types on the GPU and creates CPU objects
*****************************************************************************************/
class meshBuilder
{
	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////
public:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Builds a simple cube object
	\param  p_mesh - the mesh data 
	\param  p_vao  - the GPU data object
	\param  program  the GPU program that holds the data
	*****************************************************************************************/
    static void cubeBuilder(std::shared_ptr<triangleMesh> p_mesh,
                            std::shared_ptr<vertexArrayObject> p_vao);
							

    static std::shared_ptr<triangleMesh> buildFullScreenQuad(std::string const &meshLabel);

    static std::shared_ptr<triangleMesh> buildSphere(std::string const &meshLabel, DefaultUvType defaultUvType);

	//TODO(cody): create a line object builder
	/*!***************************************************************************************
	\brief  (Comming Soon) makes a line 
	*****************************************************************************************/
	static void lineBuilder();
};
