/*!***************************************************************************************
\file       ShaderManager.cpp
\author     Cannell, Cody
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages shaders and shader programes.
*****************************************************************************************/
#pragma once
#include "ProgramPipeline.hpp"
#include "../Events/CollisionEvent.hpp"
//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "TypeData.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
class shader;
class VertexShader;
class fragmentShader;
class shaderProgram;


////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////



/*!***************************************************************************************
\par class: ShaderManager
\brief      Manages shaders and shader programes
*****************************************************************************************/
class shaderManager
{
	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

public:
	friend class graphicsSystem;

	//==================================================================================//
	//        Constructor                                                               //
	//==================================================================================//
	shaderManager() = default;
	/*!***************************************************************************************
	\brief  clears map
	*****************************************************************************************/
    ~shaderManager();
    //shaderManager(shaderManager const& p_shaderManager) = default;
            /*static std::shared_ptr<ShaderProgram> LoadShaderGroup(ProgramType type,
        std::vector<std::pair<std::string, std::string> >const& shaderFiles);*/

    /*!***************************************************************************************
    \brief  Creates a new shader programe
    \param  p_assetPath    - The path to the assets folder
    \param  p_type         - The type of object the program will build
    \param  p_VertexName   - The name of the vertex shader file
    \param  p_fragmentName - The name of the fragment shader file
    \return shared pointer to the newly constructed shader program
    *****************************************************************************************/
    std::shared_ptr<shaderProgram> createShaderProgram(programType p_programType, assetHandle p_vertexName, assetHandle p_fragmentName);
	std::shared_ptr<programPipeline> createProgramPipelineObject(programType p_programType, assetHandle p_vertexName, assetHandle p_fragmentName);

    /*!***************************************************************************************
    \brief  Gets a shader program from map
    \param  p_program -The shape program you want
    \return shared pointer to the shader program
    *****************************************************************************************/
    std::shared_ptr<shaderProgram> getShaderProgram(programType p_program) const;
	  std::shared_ptr<programPipeline> getProgramPipeline(programType p_programType) const;
    /*!***************************************************************************************
    \brief  Gets a shader from the program
    \param  p_program - The program type
    \param  p_type - The shader type
    \return pointer to the shader
    *****************************************************************************************/
    shader const * getShader(programType p_program, shaderType p_type) const;

	void clearShaders();

///////============================================================================///////
///////    Private                                                                 ///////
///////============================================================================///////

private:

	/////============================================================================/////
	/////    Data                                                                    /////
	/////============================================================================/////
   std::unordered_map<programType, std::shared_ptr<shaderProgram> > m_shaderPrograms; //!<  map of shader programs

   std::unordered_map<programType, std::shared_ptr<programPipeline> > m_programPipeline; //!<  map of shader programs

};
///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/




