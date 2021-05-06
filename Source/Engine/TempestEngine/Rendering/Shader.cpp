/*!***************************************************************************************
\file       shader.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This hold the CPU repersentation of the GPU shaders.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Shader.hpp"
//========1st Party Includes============================================================//
#include "../Utility/AssetManager.hpp"
#include "../Utility/GeneralizedFileLoader.hpp"
#include <string_view>
#include <optional>
#include <regex>

#include "ShaderLoader.hpp"



std::string getCompleteSource(assetHandle p_assetHandle)
{
	//get source data as string
	auto const& data = p_assetHandle->getData<shaderData>();

	constexpr auto c_shader_version = "#version 430 core\n";
	std::string source = c_shader_version;
	// emit code of dependencies
	for (auto&& dep : data.getDependencies())
	{
		source += dep->getData<shaderData>().getSource();
	}
	// emit code of ourselves
	source += data.getSource();

	return source;
}


shader::shader(assetHandle p_assetHandle) 
	: m_source{getCompleteSource(p_assetHandle)}
	, m_shaderName(assetManager::stringFromWide(p_assetHandle->getName()))
{

}

void shader::swap(shader& rhs) noexcept
{
	std::swap(m_handle, rhs.m_handle);
	std::swap(m_source, rhs.m_source);
	std::swap(m_shaderName, rhs.m_shaderName);
}

void swap(shader& lhs, shader& rhs) noexcept
{
	lhs.swap(rhs);
}
