/*!***************************************************************************************
\file       VertexArrayManager.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages all the VAO objects on the CPU.
*****************************************************************************************/

#include "../Precompiled.h"
//========Self Include==================================================================//
#include "VertexArrayManager.hpp"
//========1st Party Includes============================================================//
#include "Mesh.hpp"
#include "TypeData.hpp"
#include "IndexBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "TriangleMesh.hpp"
#include "DataTypes.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//

		//==============================================================================//
		//        Destructor                                                            //
		//==============================================================================//
vertexArrayManager::~vertexArrayManager()
{
	m_vertexArrayBuffers.clear();
}

std::shared_ptr<vertexArrayObject> const vertexArrayManager::getVertexArrayBuffer(const std::string p_assetName)
{
	auto i_iter = m_vertexArrayBuffers.find(p_assetName); // find if asset exists
	if(i_iter == m_vertexArrayBuffers.end())
	{
		// no asset, load it
		std::filesystem::path l_path("models");
		l_path /= p_assetName;
		createArrayBuffer(assetManager::getAsset(l_path));
		i_iter = m_vertexArrayBuffers.find(p_assetName);
	}
	return  i_iter->second;
}

std::shared_ptr<vertexArrayObject> vertexArrayManager::createArrayBuffer(assetHandle p_assetHandle)
{

	auto assetPath = assetManager::stringFromWide(p_assetHandle->getName());

	triangleMesh * p_mesh = new triangleMesh(assetPath);

	p_mesh->loadInFile(p_assetHandle);

	// auto name = getFileNameFromAssetPath(assetManager::stringFromWide(p_assetHandle->getName()));
	auto name = std::filesystem::path(p_assetHandle->getName()).filename().generic_string();

	std::shared_ptr<vertexArrayObject> l_vao = buildVAO(p_mesh);

	m_vertexArrayBuffers.insert(std::make_pair(name, l_vao));
	return getVertexArrayBuffer(name);
}

std::shared_ptr<vertexArrayObject> vertexArrayManager::buildVAO(triangleMesh * p_mesh)
{

	std::vector<triangleMesh::vertex> l_vertices = p_mesh->getVertexData();
	std::vector<triangleMesh::face> l_faces = p_mesh->GetFaceData();

	std::shared_ptr<vertexArrayObject> l_vao = std::make_shared<vertexArrayObject>(p_mesh->getVertCount(), p_mesh->getFaceCount(), p_mesh->getVertexSize(), topology::enm_triangle);

	for (triangleMesh::vertex vert : l_vertices)
	{
		l_vao->getVertexBufferObject().addVertex(vert);
	}

	// Step 3: add all indices (per triangle) to the IBO
	for (triangleMesh::face face : l_faces)
	{
		l_vao->getIndexBufferObject().addTriangle(face.a, face.b, face.c);
	}

	// Step 4: upload the contents of the VBO and IBO to the GPU and build the VAO
	l_vao->build(p_mesh);

	return l_vao;
}

std::shared_ptr<vertexArrayObject> vertexArrayManager::buildFullScreenQuad(const std::string p_assetName)
{

	triangleMesh * l_mesh = new triangleMesh(p_assetName);


	l_mesh->addVertexToMesh(-1.0f, -1.0f, 0.0f);//0 - lower left
	l_mesh->addVertexToMesh(1.0f, -1.0f, 0.0f);//1 - lower right
	l_mesh->addVertexToMesh(-1.0f, 1.0f, 0.0f);//2 - top left
	l_mesh->addVertexToMesh(1.0f, 1.0f, 0.0f);//3 - top right

	l_mesh->addFaceToMesh(0, 1, 2);
	l_mesh->addFaceToMesh(2, 1, 3);

	l_mesh->addUVToMesh(1, 1);
	l_mesh->addUVToMesh(0, 1);
	l_mesh->addUVToMesh(1, 0);
	l_mesh->addUVToMesh(0, 0);

	std::shared_ptr<vertexArrayObject> l_vao = buildVAO(l_mesh);

	m_vertexArrayBuffers.insert(std::make_pair(p_assetName, l_vao));

	return l_vao;
}




