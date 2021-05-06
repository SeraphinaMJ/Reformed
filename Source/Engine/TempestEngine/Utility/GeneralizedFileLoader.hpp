/*!**********************************************************************************************************
\file       GeneralizedFileLoader.hpp
\author     Henry Brobeck
\date       1/30/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once
#include "AssetManager.hpp"

/*!***************************************************************************************
\brief Represents an assetdata that is simply an array of char bytes
*****************************************************************************************/
class genericAssetData : public assetData
{
public:
	/*!***************************************************************************************
	\brief Creates a new assetData with the given type and data
	\param p_type type of assetData (file extention)
	\param p_data data to store
	*****************************************************************************************/
	genericAssetData(const std::wstring& p_type, const std::vector<char>& p_data) : assetData(p_type), m_data(p_data) {}
	const std::vector<char>& getRaw() const { return m_data; };
;
private:
	friend class generalizedFileLoader; //friend for data management
	std::vector<char> m_data; //!<bytes of data in the file
};

/*!***************************************************************************************
\brief generic file loader, loads any file as binary data
*****************************************************************************************/
class generalizedFileLoader : public fileLoader
{
	
public:
	/*!***************************************************************************************
	\brief Gets the assets from the given file. For this loader, its always just the file
	\param p_base base filepath, ie: the asset directory
	\param p_filename filename of the asset
	\param p_assets out: list of assets (always just the given file)
	*****************************************************************************************/
	void assetsFrom(const std::filesystem::path& p_base, const std::wstring& p_filename, std::vector<asset*>& p_assets) override;
	/*!***************************************************************************************
	\brief loads the binary data of the asset
	\param p_asset asset to load
	*****************************************************************************************/
	void loadAsset(asset& p_asset) override;

	/*!***************************************************************************************
	\brief unloads an asset
	\param p_asset asset to unload
	*****************************************************************************************/
	void unloadAsset(asset& p_asset) override;
private:
	
};




