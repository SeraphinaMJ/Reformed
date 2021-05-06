/*!***************************************************************************************
\file       MaterialManager.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages all the materials on the GPU
*****************************************************************************************/
#include "../Precompiled.h"
//========Self Include==================================================================//
#include "MaterialManager.hpp"
#include "Material.hpp"
#include "../Utility/AssetManager.hpp"
#include "DataTypes.hpp"

#include <filesystem>

std::shared_ptr<material> materialManager::addMaterial(std::shared_ptr<material> p_material)
		{
        if(p_material->isLoaded())
        {
            m_materials.insert(std::make_pair(p_material->getFileName(), p_material));
        }

		    return getMaterial(p_material->getFileName());
		}

    std::shared_ptr<material> materialManager::getMaterial(std::string const& p_name)
		{
				auto i_iter = m_materials.find(p_name); // find if asset exists
				if(i_iter == m_materials.end())
				{
					// no asset, load it
					std::filesystem::path l_path("materials");
					l_path /= p_name;
					createMaterial(assetManager::getAsset(l_path));
					i_iter = m_materials.find(p_name);
				}
		    return  i_iter->second;
		}

		std::shared_ptr<material> materialManager::createMaterial(assetHandle p_assetHandle)
		{

				auto l_assetPath = assetManager::stringFromWide(p_assetHandle->getName());

				auto l_material = std::make_shared<material>();

				//l_material->loadInFile(p_assetHandle);

				// auto name = getFileNameFromAssetPath(l_assetPath);
				auto name = std::filesystem::path(p_assetHandle->getName()).filename().generic_string();

				return m_materials.emplace(name, l_material).first->second;
		}







