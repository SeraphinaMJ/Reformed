/*!**********************************************************************************************************
\file       FBXLoader.hpp
\author     Henry Brobeck
\date       11/20/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			Loads FBX Files for models/animation/lights
************************************************************************************************************/
#pragma once

#include <fbxsdk.h>
#include <string>

#include <unordered_map>
#include "../Utility/AssetManager.hpp"
#include <Vector2.hpp>



struct vector3;
class FBXLoader;

/*!***************************************************************************************
\brief Storage and metadata for an FBX scene
*****************************************************************************************/
class FbxFileData
{
public:
	/*!***************************************************************************************
	\brief FbxFileData constructor
	\param p_scene FBX Scene to manage
	\param p_parent Parent loader
	*****************************************************************************************/
	FbxFileData(FbxScene* p_scene, FBXLoader& p_parent);
	/*!***************************************************************************************
	\brief Cleans up data, destroys scene
	*****************************************************************************************/
	~FbxFileData();
	
	/*!***************************************************************************************
	\brief Get the root node of the scene
	\return The root node of the FBX Scene
	*****************************************************************************************/
	FbxNode* getRoot();
	/*!***************************************************************************************
	\brief Get the scene
	\return Get scene pointer this handles
	*****************************************************************************************/
	FbxScene* getScene();
	/*!***************************************************************************************
	\brief Gets a node with the given name
	\param p_name Get node by name
	\return The node with the name given, null otherwise
	*****************************************************************************************/
	FbxNode* getNodeByName(const char* p_name);
	/*!***************************************************************************************
	\brief Gets a mesh by a given name
	\param p_name The name of the mesh
	\return the mesh with the given name, nullptr  if not found
	*****************************************************************************************/
	FbxMesh* getMeshByName(const char* p_name);

	/*!***************************************************************************************
	\brief Gets the list of vertices for the given mesh name
	\param p_mesh_name The name of the mesh to get verts of
	\return A vector of vertices in order
	*****************************************************************************************/
	std::vector<vector3> getNodeVerts(const char* p_mesh_name);
	/*!***************************************************************************************
	\brief Get the list of faces for the given mesh
	\param p_mesh_name Name of mesh to get faces of
	\return The list of faces, as indices of vertices list
	*****************************************************************************************/
	std::vector<size_t> getNodeFaces(const char* p_mesh_name);
	/*!***************************************************************************************
	\brief Gets mesh UVs
	\param p_mesh_name The mesh to get the UV's of
	\return The list of UV values for this mesh
	*****************************************************************************************/
	std::vector<vector2> getNodeUVs(const char* p_mesh_name);
	/*!***************************************************************************************
	\brief Get normals
	\param p_mesh_name Name of the mesh
	\return The vertex normals of the given mesh
	*****************************************************************************************/
	std::vector<vector3> getNodeVertexNormals(const char* p_mesh_name);

	/*!***************************************************************************************
	\brief Gets a node's transformation at a given frame of animation
	\param p_name Mesh name
	\param frame Frame of animation
	\return Matrix of given animation
	*****************************************************************************************/
	FbxMatrix getNodeTransformAnim(const char* p_name, size_t frame);

	/*!***************************************************************************************
	\brief Gets the node transformation
	\param p_name name of the mesh
	\param p_seconds seconds into the animation
	\param p_position OUTPUT: position of mesh
	\param p_rotation OUTPUT: euler rotation of mesh
	\param p_scale OUTPUT: scale of mesh
	*****************************************************************************************/
	void getNodeTransformAnimParts(const char* p_name, float p_seconds, vector3& p_position, vector3& p_rotation, vector3& p_scale);
	
	/*!***************************************************************************************
	\brief Get the list of all meshes in the scene
	\return List of all meshes in the scene
	*****************************************************************************************/
	const std::vector<FbxMesh*>& getMeshes();
	/*!***************************************************************************************
	\brief Gets the list of all meshes in the scene, as a map of their names
	\return Map of all meshes in the scene
	*****************************************************************************************/
	const std::unordered_map<std::string, FbxMesh*>& getMeshMap();


private:

