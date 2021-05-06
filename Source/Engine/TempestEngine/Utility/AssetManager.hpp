/*!**********************************************************************************************************
\file       AssetManager.hpp
\author     Henry Brobeck
\date       1/30/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <Logger.hpp>

class fileLoader;

class assetData
{
public:
	assetData() noexcept = default;
	virtual ~assetData() = default;
	/*!***************************************************************************************
	\brief Create a new type name
	\param p_type type string to set
	*****************************************************************************************/
	explicit assetData(std::wstring p_type) noexcept : m_type(std::move(p_type)) {};
	/*!***************************************************************************************
	\brief get the asset data type string
	\return the asset data type
	*****************************************************************************************/
	const std::wstring& getType() const noexcept { return m_type; }
protected:
	std::wstring m_type; //!assetData type
};

class asset
{
public:
	// asset(const std::wstring& p_fullName);
	/*!***************************************************************************************
	\brief Create a new asset
	\param p_fullName full file path of the asset
	\param p_name Given name of the asset
	\param p_loader Associated asset loader
	*****************************************************************************************/
	asset(const std::wstring& p_fullName, const std::wstring& p_name, fileLoader* p_loader);
	asset(const asset&) = delete;
	asset(asset&&) = delete;

	/*!***************************************************************************************
	\brief Gets the data inside the asset, loading if its not there
	\tparam T Type of data to get
	\return reference to data inside asset as given type
	*****************************************************************************************/
	template <typename T>
	const T& getData();
	void setData(assetData* p_data) { m_assetData = p_data; };

	/*!***************************************************************************************
	\brief gets the name of the assete
	\return the name of the asset
	*****************************************************************************************/
	const std::wstring& getName();
	/*!***************************************************************************************
	\brief get the file extention of the asset
	\return the  filetype extention of the asset
	*****************************************************************************************/
	const std::wstring& getExtention();
	/*!***************************************************************************************
	\brief get the path without the filename of the asset
	\return the path without the filename of the asset
	*****************************************************************************************/
	const std::wstring& getPath();
	/*!***************************************************************************************
	\brief Get the full file path of the asset
	\return The full filepath of the asset e.g. "C://.../asset.file"
	*****************************************************************************************/
	const std::filesystem::path& getFullPath();
	/*!***************************************************************************************
	\brief Check if the asset has stored data, ie the asset is loaded
	\return true if there is data
	*****************************************************************************************/
	bool hasData();
	/*!***************************************************************************************
	\brief Gets the loader associated with the asset
	\return A loader
	*****************************************************************************************/
	fileLoader* getLoader();

	/*!***************************************************************************************
	\brief Gets the asset type string
	\return Asset type
	*****************************************************************************************/
	const std::wstring& getType();
	/*!***************************************************************************************
	\brief Sets the asset type
	\param p_type Asset type
	*****************************************************************************************/
	void setType(std::wstring p_type);

	/*!***************************************************************************************
	\brief Gets the number of references (alive assetHandles) to this asset
	\return The number of references to this asset
	*****************************************************************************************/
	int getRefs();


private:
	friend class assetHandle; //!<friend class for assethandle so assethandle can modify reference counts
	assetData* m_assetData; //!<Data this asset points to, may be nullptr if no asset is loaded
	int m_references; //!<number of references (alive assetHandles) to this asset
	std::wstring m_name; //!<name of this asset
	std::wstring m_extention; //!< filename extention of this asset
	std::wstring m_path; //!< path (not including filename) of this asseet
	std::filesystem::path m_fullPath; //!< path including filename of this asset
	fileLoader* m_loader; //!< loader of this asset
	std::wstring m_type; //!< type string of this asset

};

/*!***************************************************************************************
\brief Asset handle, to facilitate flyweight pattern. Usable as an asset*
*****************************************************************************************/
class assetHandle
{
public:
	/*!***************************************************************************************
	\brief Create a new assetHandle
	\param p_parent Parent asset this is a handle for
	*****************************************************************************************/
	explicit assetHandle(asset* p_parent);
	/*!***************************************************************************************
	\brief assetHandle move constructor
	\param rhs assetHandle to take data from
	*****************************************************************************************/
	assetHandle(assetHandle&& rhs) noexcept;
	/*!***************************************************************************************
	\brief assetHandle copy constructor
	\param rhs assetHandle to copy from
	*****************************************************************************************/
	assetHandle(const assetHandle& rhs);
	/*!***************************************************************************************
	\brief assetHandle destructor
	*****************************************************************************************/
	~assetHandle();

	/*!***************************************************************************************
	\brief Assethandle arrow operator
	Has a side effect of loading the asset this handle points to if it was not already
	\return asset* to asset this is a handle for
	*****************************************************************************************/
	asset* operator->() const;

	/*!***************************************************************************************
	\brief returns the raw asset pointer
	\return asset* to asset this is a handle for
	*****************************************************************************************/
	asset* get() const;

	/*!***************************************************************************************
	\brief assetHandle star operator
	\par unsafe if asset is nullptr
	\return dereferenced asset* that this asset manges;
	*****************************************************************************************/
	asset& operator*() const;

	/*!***************************************************************************************
	\brief assetHandle copy =  operator
	\param rhs assetHandle to copy
	\return this assetHandle for chaining
	*****************************************************************************************/
	assetHandle& operator=(assetHandle const& rhs) noexcept;

