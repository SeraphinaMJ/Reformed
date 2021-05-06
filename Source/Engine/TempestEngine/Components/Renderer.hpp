/*!***************************************************************************************
\file       Renderer.hpp
\author     Cannell, Cody
\date       5/21/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Renders the object
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Rendering/Texture.hpp"
#include "../Rendering/TextureManager.hpp"
#include "../Rendering/TypeData.hpp"
#include "../Rendering/ShaderProgram.hpp"
#include "../Rendering/Material.hpp"


//========3rd Party Includes============================================================//
#include <Component.hpp>
#include <stdexcept>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

class textureManager;
class programPipeline;
class assetHandle;

struct texturePack
{
	std::string m_diffuseMapName;
	bool m_hasDiffuseMap = false;

	std::string m_ambiantOcclussionMapName;
	bool m_hasAmbiantOcclussionMap = false;

	std::string m_specularMapName;
	bool m_hasSpecularMap = false;

	std::string m_normalMapName;
	bool m_hasNormalMap = false;

	std::string m_heightMapName;
	bool m_hasHeightMap = false;

	/* Possible other maps imported by blender/maya */
	//std::string m_glossMapName;
	//std::shared_ptr<texture> m_glossTexture;
	//bool m_hasGlossMap;

	//std::string m_selfIlluminationMapName;
	//std::shared_ptr<texture> m_selfIlluminationTexture;
	//bool m_hasSelfIlluminationMap;

	//std::string m_reflectionMapName;
	//std::shared_ptr<texture> m_reflectionTexture;
	//bool m_hasReflectionMap;

};

/*!*************************************************************************************
\par class: Render.hpp
\brief   This will render the object on the screen
***************************************************************************************/
class renderer : public componentCRTP<renderer>
{

	///////============================================================================///////
	///////   public                                                                   ///////
	///////============================================================================///////
public:

	//==================================================================================//
	//        Constructor                                                               //
	//==================================================================================//
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Create a default implementation of a renderer component
	*****************************************************************************************/
	renderer() = default;

	/*!***************************************************************************************
	\brief  Constructor for a render component
	\param  type - type of the primitive
	*****************************************************************************************/
	renderer(std::string p_primitiveType, programType p_programType, std::string p_materialName) : m_programType(p_programType), m_materialName(p_materialName), m_modelName(p_primitiveType), m_hasMaterial(true) {}

	renderer(std::string p_primitiveType, programType p_programType) : m_programType(p_programType), m_modelName(p_primitiveType) {}

	/*!***************************************************************************************programPipeline
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	renderer(typeRT& p_type); 

	/*!***************************************************************************************
	  \brief  sending model matrix uniform to shaders
	  \param  program - used to send uniforms to shader
	  *****************************************************************************************/
	void setRenderUniforms(std::shared_ptr<shaderProgram> p_program);
	void setRenderUniforms(std::shared_ptr<programPipeline> p_program);
	/*!***************************************************************************************
	\brief  Gets component type
	\return The component type
	*****************************************************************************************/
	componentType const type() const override { return getType(); }

	/*!***************************************************************************************
	\brief  Gets component type
	\return The component type
	*****************************************************************************************/
	static componentType const getType() { static componentType const type = componentType::forConcrete<renderer>(); return type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Renderer", true, true, false); }

	void updateFromTypeRT(typeRT& p_type) override;

	std::filesystem::path getMaterialPath() const;

	void setMaterialName(std::string p_materialName)
	{
		m_materialName = p_materialName;
		m_hasMaterial = true;
	}
	//==============================================//


	/*!***************************************************************************************
	\brief Gets Render type
	\return Render type by primitive
	*****************************************************************************************/
	std::string const& GetRenderType() const { return m_modelName; }

	programType GetProgramType() const { return m_programType; }

	std::string const& getMaterialName() const { return m_materialName; }

	// TODO: why does this throw? 
	color getColor() const { throw std::runtime_error("Color has not been implemented"); }

	bool hasMaterial() const { return m_hasMaterial; }

	void setMaterialUniforms(std::shared_ptr<programPipeline> p_program, material const&  p_materialManager, textureManager * p_textureManager) const;


	void setColor(const color p_color) { if (p_color.x) {}throw std::runtime_error("Color has not been implemented"); }

	void setMaterialUniforms(std::shared_ptr<shaderProgram> p_program, material const& p_material, textureManager * p_textureManager) const;
	void unbindTextures(material const& p_mat, textureManager * p_textureManager) const;
	void setPrimitiveType(std::string p_primitiveType) { m_modelName = p_primitiveType; }

	void setProgramType(programType p_program_type) { m_programType = p_program_type; }
	/////================================================================================/////
	/////     Data                                                                       /////
	/////================================================================================/////
private:

	//texturePack m_textureData;

	programType m_programType = programType::enm_forward;

	std::string m_fontName = "arial.ttf";
	std::string m_materialName = "basic.mtl";
	//std::string m_defaultMaterialName = m_materialName;  //this allows us to change the material then get back to the original one
	std::string m_modelName = "sampleCube.obj";
	std::string m_vertexShaderName = "shader.vert";
	std::string m_fragmentShaderName = "shader.frag";
	std::string m_geometryShaderName = "shader.geom";
	std::string m_tessellationControlShaderName = "shader.tcon";
	std::string m_tessellationEvaluationShaderName = "shader.tval";
	std::string m_computeShaderName = "shader.comp";

	bool m_hasMaterial = true;

};