	/*!***************************************************************************************
	\brief traverses a scene and parses each node
	*****************************************************************************************/
	void parseScene();
	/*!***************************************************************************************
	\brief parses a node
	\param p_node node to parse
	*****************************************************************************************/
	void parseNode(FbxNode* p_node);

	/*!***************************************************************************************
	\brief parses a mesh
	\param p_meshNode Mesh to parse
	*****************************************************************************************/
	void parseMesh(FbxNode* p_meshNode);

	FBXLoader& m_parent; //!< Parent laoder

	FbxScene* m_scene; //!< Scene this object is managing
	std::vector<FbxMesh*> m_meshes; //!< list of all meshes
	std::unordered_map<std::string, FbxMesh*> m_meshMap; //!<Map of meshes


	static logger log; //!<logger
	
};

/*!***************************************************************************************
\brief Mesh asset data
*****************************************************************************************/
class FbxMeshData : public assetData
{
public:
	/*!***************************************************************************************
	\brief Create a new mesh asset data
	\param p_parent parent file
	\param p_name name of the mesh node
	*****************************************************************************************/
	FbxMeshData(FbxFileData* p_parent, const std::string& p_name);

	/*!***************************************************************************************
	\brief Get vertices of mesh
	\return The list of vertices of this mesh asset
	*****************************************************************************************/
	std::vector<vector3> getVerts() const;
	/*!***************************************************************************************
	\brief Gets the list of faces
	\return List of faces
	*****************************************************************************************/
	std::vector<size_t> getFaces()const;
	/*!***************************************************************************************
	\brief Get UVs
	\return List of UVs
	*****************************************************************************************/
	std::vector<vector2> getUVs()const;
	/*!***************************************************************************************
	\brief Get normals
	\return List of normals
	*****************************************************************************************/
	std::vector<vector3> getNormals()const;
	/*!***************************************************************************************
	\brief Get animation in current seconds
	\param p_seconds Seconds into an animation to play
	\param p_position OUT: position of mesh
	\param p_rotation OUT: rotation of mesh in euler angles
	\param p_scale OUT: Scle of mesh
	*****************************************************************************************/
	void getAnimParts(float p_seconds, vector3& p_position, vector3& p_rotation, vector3& p_scale)const;

private:
	FbxFileData* m_parent; //!<parent file
	std::string m_name; //!<name of mesh node in the parent file
};


/*!***************************************************************************************
\brief FBX file loader
*****************************************************************************************/
class FBXLoader : public fileLoader
{
public:
	/*!***************************************************************************************
	\brief create a new fbx loader
	*****************************************************************************************/
	FBXLoader();
	FBXLoader(const FBXLoader& rhs) = delete;
	~FBXLoader();



	/*!***************************************************************************************
	\brief Get  geometry converter
	\return The Geometry converter for an fbx file
	*****************************************************************************************/
	FbxGeometryConverter* getGeometryConverter();


	/*!***************************************************************************************
	\brief Gets the list of asset names from the given filename
	\param base Base path for the asset directory
	\param p_filename Filename to check
	\param p_assets List of asset names
	*****************************************************************************************/
	void assetsFrom(const std::filesystem::path& base, const std::wstring& p_filename,
		std::vector<asset*>& p_assets) override;
	/*!***************************************************************************************
	\brief Loads an asset and fills out its data
	\param p_asset asset to load
	*****************************************************************************************/
	void loadAsset(asset& p_asset) override;
	/*!***************************************************************************************
	\brief Unloads the asset and repalces data with nullptr
	\param p_asset asset to unload
	*****************************************************************************************/
	void unloadAsset(asset& p_asset) override;
protected:

private:
	logger log; //!< logger
	FbxManager * m_manager = nullptr; //!<FbxSdk manager
	FbxIOSettings * m_io = nullptr; //!<FbxSdk IOsettings
	FbxGeometryConverter * m_converter = nullptr; //!<fbxsdk geo converter

	FbxFileData m_placeholder; //!< placeholder data for missing file handling
	std::unordered_map<std::string, FbxFileData*> m_loaded; //!< map of loaded fbx scenes to protect loading the same file twice

};


// namespace <namespace name>