	/*!***************************************************************************************
	\brief assetHandle move= operator
	\param rhs assetHandle to take from
	\return reference to this for chaining
	*****************************************************************************************/
	assetHandle& operator=(assetHandle&& rhs) noexcept;

	bool operator==(void* rhs) const noexcept;

	bool operator==(std::nullptr_t rhs) const noexcept;

	//! returns if handle contains an asset
	operator bool() const noexcept;

	void swap(assetHandle& rhs) noexcept;

	// friend version for STL to use
	friend void swap(assetHandle& lhs, assetHandle& rhs) noexcept;

private:
	friend class assetManager; //!< friend to allow for assetManager to mess with this
	asset* m_parent; //!< parent asset
	void incRef(); //!< increases reference, if there is a reference currently
	void decRef(); //!< decreases reference, if there is a reference currently
};


/*!***************************************************************************************
\brief abstract loader class, to allow for custom file loaders
*****************************************************************************************/
class fileLoader
{
public:
    virtual ~fileLoader() = default;

	/*!***************************************************************************************
	\brief Fill a list of all assets found in the filename
	\param base base path of the asset folder (names are the different between base and filename)
	\param p_filename full filename with path of the asset
	\param p_assets out: list of assets
	*****************************************************************************************/
	virtual void assetsFrom(const std::filesystem::path& base, const std::wstring& p_filename, std::vector<asset*>& p_assets) = 0;
	/*!***************************************************************************************
	\brief loads an asset and fills out its assetData
	\param p_asset asset to load
	*****************************************************************************************/
	virtual void loadAsset(asset& p_asset) = 0;
	/*!***************************************************************************************
	\brief unloads an asset's data and sets it to nullptr
	\param p_asset assete to unload
	*****************************************************************************************/
	virtual void unloadAsset(asset& p_asset) = 0;
};

/*!***************************************************************************************
\brief Manages assets, loading and unloading, with reference counting and flyweight
*****************************************************************************************/
class assetManager
{
public:
	/*!***************************************************************************************
	\brief initialize the asset manager, searches asset path for loadable assets
	*****************************************************************************************/
	static void init(const std::string & p_asset_path);
	/*!***************************************************************************************
	\brief shutdown the asset manager, unloads ALL ASSETS
	*****************************************************************************************/
	static void shutdown();
	/*!***************************************************************************************
	\brief loads an asset
	\param p_asset asset to load
	*****************************************************************************************/
	static void loadAsset(asset& p_asset);
	/*!***************************************************************************************
	\brief loads an asset
	\param p_assetHandle assetHandle that references an asset to load
	*****************************************************************************************/
	static void loadAsset(assetHandle& p_assetHandle);
	/*!***************************************************************************************
	\brief unloads an asset
	\param p_asset asset to unload
	*****************************************************************************************/
	static void unloadAsset(asset& p_asset);
	/*!***************************************************************************************
	\brief gets a list of all asset names in the asset directory and subdirectories
	\return wide string vector of asset names (with path relative to asset directory)
	*****************************************************************************************/
	static const std::vector<std::wstring>& assetList();

    /*!***************************************************************************************
    \brief reloads assets based on current asset path
    *****************************************************************************************/
    static void reloadAssetList();

    static std::vector<std::string> assetList(const std::string& filter);

	/*!***************************************************************************************
	\brief gets an assetHandle
	\param p_name name of asset
	\return assetHandle that points to asset with given name. may be nullptr
	*****************************************************************************************/
	static assetHandle getAsset(const std::filesystem::path& p_name);
	/*!***************************************************************************************
	\brief Converts a wide string to a standard width string
	\param p_convert wide string to convert
	\return result of conversion
	*****************************************************************************************/
	static std::string stringFromWide(const std::wstring& p_convert);
	/*!***************************************************************************************
	\brief converts a string to a wide string
	\param p_string string to convert
	\return wide string from given string
	*****************************************************************************************/
	static std::wstring wideFromString(const std::string& p_string);
	/*!***************************************************************************************
	\brief Unloads all assets with no references
	\return number of assets unloaded
	*****************************************************************************************/
	static int unloadUnreferenced();
	/*!***************************************************************************************
	\brief Unloads an asset with the given name if it has no references
	\param p_assetName name of the asset
	\return true if the asset was unloaded
	*****************************************************************************************/
	static bool unloadIfUnreferenced(const std::wstring& p_assetName);
protected:

private:
	/*!***************************************************************************************
	\brief searches the asset directory and populates the asset lists (recursive directory search)
	*****************************************************************************************/
	static void loadAssetList(const std::filesystem::path & p_asset_path);
	static std::map<std::wstring, asset*> m_assets; //!< asset name to asset pointer map
	static std::vector<std::wstring> m_assetNames; //!< list of all asset names
	static std::map<std::wstring, fileLoader*> m_typeToLoader; //!< filename extention to loader map
	static fileLoader* m_fallbackLoader; //!< uses this loader if no fileLoader matches an extention in the map
	static logger log; //!< logger

    static std::string m_asset_path;
};

//template functions
/*!***************************************************************************************
\brief Gets the assetData on an asset with the given type
\tparam T assetData type
\return reference to the assetData
*****************************************************************************************/
template <typename T>
const T& asset::getData()
{
	if(!m_assetData)
	{
		assetManager::loadAsset(*this);
	}
	const T* downAsset = dynamic_cast<T*>(m_assetData);
	return *downAsset;
}
// namespace <namespace name>

