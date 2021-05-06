/*!***************************************************************************************
\file       ObjectSelectorManipulator.cpp
\author     Seraphina Kim
\date       9/10/19
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "BulletWrapper.hpp"
#include "RigidBody.hpp"
#include "ObjectSelector.hpp"
#include "SystemManagerInterface.hpp"
#include <Logger.hpp>
#include <cmath>
#include <cfloat>
#include <random>



Editor::EditorLogger* objectSelector::editorlogger = nullptr;

objectSelector::objectSelector()
	: _camera(), m_other(nullptr), collisionstate(false), count(0)
{

}
bool objectSelector::isSelected(ImVec2 winpos)
{
	if (!_camera.expired())
	{
			ImGuiIO& io = ImGui::GetIO();
			float mouseX, mouseY;
			float texture_w = 1024;
			float texture_h = 768;

			// Get Matricies
			float viewMat[16];
			float projMat[16];
			_camera.lock()->readCameraMatrices(viewMat, projMat);
			matrix4x4 view_mat(viewMat);
			matrix4x4 proj_mat(projMat);

			//editorlogger->AddLog((std::to_string(ImGui::GetMousePos().x) + "," + std::to_string(ImGui::GetMousePos().y)  + "\n").c_str());

			mouseX = (2.0f * ImGui::GetMousePos().x - winpos.x) / texture_w - 1.0f;
			mouseY =  -((2.0f * (ImGui::GetMousePos().y - winpos.y)) / texture_h - 1.0f);
			//editorlogger->AddLog((std::to_string(winpos.x) + "," + std::to_string(winpos.y) + "\n").c_str());

			float far_distance = _camera.lock()->getFarDistance();

			vector4 ray_clip = vector4(mouseX, mouseY, -1.0f, 1.0f);
			vector4 ray_eye = proj_mat.inverse() * ray_clip;
					ray_eye.z = -1.0f;
					ray_eye.w = 0.0f;

			ray_eye.normalized();

			vector3 origin_of_ray = vector3(_camera.lock()->getCameraPosX(), _camera.lock()->getCameraPosY(), _camera.lock()->getCameraPosZ());
			vector3 ray_world = (view_mat.inverse() * ray_eye).toVector3().normalize();
			vector3 endpoint = origin_of_ray + 1000 * ray_world;

			btVector3 outNormal;

			if (!isnan(origin_of_ray.x) && !isnan(origin_of_ray.y) && !isnan(origin_of_ray.z))
			{
				btVector3 p_normal(0, 0, 0);
				btVector3 p_start(origin_of_ray.x, origin_of_ray.y, origin_of_ray.z);
				btVector3 p_end(endpoint.x, endpoint.y, endpoint.z);

				int layer = enm_trigger; //| enm_world | enm_player | enm_enemy | enm_trigger;

				if (bulletWrapper::checkCollision(nullptr, p_start, // start point
														   p_end, //end point
														   outNormal, layer, m_other))
				{
					if (m_other)
					{
						if (collisionstate == false)
						{
							collisionstate = true;

							if (editorlogger)
								editorlogger->AddLog("collision started\n");
						}
					}
				}
				else
				{
					if (collisionstate)
					{
						collisionstate = false;

						if (editorlogger)
							editorlogger->AddLog("collision ended\n");
					}
				}
			}

	}
	return false;
}

void objectSelector::setRayCastCamera(std::weak_ptr<cameraManipulatorInterface>  p_camera, Editor::EditorLogger& logger)
{
		_camera = p_camera;
		editorlogger = &logger;
}

bool objectSelector::mousePickingIntersectionTest(int layer, vector3 & intersectionPoint, vector3 & intersectionNormal)
{
	if (!_camera.expired())
	{
		ImGuiIO& io = ImGui::GetIO();
		float mouseX, mouseY;
		float texture_w = 1024;
		float texture_h = 768;
		ImVec2 windowPos = { 20,77 };

		// Get Matricies
		float viewMat[16];
		float projMat[16];
		_camera.lock()->readCameraMatrices(viewMat, projMat);

		matrix4x4 view_mat(viewMat);
		matrix4x4 proj_mat(projMat);

		mouseX = (2.0f * ImGui::GetMousePos().x - windowPos.x) / texture_w - 1.0f;
		mouseY = -((2.0f * (ImGui::GetMousePos().y - windowPos.y)) / texture_h - 1.0f);

		float far_distance = _camera.lock()->getFarDistance();

		vector4 ray_clip = vector4(mouseX, mouseY, -1.0f, 1.0f);
		vector4 ray_eye = proj_mat.inverse() * ray_clip;
		ray_eye.z = -1.0f;
		ray_eye.w = 0.0f;

		ray_eye.normalized();

		vector3 origin_of_ray = vector3(_camera.lock()->getCameraPosX(), _camera.lock()->getCameraPosY(), _camera.lock()->getCameraPosZ());
		vector3 ray_world = (view_mat.inverse() * ray_eye).toVector3().normalize();
		vector3 endpoint = origin_of_ray + 1000 * ray_world;

		btVector3 outNormal;

		if (!isnan(origin_of_ray.x) && !isnan(origin_of_ray.y) && !isnan(origin_of_ray.z))
		{
			btVector3 p_normal(0, 0, 0);

			btVector3 p_start(origin_of_ray.x, origin_of_ray.y, origin_of_ray.z);
			btVector3 p_end(endpoint.x, endpoint.y, endpoint.z);

			if (bulletWrapper::checkCollision(nullptr, p_start, p_end, outNormal, layer, m_other))
			{
				if (m_other)
				{
					intersectionNormal.x = outNormal[0];
					intersectionNormal.y = outNormal[1];
					intersectionNormal.z = outNormal[2];

					intersectionPoint.x = p_end[0];
					intersectionPoint.y = p_end[1];
					intersectionPoint.z = p_end[2];

					return true;
				}
			}
		}

	}
	return false;
}
