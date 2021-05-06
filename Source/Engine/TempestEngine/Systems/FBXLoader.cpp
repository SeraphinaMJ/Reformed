/*!**********************************************************************************************************
\file       FBXLoader.cpp
\author     Henry Brobeck
\date       11/20/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief 
************************************************************************************************************/
//Ref: http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/

//========  Self Include  =================================================================================//
#include "FBXLoader.hpp"
#include <Logger.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>

#include <string>

logger FbxFileData::log("FBXFile");

namespace
{
	template<typename T>
	void fbxToVector3(const T& fbxVec, vector3& vector3)
	{
		vector3.x = static_cast<float>(fbxVec[0]);
		vector3.y = static_cast<float>(fbxVec[1]);
		vector3.z = static_cast<float>(fbxVec[2]);
	}
	template<typename T>
	void fbxToVector2(const T& fbxVec, vector2& vector2)
	{
		vector2.x = static_cast<float>(fbxVec[0]);
		vector2.y = static_cast<float>(fbxVec[1]);
	}
}

FbxFileData::FbxFileData(FbxScene* p_scene, FBXLoader& p_parent) : m_parent(p_parent), m_scene(p_scene)
{
	
	if(p_scene == nullptr)
	{
		return;
	}
	//triangulate before any processing
	bool success = p_parent.getGeometryConverter()->Triangulate(p_scene, true);
	if(!success)
	{
		log.error("Failed to triangulate scene");
	}
	parseScene();
	
}

FbxFileData::~FbxFileData()
{
	if (!m_scene)
		return;
	m_scene->Destroy();
}



FbxNode* FbxFileData::getRoot()
{
	return m_scene->GetRootNode();
}

FbxScene* FbxFileData::getScene()
{
	return m_scene;
}

FbxNode* FbxFileData::getNodeByName(const char* p_name)
{
	return getRoot()->FindChild(p_name);
}

FbxMesh* FbxFileData::getMeshByName(const char* p_name)
{
	auto location = m_meshMap.find(p_name);
	if(location == m_meshMap.end())
	{
		return nullptr;
	}
	return location->second;
}

std::vector<vector3> FbxFileData::getNodeVerts(const char* p_mesh_name)
{
	if(!p_mesh_name)
	{
		return {};
	}
	auto mesh = getMeshByName(p_mesh_name);
	if(!mesh)
	{
		return {};
	}
	size_t num_verts = mesh->GetControlPointsCount();
	auto verts = mesh->GetControlPoints();
	std::vector<vector3> vert_results;
	for(size_t i = 0; i < num_verts; ++i)
	{
		vector3 vert;
		fbxToVector3(verts[i], vert);
		vert_results.push_back(vert);
	}
	return vert_results;
}

std::vector<size_t> FbxFileData::getNodeFaces(const char* p_mesh_name)
{
	if (!p_mesh_name)
	{
		return {};
	}
	auto mesh = getMeshByName(p_mesh_name);
	if(!mesh)
	{
		return {};
	}
	size_t num_poly = mesh->GetPolygonCount();
	auto polys = mesh->GetPolygonVertices();
	std::vector<size_t> result;
	for(size_t i = 0; i < num_poly; ++i)
	{
		result.push_back(polys[i]);
	}
	return result;
	
}

std::vector<vector2> FbxFileData::getNodeUVs(const char* p_mesh_name)
{
	if(!p_mesh_name)
	{
		return {};
	}
	auto mesh = getMeshByName(p_mesh_name);
	if(!mesh)
	{
		return {};
	}
	size_t num_UV = mesh->GetTextureUVCount();
	if(!num_UV)
	{
		return {};
	}

	std::vector<vector2> result;
	FbxLayerElementArrayTemplate<FbxVector2>* UV;
	bool success = mesh->GetTextureUV(&UV);
	if(!success)
	{
		return {};
	}
	for(size_t i = 0; i < num_UV; ++i)
	{
		vector2 vec{};
		fbxToVector2(UV->GetAt(int(i)),vec);
		result.push_back(vec);
	}
	
	return result;
}

std::vector<vector3> FbxFileData::getNodeVertexNormals(const char* p_mesh_name)
{
	if (!p_mesh_name)
	{
		return {};
	}
	auto mesh = getMeshByName(p_mesh_name);
	if (!mesh)
	{
		return {};
	}

	size_t num_vertex = mesh->GetPolygonVertexCount();
	std::vector<vector3> results;
	FbxArray<FbxVector4> normals;
	mesh->GetPolygonVertexNormals(normals);
	for(size_t i = 0; i < num_vertex; ++i)
	{
		vector3 norm;
		fbxToVector3(normals[int(i)], norm);
		results.push_back(norm);
	}

	return results;

}


