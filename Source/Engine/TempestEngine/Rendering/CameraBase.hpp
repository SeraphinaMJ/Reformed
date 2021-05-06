/*!***************************************************************************************
\file       CameraBase.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content ? 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "Color.hpp"
#include <Vector3.hpp>
#include <Matrix4x4.hpp>
#include <Component.hpp>

#include <memory>

class shaderProgram;
const float Pi = 3.1415926535897932384626433832795f;

struct viewFrustrumMesh 
{
	std::array<vector3, 8> mesh;

	vector3 center;

	vector3 dim;

	struct aabb
	{
		vector3 min;
		vector3 max;
	};

	aabb boundingBox() const;

	viewFrustrumMesh transform(matrix4x4 const& mat) const;
};

static unsigned numCameras = 0;
class cameraBase : public componentCRTP<cameraBase>
{
public:
	virtual void process() = 0;
	virtual void processNoTransform() = 0;

	virtual void calcViewMatrix() = 0;
	virtual void calcProjMatrix() = 0;
	virtual void calcViewProjMatrix() = 0;
	virtual void calcCamPosition() = 0;

	virtual vector3 getCameraPosition() { return m_cameraPosition; }

	/*!***************************************************************************************
	\brief  Gets the projection view matrix, the merge of the view matrix and the projection matrix.
	\return The matrix
	*****************************************************************************************/
	virtual matrix4x4 getViewProjectionMatrix();

	virtual matrix4x4 getInvViewMatrix() = 0;

	/*!***************************************************************************************
	\brief  Gets the view matrix.
	\return The matrix
	*****************************************************************************************/
	virtual matrix4x4 getViewMatrix();

	/*!***************************************************************************************
	\brief  Gets the projection matrix.
	\return The matrix
	*****************************************************************************************/
	virtual matrix4x4 getProjectionMatrix();

	virtual viewFrustrumMesh getViewFrustrum();


	void dirtyProcess();
	/*!***************************************************************************************
	\brief Sets all the camera unforms like fog and near and far plain
	\param p_program - the shader program we send uniforms to
	*****************************************************************************************/
	virtual void setCameraUniforms(std::shared_ptr<shaderProgram> p_program);


	virtual void setProjectionMatrix(const matrix4x4& p_projMatrix);
	virtual void setViewMatrix(const matrix4x4& p_viewMatrix);
	virtual void setViewProjectionMatrix(const matrix4x4& p_viewProjMatrix);

	virtual void setFieldOfViewDegree(float m_angleDegree);
	virtual float getFieldOfViewDegree();

	virtual float getFieldOfViewRad();

	virtual void setDimension(float p_width, float p_height);

	virtual float getWidth() { return m_width; }
	virtual void setWidth(float p_width);

	virtual float getHeight() { return m_height; }
	virtual void setHeight(float p_height);

	virtual float getAspectRatio() const;

	virtual float getNearPlaneDistance() { return m_nearPlaneDist; }
	virtual void setNearPlaneDistance(float p_zNear);

	virtual float getFarPlaneDistance() { return m_farPlaneDist; }
	virtual void setFarPlaneDistance(float p_zFar);

	virtual vector3 getViewVector() { return m_viewVec; }

	// TODO:
	virtual vector3 viewportGeometry() const {
		return { m_width / m_height, m_nearPlaneDist, m_farPlaneDist };
	}


	//virtual void setIsMainCamera(bool p_isMain) = 0;

	/*!***************************************************************************************
	\brief Prints camera data
	*****************************************************************************************/
	virtual void printData();

	/*!***************************************************************************************
	\brief Prints matrices data
	*****************************************************************************************/
	virtual void printMatrices();


	/*!***************************************************************************************
	\brief Retrieves the active status of this camera
	\return Returns true if the camera is active, false otherwise
	*****************************************************************************************/
	virtual bool getActive() { return m_active; }
	virtual void setActive(bool p_active) { m_active = p_active; }

	virtual vector3 getEyePosition() const
	{
		return m_cameraPosition;
	}

protected:
	matrix4x4 m_transform;               //!< inverse view matrix
	matrix4x4 m_viewMatrix;              //!< The view matrix
	matrix4x4 m_projectionMatrix;        //!< The projection matrix
	matrix4x4 m_viewProjectionMatrix;    //!< The view projection matrix
	matrix4x4 m_orthographicMatrix;      //!< The ortho matrix
	vector3 m_cameraPosition;            //!< The current position of this camera

	vector3 m_eye = { 0,0,1 };
	vector3 m_viewVec = { 0,0,-1 };      //!< The view vector
	float m_fieldOfView = Pi / 4.0f;     //!< The FOV
	float m_width = 1024.f;              //!< The width of the lens
	float m_height = 768.f;              //!< The height of the lens
	float m_nearPlaneDist = 1.0f;        //!< The distance to the near plane
	float m_farPlaneDist = 300.0f;      //!< The distance to the far plane
	color m_fogColor = color::gray;      //!< The fog color
	bool dirty = true;	//!< when set, recalculate matrices

	bool m_active = true;                //!< Is the camera currently active
};


