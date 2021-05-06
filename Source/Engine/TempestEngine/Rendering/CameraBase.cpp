/*!***************************************************************************************
\file       CameraBase.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "CameraBase.hpp"

#include "ShaderProgram.hpp"
#include <MathFunctions.hpp>

#include <Vector2.hpp>
#include <Vector3.hpp>
#include <algorithm>
#include <cmath>

matrix4x4 cameraBase::getViewProjectionMatrix()
{
	dirtyProcess();
	return m_viewProjectionMatrix;
}

matrix4x4 cameraBase::getViewMatrix()
{
	dirtyProcess();
	return m_viewMatrix;
}

matrix4x4 cameraBase::getProjectionMatrix()
{
	dirtyProcess();
	return m_projectionMatrix;
}

viewFrustrumMesh cameraBase::getViewFrustrum()
{
	std::array<vector3, 8> vertices;

	const float tan = std::tan(0.5f * m_fieldOfView);

	// near plane
	const float aspect = m_width / m_height;

	const float halfwidthN = tan * m_nearPlaneDist;
	const float halfheightN = halfwidthN / aspect;

	vertices[0] = vector3(-halfwidthN, -halfheightN, -m_nearPlaneDist);
	vertices[1] = vector3(halfwidthN, -halfheightN, -m_nearPlaneDist);
	vertices[2] = vector3(halfwidthN, halfheightN, -m_nearPlaneDist);
	vertices[3] = vector3(-halfwidthN, halfheightN, -m_nearPlaneDist);

	// far plane
	const float halfwidthF = tan * m_farPlaneDist;
	const float halfheightF = halfwidthF / aspect;

	vertices[4] = vector3(-halfwidthF, -halfheightF, -m_farPlaneDist);
	vertices[5] = vector3(halfwidthF, -halfheightF, -m_farPlaneDist);
	vertices[6] = vector3(halfwidthF, halfheightF, -m_farPlaneDist);
	vertices[7] = vector3(-halfwidthF, halfheightF, -m_farPlaneDist);

	const auto center = vector3(0, 0, 0.5f * (m_nearPlaneDist + m_farPlaneDist));
	const auto viewport_dimensions = vector3(2.0f * halfwidthF, 2.0f * halfheightF, m_farPlaneDist);
		
	return { vertices, center, viewport_dimensions };
}

viewFrustrumMesh::aabb viewFrustrumMesh::boundingBox() const
{
	constexpr auto float_max = std::numeric_limits<float>::max();

	aabb aabb{ float_max, -float_max };

	for (const auto& point : mesh)
	{
		for (size_t i = 0; i != 3; ++i)
		{
			aabb.min[i] = std::min(aabb.min[i], point[i]);
			aabb.max[i] = std::max(aabb.max[i], point[i]);
		}
	}

	return aabb;
}

viewFrustrumMesh viewFrustrumMesh::transform(matrix4x4 const& mat) const
{
	viewFrustrumMesh newMesh;

	const auto transform = [&mat](const vector3& point) -> vector3
	{
		// apply transformation as point
		return vector3{ mat * vector4{ point, 1.0f } };
	};
	
	const auto transformVector = [&mat](const vector3& vector) -> vector3
	{
		return vector3{ mat * vector4{ vector, 0.0f} };
	};

	std::transform(mesh.cbegin(), mesh.cend(), newMesh.mesh.begin(), transform);

	newMesh.center = transform(center);

	newMesh.dim = transformVector(dim);

	return newMesh;
}

void cameraBase::dirtyProcess()
{
	if (dirty)
	{
		process();
		dirty = false;
	}
}

void cameraBase::setProjectionMatrix(const matrix4x4& p_projMatrix)
{
	m_projectionMatrix = p_projMatrix;
	dirty = false;
}

void cameraBase::setViewMatrix(const matrix4x4& p_viewMatrix)
{
	m_viewMatrix = p_viewMatrix;
	dirty = false;
}

void cameraBase::setViewProjectionMatrix(const matrix4x4& p_viewProjMatrix)
{
	m_viewProjectionMatrix = p_viewProjMatrix;
	dirty = false;
}

void cameraBase::setFieldOfViewDegree(float m_angleDegree)
{
	m_fieldOfView = DegToRad(m_angleDegree);
	dirty = true;
}

float cameraBase::getFieldOfViewDegree()
{
	  return RadToDeg(m_fieldOfView);
}

float cameraBase::getFieldOfViewRad()
{
	return m_fieldOfView;
}

void cameraBase::setDimension(float p_width, float p_height)
{
	m_width = p_width;
	m_height = p_height;

	dirty = true;
}

void cameraBase::setWidth(float p_width)
{
	m_width = p_width;
	dirty = true;
}

void cameraBase::setHeight(float p_height)
{
	m_height = p_height;

	dirty = true;
}

float cameraBase::getAspectRatio() const
{
	return m_width / m_height;
}

void cameraBase::setNearPlaneDistance(float p_zNear)
{
	m_nearPlaneDist = p_zNear;
	dirty = true;
}

void cameraBase::setFarPlaneDistance(float p_zFar)
{
	m_farPlaneDist = p_zFar;

	dirty = true;
}

void cameraBase::calcProjMatrix()
{
	m_projectionMatrix = matrix4x4::computeProjMatrix(m_fieldOfView, (m_width / m_height), m_nearPlaneDist, m_farPlaneDist, false);
}

void cameraBase::calcViewProjMatrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void cameraBase::printData()
{
	logger l_logger("Camera");
	l_logger.debug() << "fov: " << m_fieldOfView;
	l_logger.debug() << "width: " << m_width;
	l_logger.debug() << "height: " << m_height;
	l_logger.debug() << "near: " << m_nearPlaneDist;
	l_logger.debug() << "far: " << m_farPlaneDist << "\n";
	printMatrices();
}

void cameraBase::printMatrices()
{
	if (dirty) process();

	logger l_logger("Camera");
	l_logger.debug() << "viewMat: " << m_viewMatrix.toStringPtr() << "\n";
	l_logger.debug() << "projMat: " << m_projectionMatrix.toStringPtr() << "\n";
	l_logger.debug() << "vprojMat: " << m_viewProjectionMatrix.toStringPtr() << "\n";
}

void cameraBase::setCameraUniforms(std::shared_ptr<shaderProgram> p_program)
{
	if (dirty) process();

	p_program->setUniforms("camera.position", vector4(m_cameraPosition, 1));
	p_program->setUniforms("camera.i_fog", m_fogColor);
	p_program->setUniforms("camera.z_far", m_farPlaneDist);
	p_program->setUniforms("camera.z_near", m_nearPlaneDist);
	p_program->setUniforms("ViewMatrix", m_viewMatrix);
	p_program->setUniforms("ProjectionMatrix", m_projectionMatrix);
}


