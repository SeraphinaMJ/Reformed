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
#include "ObjectSelectorManipulator.hpp"
#include "SystemManagerInterface.hpp"
#include <Logger.hpp>
#include <Matrix4x4.hpp>
#include <cmath>
#include <cfloat>
#include <random>
 


Editor::EditorLogger* objectSelectorManipulator::editorlogger = nullptr;

objectSelectorManipulator::objectSelectorManipulator()
	: _camera(), collisionstate(false), m_other(nullptr), count(0)
{
	
}
bool objectSelectorManipulator::isSelected(ImVec2 winpos)
{
	if (!_camera.expired())
	{
		if (count == 0)
		{
			ImGuiIO& io = ImGui::GetIO();
			float mouseX, mouseY;
			float texture_w = 1024;//_camera.lock()->getWidth();
			float texture_h = 768;//_camera.lock()->getHeight();
			//mouseX = io.MousePos.x - ImGui::GetWindowPos().x;
			//mouseY = io.MousePos.y - ImGui::GetWindowPos().y;

			// NDC 

			// Get Matricies
			float viewMat[16];
			float projMat[16];
			_camera.lock()->readCameraMatrices(viewMat, projMat);
			matrix4x4 view_mat(viewMat);
			matrix4x4 proj_mat(projMat);


			//editorlogger->AddLog((std::to_string(ImGui::GetMousePos().x) + "," + std::to_string(ImGui::GetMousePos().y)  + "\n").c_str());

// ==============================Online Example=================================

			
			mouseX = (2.0f * ImGui::GetMousePos().x - winpos.x) / texture_w - 1.0f;
			mouseY =  -((2.0f * (ImGui::GetMousePos().y - winpos.y)) / texture_h - 1.0f);


			///ImVec2 min = ImGui::GetWindowContentRegionMin();
			//mouseX = (2.0f * (ImGui::GetMousePos().x - ImGui::GetWindowPos().x - min.x )) / texture_w - 1.0f;
			//ImDrawList* dl = ImGui::GetWindowDrawList();
			
			//mouseY = -((2.0f * (ImGui::GetMousePos().y - ImGui::GetWindowPos().y - min.y - ImGui::GetFrameHeight())) / texture_h - 1.0f);


			//editorlogger->AddLog((std::to_string(min.x) + "," + std::to_string(min.y) + "\n").c_str());

			//editorlogger->AddLog((std::to_string((ImGui::GetMousePos().x - ImGui::GetWindowPos().x)) + "," + std::to_string(ImGui::GetMousePos().y - ImGui::GetWindowPos().y) + "\n").c_str());

			//editorlogger->AddLog((std::to_string(mouseXX) + "," + std::to_string(mouseYY) + "\n").c_str());
			//editorlogger->AddLog((std::to_string(mouseX) + "," + std::to_string(mouseY) + "\n").c_str());

			float far_distance = _camera.lock()->getFarDistance();
			
			vector4 ray_clip = vector4(mouseX, mouseY, -1.0f, 1.0f);

			vector4 ray_eye = proj_mat.inverse() * ray_clip;
					ray_eye.z = -1.0f;
					ray_eye.w = 0.0f;

			ray_eye.normalize();

			vector3 origin_of_ray = vector3(_camera.lock()->getCameraPosX(), _camera.lock()->getCameraPosY(), _camera.lock()->getCameraPosZ());

			vector3 ray_world = (view_mat.inverse() * ray_eye).toVector3().normalized();

			/*ray_world.x -= 0.012096f;
			ray_world.y += 0.079029f - 0.016924f*/;
			vector3 endpoint = origin_of_ray + 1000 * ray_world;

			//editorlogger->AddLog((std::to_string(ray_world.x) + "," + std::to_string(ray_world.y) + "," + std::to_string(ray_world.z) + "\n").c_str());
			//editorlogger->AddLog((std::to_string(ImGui::GetMousePos().x) + "," + std::to_string(ImGui::GetMousePos().y)  + "\n").c_str());
			//editorlogger->AddLog((std::to_string(mouseX) + "," + std::to_string(mouseY) + "\n").c_str());
			// ===============================================================================

			btVector3 outNormal;

			if (!isnan(origin_of_ray.x) && !isnan(origin_of_ray.y) && !isnan(origin_of_ray.z))
			{
				btVector3 p_normal(0, 0, 0);

				btVector3 p_start(origin_of_ray.x, origin_of_ray.y, origin_of_ray.z);
				btVector3 p_end(endpoint.x, endpoint.y, endpoint.z);

				int layer = enm_player; //| enm_world;// | enm_player | enm_enemy | enm_trigger;

				if (bulletWrapper::checkCollision(nullptr, p_start, // start point
														  p_end, //end point
														outNormal, layer, m_other))
				{
					if (m_other)
					{
						if (collisionstate == false) {
							collisionstate = true;

							if (editorlogger)
							{
								editorlogger->AddLog("collision started\n");
							}
						}
					}
				}
				else
				{
					if (collisionstate) {
						collisionstate = false;

						if (editorlogger)
							editorlogger->AddLog("collision ended\n");
					}
				}
			}
		}
		else
		{
			++count;
			if (count == 100)
				count = 0;
		}

	}

	return false;
}

void objectSelectorManipulator::setRayCastCamera(std::weak_ptr<cameraManipulatorInterface>  p_camera, Editor::EditorLogger& logger)
{
		_camera = p_camera;
		editorlogger = &logger;
}

