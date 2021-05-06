/*!***************************************************************************************
\file       CameraManipulator.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "CameraManipulator.hpp"
#include "../Rendering/CameraSystem.hpp"
#include <MathFunctions.hpp>
#include "../SceneManagement/SceneSystem.hpp"
#include "../SceneManagement/Scene.hpp"
#include "GameObject.hpp"

#include <Matrix4x4.hpp>

editorCamera::editorCamera()
	: m_euler_angles(0.0f,0.0f,0.0f)
{
	m_cameraPosition = vector3(0.0f, 2.0f, 20.0f);
}
typeRT editorCamera::toTypeRT() const
{
	// TODO(coseo) if we end up needing this i will write it
	typeRT result;
	return result;
}

editorCamera::editorCamera(typeRT& p_type)
{
	// TODO(coseo) if we end up needing this i will write it
}

void editorCamera::updateFromTypeRT(typeRT & p_type)
{
	// TODO(coseo) if we end up needing this i will write it
}

void editorCamera::process()
{
	calcViewMatrix();
	calcProjMatrix();
	calcViewProjMatrix();
	dirty = false;
}

void editorCamera::processNoTransform()
{
	// global matrix without transform, inverse
	m_transform = matrix4x4::build(vector3(0,0,0), toQuaternion(m_euler_angles), vector3(1,1,1));
	m_viewMatrix = m_transform.inverse();

	calcProjMatrix();
	calcViewProjMatrix();
}

void editorCamera::calcViewMatrix()
{
	// global matrix, inverse
	m_transform = matrix4x4::build(m_cameraPosition, toQuaternion(m_euler_angles), vector3(1,1,1));
	m_viewMatrix = m_transform.inverse();
}

void editorCamera::calcProjMatrix()
{
	m_projectionMatrix = matrix4x4::computeProjMatrix(m_fieldOfView, (m_width / m_height),
		m_nearPlaneDist, m_farPlaneDist, false);
}

void editorCamera::calcViewProjMatrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

matrix4x4 editorCamera::getInvViewMatrix()
{
	return m_transform;
}



void editorCamera::calcCamPosition()
{
	// no need, already computed
}
float cameraManipulator::getWidth()
{
	return getMainCamera()->getWidth();
}
float cameraManipulator::getHeight()
{
	return getMainCamera()->getHeight();
}
componentHandle<cameraBase> cameraManipulator::getMainCamera()
{
	auto l_camSystem = m_system_manager->getSystem<cameraSystem>();
	auto l_camera = l_camSystem->getMainCamera();
	return l_camera;
}
void cameraManipulator::readCameraMatrices(matrixArray1D& p_camera_view_matrix, matrixArray1D& p_camera_projection_matrix)
{
	// get the camera
	auto l_camSystem = m_system_manager->getSystem<cameraSystem>();
	auto l_camera = l_camSystem->getMainCamera();

	const matrix4x4 l_view_mat = l_camera->getViewMatrix();
	const matrix4x4 l_proj_mat = l_camera->getProjectionMatrix();

	std::memcpy(p_camera_view_matrix, l_view_mat.toFloatPtr(), sizeof(p_camera_view_matrix));
	std::memcpy(p_camera_projection_matrix, l_proj_mat.toFloatPtr(), sizeof(p_camera_projection_matrix));
}


void editorCamera::panCamera(float p_right_amount, float p_up_amount)
{
	vector3 l_newPos = m_cameraPosition;
	l_newPos += p_right_amount * m_transform.right();
	l_newPos += p_up_amount * m_transform.up();
	m_cameraPosition = l_newPos;

	dirty = true;
}

void editorCamera::rotateCamera2D(float p_right_amount, float p_up_amount)
{
	m_euler_angles.offsetX(p_up_amount);
	m_euler_angles.offsetY(p_right_amount);

	dirty = true;
}

void editorCamera::transCamera3D(float p_right_amount, float p_forward_amount)
{
	vector3 l_newPos = m_cameraPosition;
	l_newPos += p_right_amount * m_transform.right();
	l_newPos += p_forward_amount * m_transform.forward();
	m_cameraPosition = l_newPos;

	dirty = true;
}

void editorCamera::setNewPosition(const vector3 & newCameraPos, const vector3& positionToLookAt)
{


    // get vector from old pos to new pos
    vector3 diff = newCameraPos - m_cameraPosition;

    // look at vector is from new camera position to object we will be looking at
    //vector3 newLookAt = (positionToLookAt - newCameraPos).normalize();

    // physically move camera
    //m_transform = m_transform.translation(newCameraPos);

    // update our matrix
    this->m_transform.computeModelMatrixFromLookAtVector2(newCameraPos, positionToLookAt);

    m_cameraPosition = newCameraPos;

    dirty = true;
}



cameraManipulator::cameraManipulator(systemManagerInterface * p_system_manager)
	: m_system_manager{p_system_manager}
{ }

void cameraManipulator::enableEditorCamera()
{
	cameraSystem * l_cam_sys = m_system_manager->getSystem<cameraSystem>();
	if (!m_editor_camera)
	{
		// editor camera should exist
		std::shared_ptr<gameObject> objPtr =
			m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock()->getCameraObject();
		m_editor_camera = objPtr->getComponent<editorCamera>();
	}
	l_cam_sys->enableEditorCamera(castHandle<cameraBase>(m_editor_camera));
}

void cameraManipulator::disableEditorCamera()
{
	cameraSystem * l_cam_sys = m_system_manager->getSystem<cameraSystem>();
	l_cam_sys->disableEditorCamera();
}

void cameraManipulator::panCameraPosition(float p_right_amount, float p_up_amount)
{
	m_editor_camera->panCamera(p_right_amount, p_up_amount);
}

void cameraManipulator::rotateCamera2DCoords(float p_right_amount, float p_up_amount)
{
	m_editor_camera->rotateCamera2D(p_right_amount, p_up_amount);
}

void cameraManipulator::transCameraXZcoords(float p_right_amount, float p_forward_amount)
{
	m_editor_camera->transCamera3D(p_right_amount, p_forward_amount);
}

void cameraManipulator::set3DPos(const vector3 & newPos, const vector3& positionToLookAt)
{
    m_editor_camera->setNewPosition(newPos, positionToLookAt);
}


float cameraManipulator::getViewX()
{
	return getMainCamera()->getViewVector().x;
}

float cameraManipulator::getViewY()
{
	return getMainCamera()->getViewVector().y;
}

float cameraManipulator::getViewZ()
{
	return getMainCamera()->getViewVector().z;
}

float cameraManipulator::getNearDistance()
{
	return getMainCamera()->getNearPlaneDistance();
}

float cameraManipulator::getFarDistance()
{
	return getMainCamera()->getFarPlaneDistance();
}

float cameraManipulator::getCameraPosX()
{
	return getMainCamera()->getCameraPosition().getX();
}

float cameraManipulator::getCameraPosY()
{
	return getMainCamera()->getCameraPosition().getY();
}

float cameraManipulator::getCameraPosZ()
{
	return getMainCamera()->getCameraPosition().getZ();
}


