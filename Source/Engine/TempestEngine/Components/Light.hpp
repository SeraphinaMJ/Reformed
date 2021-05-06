/*!***************************************************************************************
\file       Light.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "../Rendering/TypeData.hpp"
#include "Renderer.hpp"
#include <Component.hpp>
#include <Vector3.hpp>
#include <MathConstants.hpp>
#include <Color.hpp>
#include "../EngineInterfaces/Public/IComponentRepository.hpp"

class cameraBase;

class light : public componentCRTP<light>
{

public:
	friend class graphicsSystem;

	/*!***************************************************************************************
	\brief  Converts this into a type
	\return Type equivalent.
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Create a default implementation of a light
	*****************************************************************************************/
	light() = default;

	/*!***************************************************************************************
	\brief  Constructor for a light component
	\param  type - type of the primitive
	*****************************************************************************************/
	light(lighttype p_type, color p_diffuseColor) : /*m_direction(p_direction),*/ m_lightType(p_type), m_diffuseColor(p_diffuseColor) {}

	/*!***************************************************************************************
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	light(typeRT& p_type);

	/*!***************************************************************************************
	\brief  Gets component type
	\return The component type
	*****************************************************************************************/
	componentType const type() const override { return getType(); }

	/*!***************************************************************************************
	\brief  Gets component type
	\return The component type
	*****************************************************************************************/
	static componentType const getType() { static componentType const type = componentType::forConcrete<light>(); return type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Light", true, true, false); }

	void updateFromTypeRT(typeRT& p_type) override;

	void SetLightUniform(int p_index, std::shared_ptr<shaderProgram> p_program, componentHandle<cameraBase> cam);
	static void SetGlobalAmbient(std::shared_ptr<shaderProgram> p_program);

	void setLightType(lighttype p_type) { m_lightType = p_type; }
	void setIntensity(float p_intensity) { m_intensity = p_intensity; }
	void setDiffuseColor(color p_diffuseColor) { m_diffuseColor = p_diffuseColor; }

private:

	bool m_isActive = true;
	bool m_castShadow = false;

	lighttype m_lightType = lighttype::enm_directional;
	float m_intensity = 2.0f;
	float m_radius = 10.0f;
	float m_innerAngle = math::c_Pi / 12.0f;//15 degree
	float m_outerAngle = math::c_Pi / 6.0f;//30 degree
	float m_spotFalloff = 2.0f;

	//distance attenuation will only be set when this is true.
	bool m_ifDecay = true;
	vector3 m_disAtten = { 1.0f,0.0014f, 0.000007f };//constant, linear, quadratic

	color m_diffuseColor = color(1, 1, 1);
	color m_ambientColor = color(0, 0, 0);
	color m_specularColor = color(1, 1, 1);

	static color s_globalAmbient;
};