FbxMatrix FbxFileData::getNodeTransformAnim(const char* p_name, size_t frame)
{
	auto node = getNodeByName(p_name);
	if(node == nullptr)
	{
		return {};
	}

	auto eval = m_scene->GetAnimationEvaluator();
	FbxTime time;
	time.SetFrame(frame);
	
	return eval->GetNodeGlobalTransform(node, time);


}

void FbxFileData::getNodeTransformAnimParts(const char* p_name, float p_seconds, vector3& p_position, vector3& p_rotation,
	vector3& p_scale)
{
	auto node = getNodeByName(p_name);
	if(node == nullptr)
	{
		return;
	}
	auto eval = m_scene->GetAnimationEvaluator();
	FbxTime time;
	time.SetSecondDouble(p_seconds);
	auto position = eval->GetNodeLocalTranslation(node, time);
	auto scale = eval->GetNodeLocalScaling(node, time);
	auto rotation = eval->GetNodeLocalRotation(node, time);

	p_position.x = static_cast<float>(position[0]);
	p_position.y = static_cast<float>(position[1]);
	p_position.z = static_cast<float>(position[2]);

	p_scale.x = static_cast<float>(scale[0]);
	p_scale.y = static_cast<float>(scale[1]);
	p_scale.z = static_cast<float>(scale[2]);

	p_rotation.x = static_cast<float>(rotation[0]);
	p_rotation.y = static_cast<float>(rotation[1]);
	p_rotation.z = static_cast<float>(rotation[2]);
	
}

const std::vector<FbxMesh*>& FbxFileData::getMeshes()
{
	return m_meshes;
}

const std::unordered_map<std::string, FbxMesh*>& FbxFileData::getMeshMap()
{
	return m_meshMap;
}


void FbxFileData::parseScene()
{
	FbxNode* root = getRoot();
	if(!root)
	{
		return;
	}
	for(int i = 0; i < root->GetChildCount(); ++i)
	{
		parseNode(root->GetChild(i));
	}
}

void FbxFileData::parseNode(FbxNode* p_node)
{
	if(!p_node)
	{
		return;
	}
	//TODO(hank): parse more than just meshes
	FbxNodeAttribute* attribute = p_node->GetNodeAttribute();
	if(attribute != nullptr)
	{
		FbxNodeAttribute::EType type;
		type = attribute->GetAttributeType();
		switch(type)
		{
		case FbxNodeAttribute::eUnknown: break;
		case FbxNodeAttribute::eNull: break;
		case FbxNodeAttribute::eMarker: break;
		case FbxNodeAttribute::eSkeleton: break;
		case FbxNodeAttribute::eMesh: 
			parseMesh(p_node);
			break;
		case FbxNodeAttribute::eNurbs: break;
		case FbxNodeAttribute::ePatch: break;
		case FbxNodeAttribute::eCamera: break;
		case FbxNodeAttribute::eCameraStereo: break;
		case FbxNodeAttribute::eCameraSwitcher: break;
		case FbxNodeAttribute::eLight: break;
		case FbxNodeAttribute::eOpticalReference: break;
		case FbxNodeAttribute::eOpticalMarker: break;
		case FbxNodeAttribute::eNurbsCurve: break;
		case FbxNodeAttribute::eTrimNurbsSurface: break;
		case FbxNodeAttribute::eBoundary: break;
		case FbxNodeAttribute::eNurbsSurface: break;
		case FbxNodeAttribute::eShape: break;
		case FbxNodeAttribute::eLODGroup: break;
		case FbxNodeAttribute::eSubDiv: break;
		case FbxNodeAttribute::eCachedEffect: break;
		case FbxNodeAttribute::eLine: break;
		default: break;
		}
	}
	for(int i = 0; i < p_node->GetChildCount(); ++i)
	{
		parseNode(p_node->GetChild(i));
	}
}

void FbxFileData::parseMesh(FbxNode* p_meshNode)
{
	FbxMesh *mesh = (FbxMesh*)(p_meshNode->GetNodeAttribute());
	if(mesh == nullptr)
	{
		return;
	}
	std::string name(p_meshNode->GetName());
	m_meshMap.insert(std::pair(name, mesh));
}

