/*!***************************************************************************************
\file       TriangleMesh.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This holds all data for constructing a mesh made of triangles
*****************************************************************************************/


#pragma once
//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "Mesh.hpp"
enum class DefaultUvType;
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
struct vector3;

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: triangleMesh
\brief      This holds all data for constructing a mesh made of triangles.
*****************************************************************************************/
class triangleMesh : public mesh
{

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

public:



	/*!***************************************************************************************
	\brief  Constructor for triangle mesh
	\param  p_uvType - what kind of uv mapping if not taken from file
	*****************************************************************************************/
	triangleMesh(const std::string p_meshLabel) : mesh(p_meshLabel) {};

	/*!***************************************************************************************
	\brief  Loads In an constructs the mesh
	\param  p_assetPath - Path to the assets folder
	\param  meshLabel   - The lable of the mesh
	\param  objFileName - The object file to be loaded in
*****************************************************************************************/
	void loadInFile(assetHandle p_assetHandle);

	/*!***************************************************************************************
	\brief triangleMesh copy constructor
	\param p_other other triangleMesh to copy
	*****************************************************************************************/
	triangleMesh(const triangleMesh& p_other);

	/*!***************************************************************************************
	\par struct: face
	\brief      This holds all data for a single triangle face.
	*****************************************************************************************/
	struct face
	{
		int a;  //!<  Point a
		int b;  //!<  Point b
		int c;  //!<  Point c

		/*!***************************************************************************************
		\brief  Constructor for a face
		\param  a - First point
		\param  b - Second point
		\param  c - Third point
		*****************************************************************************************/
		face(int a = -1, int b = -1, int c = -1)
			: a(a), b(b), c(c) {}
	};



	/*!***************************************************************************************
	\brief  Adds a face to the mesh
	\param  a - First point
	\param  b - Second point
	\param  c - Third point
	*****************************************************************************************/
	void addFaceToMesh(unsigned a, unsigned b, unsigned c);

	void addUVToMesh(float u, float v);

	void addPositionToMesh(float a, float b, float c);

	/*!***************************************************************************************
	\brief  Gets all faces of triangle mesh
	\return A shared pointer to all the faces
	*****************************************************************************************/
	std::vector<face> GetFaceData() { return m_faces; } // TODO(cody): Make LowerCase

	/*!***************************************************************************************
	\brief   Gets the count of how many faces for this mesh
	\return  The size_t of the count
	*****************************************************************************************/
	size_t getFaceCount() { return m_faces.size(); }



	/*!***************************************************************************************
	\brief Calulates all the face normals and calls centerMesh()
	*****************************************************************************************/
	void preprocess();
	void process();

	triangleMesh* CalcUvBox();

	void addNormalToMesh(float x, float y, float z);

	//======================================================================================//
	//        Operators                                                                     //
	//======================================================================================//
	/*!***************************************************************************************
	\brief triangleMesh assignment operator
	\param p_other other triangleMesh to be equal to
	\return Reference to lhs  triangleMesh
	*****************************************************************************************/
	triangleMesh& operator=(const triangleMesh& p_other);
	//triangleMesh* CalcTanBitan();

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////

private:

	/////================================================================================/////
	/////    Data                                                                        /////
	/////================================================================================/////

	/*!***************************************************************************************
	\brief  Moves the mesh center to 0,0
	*****************************************************************************************/
	void centerMesh();



	vector3 m_center = { 0,0,0 };                //!<  The center of the mesh
	std::vector<face> m_faces;                   //!<  Holds all the faces of the mesh
	std::vector<vector3> m_position;
	std::vector<vector3> m_triangleNormals;      //!<  holds all the normals of the faces
	std::vector<vector2> m_uvs;
	std::vector<vector3> m_tangent;
	std::vector<vector3> m_bitangent;


	std::vector<std::vector<std::vector<unsigned>>> m_indices;

	bool normalsSet = false;
	bool uvsSet = false;

};

