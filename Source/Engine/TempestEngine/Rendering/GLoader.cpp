#include "../Precompiled.h"
#include "GLoader.hpp"
#include "../Logging/Logger.hpp"
#include "../Rendering/GLDebug.hpp"
#include "../Rendering/TypeData.hpp"



const LPCSTR gLoader::assetListName { "data/asset.list" };

void gLoader::preload(const std::string & p_assetPath, const std::string & p_dataPath, manager_pack & p_gPach, bool p_preloadAll)
{
	UNREFERENCED_PARAMETER(p_assetPath);

	if (p_preloadAll)
		preloadAll(p_gPach);
	else
		preloadSelective(p_dataPath, p_gPach);
}

void gLoader::preloadAll(manager_pack & p_gPach)
{
	// preload materials
	p_gPach.m_materialManager->createMaterial("skybox.mtl");
	p_gPach.m_materialManager->createMaterial("basic.mtl");
	p_gPach.m_materialManager->createMaterial("duck.mtl");

	// preload meshes

	std::shared_ptr<triangleMesh> l_mesh = p_gPach.m_meshManager->createMesh("sampleCube.obj");

	std::shared_ptr<triangleMesh> l_chickenMesh = p_gPach.m_meshManager->createMesh("HEN_ry.obj");

	std::shared_ptr<triangleMesh> l_duckMesh = p_gPach.m_meshManager->createMesh("duck.obj");

	std::shared_ptr<triangleMesh> l_quadMesh = p_gPach.m_meshManager->buildFullScreenQuad("quad.obj");
}

void gLoader::preloadSelective(const std::string & p_dataPath, manager_pack & p_gPach)
{

}

bool gLoader::modified(LPCSTR p_hFile)
{
	
	// Grab creation Time from file one
	HANDLE l_assList = openFile(assetListName);
	FILETIME creationTime_A, lpLastAccessTime_A, lastWriteTime_A;
	bool err_A = GetFileTime(l_assList, &creationTime_A, &lpLastAccessTime_A, &lastWriteTime_A);

	// Grab creationg time form file two
	HANDLE l_other = openFile(p_hFile);
	FILETIME creationTime_B, lpLastAccessTime_B, lastWriteTime_B;
	bool err_B = GetFileTime(l_other, &creationTime_B, &lpLastAccessTime_B, &lastWriteTime_B);

	ASSERT(err_A || err_B);

	LONG resualt = CompareFileTime(&creationTime_A, &creationTime_B);
	

	return resualt;
}

HANDLE gLoader::openFile(LPCSTR p_fileName) 
{
	HANDLE l_hFile = CreateFile(p_fileName,               // file to open
		GENERIC_READ | GENERIC_WRITE,					  // open for reading and writing
		FILE_SHARE_READ | FILE_SHARE_WRITE,				  // shared for reading and writing
		NULL,									          // default security
		OPEN_ALWAYS,                                      // creates if doesnt exsist
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,     // normal file
		NULL);                                            // no attr. template

	ASSERT(l_hFile != INVALID_HANDLE_VALUE);

	return l_hFile;
}

//std::vector<HANDLE> gLoader::getModifiedList(LPCSTR filePath)
//{
//	UNREFERENCED_PARAMETER(filePath);
//
//	// check to see if file is older then list
//
//	// if older loader relevent information into list
//}