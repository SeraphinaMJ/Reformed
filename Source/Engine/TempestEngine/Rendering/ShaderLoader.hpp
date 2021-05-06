#pragma once
#include "Precompiled.h"
#include "AssetManager.hpp"
#include "TypeData.hpp"

#include <memory>
#include <string>
#include <vector>

class shaderData : public assetData
{
public:
	/*!***************************************************************************************
	\brief non-default constructor that initializes all members.
	\param p_type    - the type of shader this holds
	\param p_source  - the source code of the shader (more or less, see m_source for details)
	\param p_depends - other shader assets this shader depends on 
	*****************************************************************************************/
	shaderData(const shaderType p_type, std::string p_source, std::vector<assetHandle> p_depends) noexcept
		: assetData{}
		, m_shaderType{p_type}
		, m_source{std::move(p_source)}
		, m_depends{std::move(p_depends)}
	{}

	shaderData() noexcept = default;

	// asset dats should really only be moved, not copied
	shaderData(shaderData const&) = delete;
	shaderData& operator=(shaderData const&) = delete;

	shaderData& operator=(shaderData&&) = default;
	shaderData(shaderData&&) = default;
	~shaderData() override = default;

	/*!*********************************************************
	if shader type is not of "enm_glsl", the full source
	otherwise only contains source without dependencies emitted.
	***********************************************************/
	std::string const& getSource() const noexcept { return m_source; };
	shaderType getShaderType() const noexcept { return m_shaderType; };

	//! gets this shader's total dependencies, in reverse emission order
	std::vector<assetHandle> const& getDependencies() const noexcept { return m_depends; }

private:
	friend class shaderLoader;

	//! informs what kind of shader this asset is
	shaderType m_shaderType = shaderType::enm_none;

	/*!************************************************************************
	source of the shader, stripped of comments. Includable shaders (.glsl) 
	do not have recursive includes emitted. All other shader types defined in
	shaderType have their full code emitted and are ready for compilation
	***************************************************************************/
	std::string m_source;

	/*!************************************************************************
	include shaders (.glsl) that must be present for this shader to function
	without good reason otherwise, only the shaderLoader may know about this 
	***************************************************************************/
	std::vector<assetHandle> m_depends;
};

class shaderLoader : public fileLoader
{
public:

	/*!***************************************************************************************
	\brief Fill a list of all assets found in the filename
	\param base base path of the asset folder (names are the different between base and filename)
	\param p_filename full filename with path of the asset
	\param p_assets out: list of assets
	*****************************************************************************************/
	void assetsFrom(const std::filesystem::path& p_base, const std::wstring& p_filename, std::vector<asset*>& p_assets) override;

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
