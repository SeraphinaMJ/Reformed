#pragma once
#include "../Systems/FBXLoader.hpp"

class textureData : public assetData
{
public:
	/*!***************************************************************************************
	\brief Creates a new assetData with the given type and data
	\param p_type type of assetData (file extention)
	\param p_data data to store
	*****************************************************************************************/
	textureData(const std::wstring& p_type, unsigned char * p_data) : assetData(p_type), m_data(p_data) {}
	unsigned char * getRaw() const { return m_data; };
	unsigned m_width = 0;
	unsigned m_height = 0;
	unsigned m_bpp = 0;
	
private:
	friend class textureLoader; //friend for data management
	unsigned char * m_data = NULL; //!<bytes of data in the file


};


class textureLoader : public fileLoader
{
public:
	void assetsFrom(const std::filesystem::path& base, const std::wstring& p_filePath,
		std::vector<asset*>& p_assets) override;

	void loadAsset(asset& p_asset) override;
	void unloadAsset(asset& p_asset) override;
};
