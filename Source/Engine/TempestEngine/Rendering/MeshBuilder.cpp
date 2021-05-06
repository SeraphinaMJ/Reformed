/*!***************************************************************************************
\file       MeshBuilder.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Static class that Builds all mesh types on the GPU and creates CPU objects.
*****************************************************************************************/
//========Self Include==================================================================//
#include "MeshBuilder.hpp"
//========1st Party Includes============================================================//
#include "TriangleMesh.hpp"
#include "TypeData.hpp"
#include "Mesh.hpp"
#include "VertexArrayManager.hpp"
#include "GraphicsSystem.hpp"
#include "VertexArrayObject.hpp"
#include "ShaderManager.hpp"

#include "../SystemManager.hpp"

        void meshBuilder::cubeBuilder(std::shared_ptr<triangleMesh> p_mesh, 
                                     std::shared_ptr<vertexArrayObject> p_vao)
        {

            std::vector<triangleMesh::vertex> l_vertices = p_mesh->getVertexData();
            std::vector<triangleMesh::face> l_faces = p_mesh->GetFaceData();

            for (triangleMesh::vertex vert : l_vertices)
            {
                p_vao->getVertexBufferObject().addVertex(vert);
            }

            // Step 3: add all indices (per triangle) to the IBO
            for (triangleMesh::face face : l_faces)
            {
                p_vao->getIndexBufferObject().addTriangle(face.a, face.b, face.c);
            }

            // Step 4: upload the contents of the VBO and IBO to the GPU and build the VAO
            p_vao->build(p_mesh.get());

            //Store everything
        }









