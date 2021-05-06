/*!**********************************************************************************************************
\file       GeneralizedFileLoader.cpp
\author     Henry Brobeck
\date       1/30/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/

#include "GeneralizedFileLoader.hpp"
#include <fstream>

void generalizedFileLoader::assetsFrom(const std::filesystem::path& p_base, const std::wstring& p_filePath, std::vector<asset*>& p_assets)
{
	//assume filename exists
	p_assets.emplace_back(new asset(p_filePath, std::filesystem::relative(p_filePath, p_base).generic_wstring(), this));
}

void generalizedFileLoader::loadAsset(asset& p_asset)
{
	auto& path = p_asset.getFullPath();
	std::vector<char> fileData;

	std::ifstream infile(path, std::ios::binary | std::ios::ate);;
	//infile.open(path);
	if(!infile.is_open())
	{
		throw std::runtime_error("Failed to open generic asset");
	}
	//infile.seekg(0, std::ios_base::end);
	std::streampos fileSize = infile.tellg();
	fileData.resize(fileSize);
	infile.seekg(0, std::ios_base::beg);
	infile.read(&fileData[0], fileSize);
	

	auto data = new genericAssetData(p_asset.getExtention(), fileData);
	p_asset.setData(data);

}

void generalizedFileLoader::unloadAsset(asset& p_asset)
{
	delete &p_asset.getData<genericAssetData>();
	p_asset.setData(nullptr);
}
