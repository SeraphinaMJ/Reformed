/*!***************************************************************************************
\file       Renderer.cpp
\author     Cody Cannell
\date       5/21/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Renders the object
*****************************************************************************************/

//========Self Include==================================================================//
#include "Renderer.hpp"
//========1st Party Includes============================================================//
#include "../SceneManagement/GameObject.hpp"
#include "../SceneManagement/Transform.hpp"
#include "Camera.hpp"
#include "../Rendering/GLDebug.hpp"
#include "../Rendering/GraphicsSystem.hpp"
#include "../Rendering/ShaderManager.hpp"
#include "../Rendering/ShaderProgram.hpp"



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

		renderer::renderer(typeRT & p_type)
		{
		
              updateFromTypeRT(p_type);
		}

		void renderer::updateFromTypeRT(typeRT& p_type)
		{
			auto l_members = p_type.members();

			if (l_members.find("fontName") != l_members.end())
			{
				m_fontName = p_type.member("fontName").getString();
			}
			if (l_members.find("materialName") != l_members.end())
			{
				m_materialName = p_type.member("materialName").getString();
			}
			if (l_members.find("modelName") != l_members.end())
			{
				m_modelName = p_type.member("modelName").getString();
			}
			if (l_members.find("vertexShaderName") != l_members.end())
			{
				m_vertexShaderName = p_type.member("vertexShaderName").getString();
			}
			if (l_members.find("fragmentShaderName") != l_members.end())
			{
				m_fragmentShaderName = p_type.member("fragmentShaderName").getString();
			}
			if (l_members.find("geometryShaderName") != l_members.end())
			{
				m_geometryShaderName = p_type.member("geometryShaderName").getString();
			}
			if (l_members.find("tessellationControlShaderName") != l_members.end())
			{
				m_tessellationControlShaderName = p_type.member("tessellationControlShaderName").getString();
			}
			if (l_members.find("tessellationEvaluationShaderName") != l_members.end())
			{
				m_tessellationEvaluationShaderName = p_type.member("tessellationEvaluationShaderName").getString();
			}
			if (l_members.find("computeShaderName") != l_members.end())
			{
				m_computeShaderName = p_type.member("computeShaderName").getString();
			}
			if(l_members.find("hasMaterial") != l_members.end())
			{
				m_hasMaterial = p_type.member("hasMaterial").getBool();
			}
		}

		std::filesystem::path renderer::getMaterialPath() const
		{
			return std::filesystem::path{"materials"} / m_materialName;
		}

    void renderer::setRenderUniforms(std::shared_ptr<shaderProgram> p_program)
    {
        auto objTransform = getGameObject().lock()->getComponent<transform>();
        matrix4x4 modelMatrix = objTransform->getGlobalMatrix();

        //materials     
        p_program->setUniforms("ModelMatrix", modelMatrix);
    }

		void renderer::setRenderUniforms(std::shared_ptr<programPipeline> p_program)
		{
			auto objTransform = getGameObject().lock()->getComponent<transform>();
			matrix4x4 modelMatrix = objTransform->getGlobalMatrix();
			p_program->setActiveProgram(1);
			//materials     
			p_program->setUniforms(0,"ModelMatrix", modelMatrix);
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
    typeRT renderer::toTypeRT() const
    {
        typeRT result;
        result.setTypeName(getType().name());
        result.setVariableName(getType().name());
        std::vector<typeRT> members;
        typeRT fontName("fontName", m_fontName);
        members.push_back(fontName);

        typeRT materialName("materialName", m_materialName);
        members.push_back(materialName);

        typeRT modelName("modelName", m_modelName);
        members.push_back(modelName);

        typeRT vertexShaderName("vertexShaderName", m_vertexShaderName);
        members.push_back(vertexShaderName);

        typeRT fragmentShaderName("fragmentShaderName", m_fragmentShaderName);
        members.push_back(fragmentShaderName);

        typeRT geometryShaderName("geometryShaderName", m_geometryShaderName);
        members.push_back(geometryShaderName);

        typeRT tessellationControlShaderName("tessellationControlShaderName", m_tessellationControlShaderName);
        members.push_back(tessellationControlShaderName);

        typeRT tessellationEvaluationShaderName("tessellationEvaluationShaderName", m_tessellationEvaluationShaderName);
        members.push_back(tessellationEvaluationShaderName);

        typeRT computeShaderName("computeShaderName", m_computeShaderName);
        members.push_back(computeShaderName);

				typeRT hasMaterial("hasMaterial", m_hasMaterial);
				members.push_back(hasMaterial);

        for (auto i_member : members)
        {
            result.insertMember(i_member);
        }

        return result;
    }



void renderer::setMaterialUniforms(std::shared_ptr<programPipeline> p_program, material const& p_material, textureManager * p_textureManager) const
{
    
    p_program->setUniforms(2,"skybox", false);

    //send all the material data
    p_program->setUniforms(2,"material.ambientColor", p_material.getAmbientColor());
    p_program->setUniforms(2,"material.diffuseColor", p_material.getDiffuseColor());
    p_program->setUniforms(2,"material.emissiveColor", p_material.getEmissiveColor());
    p_program->setUniforms(2,"material.specularColor", p_material.getSpecularColor());
    p_program->setUniforms(2,"material.specularExponent", p_material.getSpecularExponent());

    // send all matireal texture data
    if (p_material.hasDiffuseMap())
    {
       // get and bind texture
       auto l_texture = p_textureManager->getSimpleTexture(p_material.getDiffuseMapName());
       l_texture->bind(1);

       // set uniform data
       p_program->setUniforms(2,"material.hasDiffuseTexture", true);
       p_program->setUniforms(2,"diffuseTexture", l_texture->getBoundSlot());

    }
    else
    {
        p_program->setUniforms(2,"material.hasDiffuseTexture", false);
    }

    if (p_material.hasSpecularMap())
    {
        // get and bind texture
        auto l_texture = p_textureManager->getSimpleTexture(p_material.getSpecularMapName());
        l_texture->bind(2);

        // set uniform data
        p_program->setUniforms(2,"material.hasSpecularTexture", true);
        p_program->setUniforms(2,"specularTexture", l_texture->getBoundSlot());
    }
    else
    {
        p_program->setUniforms(2,"material.hasSpecularTexture", false);
    }
       

    if (p_material.hasNormalMap())
    {
        // get and bind texture
        auto l_texture = p_textureManager->getSimpleTexture(p_material.getNormalMapName());
        l_texture->bind(3);

        // set uniform data
        p_program->setUniforms(2,"material.hasNormalTexture", true);
        p_program->setUniforms(2,"normalTexture", l_texture->getBoundSlot());
    }
    else
    {
        p_program->setUniforms(2,"material.hasNormalTexture", false);
    }
}

void renderer::setMaterialUniforms(std::shared_ptr<shaderProgram> p_program, material const& p_material, textureManager * p_textureManager) const
{

	p_program->setUniforms( "skybox", false);

	//send all the material data
	p_program->setUniforms( "material.ambientColor", p_material.getAmbientColor());
	p_program->setUniforms( "material.diffuseColor", p_material.getDiffuseColor());
	p_program->setUniforms( "material.emissiveColor", p_material.getEmissiveColor());
	p_program->setUniforms( "material.specularColor", p_material.getSpecularColor());
	p_program->setUniforms( "material.specularExponent", p_material.getSpecularExponent());

	// send all matireal texture data
	if (p_material.hasDiffuseMap())
	{
		// get and bind texture
		auto l_texture = p_textureManager->getSimpleTexture(p_material.getDiffuseMapName());
		l_texture->bind(1);

		// set uniform data
		p_program->setUniforms("material.hasDiffuseTexture", true);
		p_program->setUniforms("diffuseTexture", l_texture->getBoundSlot());

	}
	else
	{
		p_program->setUniforms("material.hasDiffuseTexture", false);
	}

	if (p_material.hasSpecularMap())
	{
		// get and bind texture
		auto l_texture = p_textureManager->getSimpleTexture(p_material.getSpecularMapName());
		l_texture->bind(2);

		// set uniform data
		p_program->setUniforms("material.hasSpecularTexture", true);
		p_program->setUniforms("specularTexture", l_texture->getBoundSlot());
	}
	else
	{
		p_program->setUniforms( "material.hasSpecularTexture", false);
	}


	if (p_material.hasNormalMap())
	{
		// get and bind texture
		auto l_texture = p_textureManager->getSimpleTexture(p_material.getNormalMapName());
		l_texture->bind(3);

		// set uniform data
		p_program->setUniforms( "material.hasNormalTexture", true);
		p_program->setUniforms("normalTexture", l_texture->getBoundSlot());
	}
	else
	{
		p_program->setUniforms( "material.hasNormalTexture", false);
	}
}

void renderer::unbindTextures(material const& p_mat, textureManager * p_textureManager) const
{
	if (p_mat.hasNormalMap())
	{
		p_textureManager->getSimpleTexture(p_mat.getNormalMapName())->unbind();
	}

	if (p_mat.hasSpecularMap())
	{
		p_textureManager->getSimpleTexture(p_mat.getSpecularMapName())->unbind();
	}

	if (p_mat.hasDiffuseMap())
	{
		p_textureManager->getSimpleTexture(p_mat.getDiffuseMapName())->unbind();
	}
}
