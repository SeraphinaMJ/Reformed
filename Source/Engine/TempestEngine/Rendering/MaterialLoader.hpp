#pragma once
#include "Precompiled.h"
#include "AssetManager.hpp"
#include "Material.hpp"

class materialData : public assetData
{
public:
	materialData(std::wstring p_type, material mat);

	material const& getData() const noexcept;
private:
	friend class materialLoader;
	material m_mat;
};

class materialLoader : public fileLoader
{
public:

	/*!***************************************************************************************
	\brief Fill a list of all assets found in the filename
	\param base base path of the asset folder (names are the different between base and filename)
	\param p_filename full filename with path of the asset
	\param p_assets out: list of assets
	*****************************************************************************************/
	 void assetsFrom(const std::filesystem::path& base, const std::wstring& p_filename, std::vector<asset*>& p_assets) override;
	/*!***************************************************************************************
	\brief loads an asset and fills out its assetData
	\param p_asset asset to load
	*****************************************************************************************/
	 void loadAsset(asset& p_asset) override;
	/*!***************************************************************************************
	\brief unloads an asset's data and sets it to nullptr
	\param p_asset assete to unload
	*****************************************************************************************/
	void unloadAsset(asset& p_asset) override;
};
