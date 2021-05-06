/*!***************************************************************************************
\file       ShaderManager.cpp
\author     Cannell, Cody
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages shaders and shader programes.
*****************************************************************************************/

#include "../Precompiled.h"
//========Self Include==================================================================//
#include "ShaderManager.hpp"
//========1st Party Includes============================================================//
#include "ShaderProgram.hpp"
#include "TypeData.hpp"
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
		shaderManager::~shaderManager()
		{
		    m_shaderPrograms.clear();
		}
		//==============================================================================//
		//        Getters & Setters                                                     //
		//==============================================================================//
		std::shared_ptr<shaderProgram> shaderManager::getShaderProgram(programType p_programType) const
		{
		    return m_shaderPrograms.find(p_programType)->second;
		}

		shader const *  shaderManager::getShader(programType p_programType, shaderType p_shaderType) const
		{
		    return m_shaderPrograms.find(p_programType)->second->getShader(p_shaderType);
		}

		void shaderManager::clearShaders()
		{
			m_shaderPrograms.clear();
			m_programPipeline.clear();
		}

		std::shared_ptr<programPipeline> shaderManager::getProgramPipeline(programType p_programType) const
		{
			return m_programPipeline.find(p_programType)->second;
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
		std::shared_ptr<shaderProgram> shaderManager::createShaderProgram(programType p_programType, assetHandle p_vertexName, assetHandle p_fragmentName)
		{
		    m_shaderPrograms.insert(std::make_pair(p_programType, std::make_shared<shaderProgram>(p_vertexName, p_fragmentName)));

		    return getShaderProgram(p_programType);

		}
		std::shared_ptr<programPipeline> shaderManager::createProgramPipelineObject( programType p_programType, assetHandle p_vertexName, assetHandle p_fragmentName)
		{
			m_programPipeline.insert(std::make_pair(p_programType, std::make_shared<programPipeline>(p_vertexName, p_fragmentName)));
			return getProgramPipeline(p_programType);
		}

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







// This is for creating multiple shader programes at the same time
//std::shared_ptr<ShaderProgram> meshManager::LoadShaderGroup(ProgramType type,
//    std::vector<std::pair<std::string, std::string>> const& shaderFiles)
//{
//    return nullptr;
//}