FbxMeshData::FbxMeshData(FbxFileData* p_parent, const std::string& p_name) : m_parent(p_parent),m_name(p_name)
{
}

std::vector<vector3> FbxMeshData::getVerts() const
{
	return m_parent->getNodeVerts(m_name.c_str());
}

std::vector<size_t> FbxMeshData::getFaces() const
{
	return m_parent->getNodeFaces(m_name.c_str());
}

std::vector<vector2> FbxMeshData::getUVs() const
{
	return m_parent->getNodeUVs(m_name.c_str());
}

std::vector<vector3> FbxMeshData::getNormals() const
{
	return m_parent->getNodeVertexNormals(m_name.c_str());
}

void FbxMeshData::getAnimParts(float p_seconds, vector3& p_position, vector3& p_rotation, vector3& p_scale) const
{
	return m_parent->getNodeTransformAnimParts(m_name.c_str(), p_seconds, p_position, p_rotation, p_scale);
}


FBXLoader::FBXLoader() : log("FBXLoader"), m_placeholder(nullptr, *this)
{
	m_manager = FbxManager::Create();
	m_io = FbxIOSettings::Create(m_manager, IOSROOT);
	m_manager->SetIOSettings(m_io);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, false);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, false);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, false);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
	(*(m_manager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	m_converter = new FbxGeometryConverter(m_manager);
	
	
}

FBXLoader::~FBXLoader()
{
	for (auto iter : m_loaded)
	{
		delete iter.second;
	}
	m_io->Destroy();
	m_manager->Destroy();
	
	
}




FbxGeometryConverter* FBXLoader::getGeometryConverter()
{
	return m_converter;
}

void FBXLoader::assetsFrom(const std::filesystem::path& p_base, const std::wstring& p_filename,
	std::vector<asset*>& p_assets)
{
	//determine file relative name
	auto name = std::filesystem::relative(p_filename, p_base);
	//get std string version of filename
	auto thinFilename = assetManager::stringFromWide(p_filename);
	//see if scene has already been loaded
	

	//setup importer
	FbxImporter *importer = FbxImporter::Create(m_manager, "");
	bool init = importer->Initialize(thinFilename.c_str(), -1, m_manager->GetIOSettings());
	if (!init)
	{
		log.error("Failed to initialize importer for fbx file");
		importer->Destroy();
		return;
	}

	FbxScene* fileScene = FbxScene::Create(m_manager, "");
	importer->Import(fileScene);
	FbxFileData file(fileScene, *this);


	
	//get list of meshes
	auto meshes = file.getMeshMap();
	for(auto& iter: meshes)
	{
		std::wstring meshName = assetManager::wideFromString(iter.first);
		std::wstring assetName = name.generic_wstring() + L"|" + meshName;
		auto meshAsset = new asset(p_filename, assetName, this);
		//set asset type to mesh
		meshAsset->setType(L"FbxMesh");
		p_assets.push_back(meshAsset);

	}

	

}

void FBXLoader::loadAsset(asset& p_asset)
{
	//TODO support more than just mesh
	//TODO is this just turning std::wstring into std::string?
	auto thinFilename = assetManager::stringFromWide(p_asset.getFullPath().generic_wstring());
	auto fileIter = m_loaded.find(thinFilename);
	FbxFileData* file = nullptr;
	if(fileIter == m_loaded.end())
	{
		//setup importer
		FbxImporter *importer = FbxImporter::Create(m_manager, "");
		bool init = importer->Initialize(thinFilename.c_str(), -1, m_manager->GetIOSettings());
		if (!init)
		{
			log.error("Failed to initialize importer for fbx file");
			importer->Destroy();
			return;
		}

		FbxScene* fileScene = FbxScene::Create(m_manager, "");
		importer->Import(fileScene);
		//keep loaded
		file = new FbxFileData(fileScene, *this);
		m_loaded.insert(std::pair(thinFilename, file));
	}
	else
	{
		file = fileIter->second;
	}

	//get node name from asset name
	auto name = p_asset.getName();
	auto pipeloc = name.find_last_of(L"|");
	auto postpipe = name.substr(pipeloc).substr(1);
	p_asset.setData(new FbxMeshData(file, assetManager::stringFromWide(postpipe)));
}

void FBXLoader::unloadAsset(asset& p_asset)
{
	//TODO: support more than just mesh
	delete &p_asset.getData<FbxMeshData>();
	p_asset.setData(nullptr);
}
