#include "TextureLoader.hpp"
#include <stb_image.h>

#include <cstring>

void textureLoader::assetsFrom(const std::filesystem::path& p_base, const std::wstring& p_filePath,
	std::vector<asset*>& p_assets)
{
	p_assets.emplace_back(new asset(p_filePath, std::filesystem::relative(p_filePath, p_base).generic_wstring(), this));
}

void textureLoader::loadAsset(asset& p_asset)
{
	int l_width, l_height, l_bpp;

	auto& path = p_asset.getFullPath();


	//GLenum l_format = (m_format == FormatType::enm_rgb) ? STBI_rgb : STBI_rgb_alpha;

	stbi_set_flip_vertically_on_load(true);
	unsigned char * l_textureData = stbi_load(path.string().c_str(), &l_width, &l_height, &l_bpp, 0);

	if (!l_textureData)
	{
		 logger("SimpleTexture").debug() << "ERROR: stbi_load failed";
		return;
	}

	// make our buffer
	auto l_pixelData = new unsigned char[l_width* l_height* l_bpp];

	// copy data into our buffer
	std::memcpy(l_pixelData, l_textureData, l_width * l_height * l_bpp);


	auto data = new textureData(p_asset.getExtention(), l_pixelData);

	data->m_width = l_width;
	data->m_height = l_height;
	data->m_bpp = l_bpp;

	p_asset.setData(data);

	stbi_image_free(l_textureData);
}

void textureLoader::unloadAsset(asset& p_asset)
{
	delete &p_asset.getData<textureData>();
	p_asset.setData(nullptr);
}
