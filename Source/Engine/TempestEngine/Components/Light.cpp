/*!***************************************************************************************
\file       Light.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "Light.hpp"
#include "Camera.hpp"
#include "../SceneManagement/Transform.hpp"
#include <Vector3Serialization.hpp>
#include <ColorSerialization.hpp>


color light::s_globalAmbient = color(0,0,0);

typeRT light::toTypeRT() const
{

    typeRT result;
    result.setTypeName(getType().name());
    result.setVariableName(getType().name());
	std::vector<typeRT> members;

    typeRT isActive("isActive", static_cast<bool>(m_isActive));
	members.push_back(isActive);
    typeRT castShadow("castShadow", static_cast<bool>(m_castShadow));
	members.push_back(castShadow);


    typeRT lightType("lightType", static_cast<int>(m_lightType));
	members.push_back(lightType);

    typeRT intensity("intensity", static_cast<float>(m_intensity));
	members.push_back(intensity);
    typeRT radius("radius", static_cast<float>(m_radius));
	members.push_back(radius);
    typeRT innerAngle("innerAngle", static_cast<float>(m_innerAngle));
	members.push_back(innerAngle);
    typeRT outerAngle("outerAngle", static_cast<float>(m_outerAngle));
	members.push_back(outerAngle);
    typeRT spotFalloff("spotFalloff", static_cast<float>(m_spotFalloff));
	members.push_back(spotFalloff);

   

    typeRT ifDecay("ifDecay", static_cast<bool>(m_ifDecay));
	members.push_back(ifDecay);

    typeRT disAtten = ::toTypeRT(m_disAtten);
    disAtten.setVariableName("disAtten");
	members.push_back(disAtten);

    typeRT diffuseColor = ::toTypeRT(m_diffuseColor);
    diffuseColor.setVariableName("diffuseColor");
	members.push_back(diffuseColor);

    typeRT ambientColor = ::toTypeRT(m_ambientColor);
    ambientColor.setVariableName("ambientColor");
	members.push_back(ambientColor);

    typeRT specularColor = ::toTypeRT(m_specularColor);
    specularColor.setVariableName("specularColor");
	members.push_back(specularColor);

    typeRT globalAmbient = ::toTypeRT(s_globalAmbient);
    globalAmbient.setVariableName("globalAmbient");
	members.push_back(globalAmbient);

	for (auto i_member : members)
	{
		result.insertMember(i_member);
	}

    return result;

}

light::light(typeRT & p_type)
{
	updateFromTypeRT(p_type);
}

template<typename T>
void fromTypeRT(typeRT& p_type, T& p_member, std::string const& name)
{
	if (auto&& i_found = p_type.members().find(name); i_found != p_type.members().end())
	{
		p_member = *reinterpret_cast<T*>(i_found->second.getRawData());
	}
}

template<>
void fromTypeRT(typeRT& p_type, lighttype& p_member, std::string const& name)
{
	if (auto&& i_found = p_type.members().find(name); i_found != p_type.members().end())
	{
		p_member = static_cast<lighttype>(i_found->second.getUInt());
	}
}

template<>
void fromTypeRT(typeRT& p_type, std::string& p_member, std::string const& name)
{
	if (auto&& i_found = p_type.members().find(name); i_found != p_type.members().end())
	{
		p_member = i_found->second.getString();
	}
}

template<>
void fromTypeRT(typeRT& p_type, vector3& p_member, std::string const& name)
{
	if (auto&& i_found = p_type.members().find(name); i_found != p_type.members().end())
	{
		p_member = ::toVector3(i_found->second);
	}
}

template<>
void fromTypeRT(typeRT& p_type, color& p_member, std::string const& name)
{
	if (auto&& i_found = p_type.members().find(name); i_found != p_type.members().end())
	{
		p_member = ::toColor(i_found->second);
	}
}

void light::updateFromTypeRT(typeRT& p_type)
{
	fromTypeRT(p_type, m_isActive     ,"isActive");
	fromTypeRT(p_type, m_castShadow   ,"castShadow");
	fromTypeRT(p_type, m_lightType    ,"lightType");
	fromTypeRT(p_type, m_intensity    ,"intensity");
	fromTypeRT(p_type, m_radius       ,"radius");
	fromTypeRT(p_type, m_innerAngle   ,"innerAngle");
	fromTypeRT(p_type, m_outerAngle   ,"outerAngle");
	fromTypeRT(p_type, m_spotFalloff  ,"spotFalloff");
	fromTypeRT(p_type, m_ifDecay      ,"ifDecay");
	fromTypeRT(p_type, m_disAtten     ,"disAtten");
	fromTypeRT(p_type, m_diffuseColor ,"diffuseColor");
	fromTypeRT(p_type, m_ambientColor ,"ambientColor");
	fromTypeRT(p_type, m_specularColor,"specularColor");
	fromTypeRT(p_type, s_globalAmbient,"globalAmbient");
}

void light::SetLightUniform(int p_index, std::shared_ptr<shaderProgram> p_program, componentHandle<cameraBase> /*p_camera*/) 
{
		static std::unordered_map<int /*index*/, std::unordered_map<std::string /*attribute*/, std::string /*uniform_name*/>> s_uniform_strings;
		if (!p_index) SetGlobalAmbient(p_program);

		// get the map of uniform strings (map is an optimization so we only do the concatenation once)
		auto l_iter = s_uniform_strings.find(p_index);
		if(l_iter == s_uniform_strings.end())
		{
			// build a map for this index
			std::unordered_map<std::string, std::string> l_map;
			std::stringstream str;
			str << "Lights[" << p_index << "].";

			l_map["ambient"] = str.str() + "ambient";
			l_map["diffuse"] = str.str() + "diffuse";
			l_map["specular"] = str.str() + "specular";
			l_map["isActive"] = str.str() + "isActive";
			l_map["direction"] = str.str() + "direction";
			l_map["position"] = str.str() + "position";
			l_map["type"] = str.str() + "type";
			l_map["intensity"] = str.str() + "intensity";
			l_map["ifDecay"] = str.str() + "ifDecay";
			l_map["disAtten"] = str.str() + "disAtten";
			l_map["theta"] = str.str() + "theta";
			l_map["phi"] = str.str() + "phi";
			l_map["falloff"] = str.str() + "falloff";
			l_map["disAtten"] = str.str() + "disAtten";
			s_uniform_strings[p_index] = l_map;
			l_iter = s_uniform_strings.find(p_index);
		}
		auto & l_uniform_map = l_iter->second;

    //colors
    p_program->setUniforms(l_uniform_map["ambient"], m_ambientColor);
    p_program->setUniforms(l_uniform_map["diffuse"], m_diffuseColor);
    p_program->setUniforms(l_uniform_map["specular"], m_specularColor);

    //other attribute      
	componentHandle<transform> l_transform = getGameObject().lock()->getComponent<transform>();
    p_program->setUniforms(l_uniform_map["isActive"], m_isActive);
    p_program->setUniforms(l_uniform_map["direction"], vector4(l_transform->getGlobalRotation().getForward(), 0.0f));
    p_program->setUniforms(l_uniform_map["position"], vector4(l_transform->getGlobalPosition(), 0.0f));
    p_program->setUniforms(l_uniform_map["type"], static_cast<int>(m_lightType));
    p_program->setUniforms(l_uniform_map["intensity"], m_intensity);
    p_program->setUniforms(l_uniform_map["ifDecay"], m_ifDecay);

    switch (m_lightType)
    {
    case lighttype::enm_directional:
        break;
    case lighttype::enm_point:
       
        p_program->setUniforms(l_uniform_map["disAtten"], m_disAtten);
        break;                 
    case lighttype::enm_spot:  
  
        p_program->setUniforms(l_uniform_map["theta"], m_innerAngle);
        p_program->setUniforms(l_uniform_map["phi"], m_outerAngle);
        p_program->setUniforms(l_uniform_map["falloff"], m_spotFalloff);
        p_program->setUniforms(l_uniform_map["disAtten"], m_disAtten);
        break;

    default:
        break;
    }
}

void light::SetGlobalAmbient(std::shared_ptr<shaderProgram> p_program)
{
	p_program->setUniforms("globalAmbient", s_globalAmbient);
}

