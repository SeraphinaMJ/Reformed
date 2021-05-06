#include "../Precompiled.h"

#include "Skybox.hpp"

#include "../Rendering/ProgramPipeline.hpp"

#include "../SceneManagement/Transform.hpp"

#include "Camera.hpp"


typeRT skybox::toTypeRT() const
{
    typeRT result;
    result.setTypeName(getType().name());
    result.setVariableName(getType().name());
	std::vector<typeRT> members;

    typeRT primitiveType("primitiveType", static_cast<int>(m_primitiveType));
	members.push_back(primitiveType);
    typeRT programType("programType", static_cast<int>(m_programType));
	members.push_back(programType);
    typeRT textureName("textureName", m_textureName);
	members.push_back(textureName);

	for (auto i_member : members)
	{
		result.insertMember(i_member);
	}

    return result;
}

skybox::skybox(typeRT & p_type)
{
	m_primitiveType = static_cast<primitiveType>(p_type.member("primitiveType").getUInt());
	m_programType = static_cast<programType>(p_type.member("programType").getUInt());
	m_textureName = (p_type.member("textureName").getString());
}

void skybox::onRender(std::shared_ptr<shaderProgram> p_program,
	material const& p_mat, std::shared_ptr<texture> p_texture)
{
    p_texture->bind(0);
    p_program->setUniforms("cubeTexture", p_texture->getBoundSlot());
}

void skybox::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("primitiveType") != l_members.end())
	{
		m_primitiveType = static_cast<primitiveType>(p_type.member("primitiveType").getUInt());
	}
	if (l_members.find("programType") != l_members.end())
	{
		m_programType = static_cast<programType>(p_type.member("programType").getUInt());
	}
	if (l_members.find("textureName") != l_members.end())
	{
		m_textureName = p_type.member("textureName").getString();
	}

}


//==========================programable Pipeline Funcitons

void skybox::onRender(std::shared_ptr<programPipeline> p_program,
	material const& p_mat, std::shared_ptr<texture> p_texture)
{



	auto objTransform = getGameObject().lock()->getComponent<transform>();
	matrix4x4 modelMatrix = objTransform->getGlobalMatrix();



	//materials 

	p_program->setUniforms(2, "cubeTexture", p_texture->getBoundSlot());



	p_program->setUniforms(1, "ModelMatrix", modelMatrix);

}