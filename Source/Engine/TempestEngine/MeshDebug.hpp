/*!***************************************************************************************
\file       MeshDebug.hpp
\author     Henry Brobeck
\date       8/15/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Decorator for Mesh for extra debug info
*****************************************************************************************/

#include "Rendering/Mesh.hpp"
#include "Rendering/TriangleMesh.hpp"

#include <glm/glm.hpp>

/*!***************************************************************************************
\par class: meshDebug
\brief Decorator for Mesh with extra debug info
*****************************************************************************************/
class meshDebug : public triangleMesh
{
public:
	/*!***************************************************************************************
	\brief Decorate a mesh with debug info
	\param p_parent mesh to decorate
	*****************************************************************************************/
	meshDebug(const triangleMesh& p_parent);


	/*!***************************************************************************************
	\brief Assignment
	\param p_parent mesh to decorate
	\return A decorated mesh
	*****************************************************************************************/
	meshDebug& operator=(const triangleMesh& p_parent);


	/*!***************************************************************************************
	\brief Get the minimum vertex position
	\return the minimum vertex positon (bottom-left-back)
	*****************************************************************************************/
	glm::vec3 getMinPos();

	/*!***************************************************************************************
	\brief Get the maximum vertex position
	\return get the maximum vertex position (top-right-forward)
	*****************************************************************************************/
	glm::vec3 getMaxPos();

	/*!***************************************************************************************
	\brief Get the min and max vertex positions
	\return a tuple containing <min,max>
	*****************************************************************************************/
	std::tuple<glm::vec3, glm::vec3> getMinMax();

	/*!***************************************************************************************
	\brief Get the bounding box size of the mesh
	\return the box size as <x, y, z>
	*****************************************************************************************/
	glm::vec3 getBoxSize();

	void setBoxColor(const glm::vec3& p_color);

	void setBoxColor(float p_r, float p_g, float p_b);

	glm::vec3 getBoxColor();


	/*!***************************************************************************************
	\brief Get the center point of the mesh
	\return the centroid of the mesh
	*****************************************************************************************/
	glm::vec3 getCenterPos();

	/*!***************************************************************************************
	\brief Get the offset between origin and center position
	\return The offset between origin and the center position
	*****************************************************************************************/
	glm::vec3 getOffset();


	void setLineLength(float p_length);


	/*!***************************************************************************************
	\brief deconstructor
	*****************************************************************************************/
	~meshDebug() = default;



protected:

	glm::vec3 m_min_pos; //!< Minimum position
	glm::vec3 m_max_pos; //!< Maximum position

	glm::vec3 m_box_size; //!< Box size
	glm::vec3 m_box_color; //!< RGB box color

	float m_line_length; //!<Normal data viewer line length





private:

	void generateData();


};


