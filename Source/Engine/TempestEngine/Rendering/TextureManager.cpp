/*!***************************************************************************************
\file       TextureManager.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages all the textures on both the CPU and GPU
*****************************************************************************************/
#include "../Precompiled.h"
//========Self Include==================================================================//
#include "TextureManager.hpp"

//========1st Party Includes============================================================//

#include "SimpleTexture.hpp"

#include "TypeData.hpp"

#include "../Utility/TextureLoader.hpp"


//========3rd Party Includes============================================================//
#include <Logger.hpp>
#include <filesystem>
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

		//==============================================================================//
		//        Operators                                                             //
		//==============================================================================//

		//==============================================================================//
		//        Getters & Setters                                                     //
		//==============================================================================//

		// could do this better but just want to get it working
std::shared_ptr<texture> textureManager::getSimpleTexture(std::string const& p_fileName)
{

	auto l_texture = m_textures.find(p_fileName); // find texture in map

	if (l_texture == m_textures.end())
	{
		// not in map, add it
		std::filesystem::path l_path("textures");
		l_path /= p_fileName;
		auto asset = assetManager::getAsset(l_path);
		if (asset == nullptr) return std::nullptr_t{};
		return createSimpleTexture(asset);
		//logger("TextureManager").debug() << "Error: Simple Texture Not in Map: " << p_fileName;
		//return nullptr;
	}

	return l_texture->second;
}

std::shared_ptr<texture> textureManager::getCubeTexture(std::string const& p_fileName)
{
	auto l_texture = m_textures.find(p_fileName);

	if (l_texture == m_textures.end())
	{
		logger("TextureManager").debug() << "Error: Cube Texture Not in Map: " << p_fileName;
		return nullptr;
	}
	return l_texture->second;
}

void textureManager::UnbindAll()
{
	for (auto & t : m_textures)
	{
		t.second->unbind();
	}


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
std::shared_ptr<texture> textureManager::createSimpleTexture(assetHandle p_assetHandle)
{
	const auto l_filename = p_assetHandle->getFullPath().filename();
	const auto texturePtr = std::make_shared<simpleTexture>(std::move(p_assetHandle));
	texturePtr->build();
	m_textures.emplace(l_filename.generic_string(), texturePtr);
	return texturePtr;
}

void textureManager::deleteSimpleTexture(std::string const& p_name)
{
	m_textures.erase(p_name);
}

std::shared_ptr<texture> textureManager::createCubeTexture(std::string p_label, std::vector<assetHandle> p_assetHandles)
{
	const auto texturePtr = [&p_assetHandles]() -> std::shared_ptr<texture>
	{
		const auto filename = p_assetHandles.front()->getFullPath().filename();
		return std::make_shared<cubeTexture>(std::move(p_assetHandles), filename.generic_string());
	}();

	m_textures.emplace(std::move(p_label), texturePtr);

	return texturePtr;
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
