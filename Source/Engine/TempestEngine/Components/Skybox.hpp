/*!***************************************************************************************
\file       Skybox.h
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Rendering/Material.hpp"

#include "../Rendering/ShaderProgram.hpp"

#include "../Rendering/Texture.hpp"

#include "../Rendering/TypeData.hpp"

#include "../SceneManagement/GameObject.hpp"

#include <Component.hpp>


class programPipeline;

class skybox : public componentCRTP<skybox>
{
public:

    /*!***************************************************************************************
     \brief  Converts this into a type
     \return Type equivalent.
     *****************************************************************************************/
    typeRT toTypeRT() const override;

		/*!***************************************************************************************
		\brief  Create a default implementation of a skybox component
		*****************************************************************************************/
		skybox() : skybox(primitiveType::enm_cube, programType::enm_forward, "sky_box_texture") {}

    /*!***************************************************************************************
    \brief  Constructor for a render component
    \param  type - type of the primitive
    *****************************************************************************************/
    skybox(primitiveType p_primitiveType, programType p_programType, std::string p_textureName) : m_primitiveType(p_primitiveType), m_programType(p_programType), m_textureName(p_textureName) {}

		/*!***************************************************************************************
		\brief  Converts type to transform.
		\param p_type - type to convert
		*****************************************************************************************/
		skybox(typeRT& p_type);

    /*!***************************************************************************************
    \brief  Renders the object by sending uniforms to shaders
    \param  camera  - needed to get view perjection matrix
    \param  program - used to send uniforms to shader
    \param  mat     -  used to send material information to shader
    *****************************************************************************************/
    void onRender(std::shared_ptr<shaderProgram> p_program, material const& p_mat, std::shared_ptr<texture> p_texture);

	void onRender(std::shared_ptr<programPipeline> p_program, material const& p_mat, std::shared_ptr<texture> p_texture);


    /*!***************************************************************************************
    \brief  Gets component type
    \return The component type
    *****************************************************************************************/
    componentType const type() const override { return getType(); }

    /*!***************************************************************************************
    \brief  Gets component type
    \return The component type
    *****************************************************************************************/
    static componentType const getType() { static componentType const type = componentType::forConcrete<skybox>(); return type; }
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Skybox", true, true, false); }

	void updateFromTypeRT(typeRT& p_type) override;

    /*!***************************************************************************************
    \brief  Gets the name of the texture
    \return The texture name
    *****************************************************************************************/
    std::string getTextureName() { return m_textureName; }

    /*!***************************************************************************************
    \brief Gets Render type
    \return Render type by primitive
    *****************************************************************************************/
    primitiveType GetRenderType() { return m_primitiveType; }

    programType GetProgramType() { return m_programType; }

private:

    primitiveType m_primitiveType = primitiveType::enm_cube; //!< 	 The type of the object
    programType m_programType = programType::enm_forward;
    std::string m_textureName = "sky_box_texture";

};
