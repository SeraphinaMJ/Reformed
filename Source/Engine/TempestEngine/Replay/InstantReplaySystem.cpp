/*!**********************************************************************************************************
\file       InstantReplaySystem.cpp
\author     Cole Astaire
\date       10/30/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "InstantReplaySystem.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "JumbotronComponent.hpp"
#include "../Components/Camera.hpp"
#include "../Components/RenderTarget.hpp"
//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Static Deceleration  ==========================================================================//

//========  Static Class Methods  =========================================================================//

void instantReplaySystem::onInitialize()
{
	//set the current time counter
	m_current_time_stamp = 0.0f;

}

void instantReplaySystem::onUpdate()
{
	//update my current time
	m_current_time_stamp += getDt();

	//get each object and component
	GameObjectFiltering::componentTypeSet l_jumbos;
	l_jumbos.setType(jumbotronComponent::getType());
	GameObjectFiltering::componentTypeSet l_cameras;
	l_cameras.setType(camera::getType());
	GameObjectFiltering::componentTypeSet l_renders;
	l_renders.setType(renderTarget::getType());

	//ask object gatherer for all objects with jumbotron components, all cameras, and all rendertargets
	auto l_jumbo_objects = getSystemManager()->getGameObjectGatherer()->getList(l_jumbos);
	auto l_camera_objects = getSystemManager()->getGameObjectGatherer()->getList(l_cameras);
	auto l_render_objects = getSystemManager()->getGameObjectGatherer()->getList(l_renders);

	//update jumbotron components
	for(auto & l_obj : l_jumbo_objects)
	{
		//get correct component
		auto l_comp = l_obj->getComponent<jumbotronComponent>();

		l_comp->UpdateReplayDuration(0.02f); //TODO: replace hardcoded input with dt() once the previous frame isnt so slow. currently the stall from the graphics system on the previous frame causes replay to be cut short
	}

	//for each jumbotron
	for (auto & l_obj : l_jumbo_objects)
	{
		//get jumbo component
		auto l_jumbo = l_obj->getComponent<jumbotronComponent>();

		size_t i = l_jumbo->GetID();

		////script behavior TODO: remove, and add to scripts
		//if (static_cast<int>(m_current_time_stamp) % 10 == 0)
		//{
		//	l_jumbo->BeginInstantReplay(1.0f);

		//	////get each camera, and save recording
		//	//for(auto & l_camera : l_camera_objects)
		//	//{
		//	//	//get correct components
		//	//	auto l_camera_comp = l_camera->getComponent<camera>();

		//	//	l_camera_comp->saveCapture();
		//	//}
		//}

		//check for record flag
		if (l_jumbo->NeedsRecording() == true)
		{
			////record corresponding camera
			//TODO: move to this variable camera capture functionality once able
			//auto l_cam = l_camera_objects[1]->getComponent<camera>(); //add 1 to skip the main camera, assumedly at index 0
			//l_cam->saveCapture();

			//get each camera, and save recording
			//TODO: currently, all cameras must savecapture() in order to replay any of them
			for (auto & l_camera : l_camera_objects)
			{
				//get correct components
				auto l_camera_comp = l_camera->getComponent<camera>();

				l_camera_comp->saveCapture();
			}

			//reset recording flag
			l_jumbo->SetNeedsRecording(false);
		}

		//if not live
		if (l_jumbo->IsLive() == false)
		{
			if (i < l_render_objects.size())
			{
				//set corresponding rendertarget to playback
				auto l_render = l_render_objects[i]->getComponent<renderTarget>();
				l_render->changePlayState(play_state::enum_playback);
			}
		} else
		{
			if (i < l_render_objects.size())
			{
				////set corresponding rendertarget to feed
				auto l_render = l_render_objects[i]->getComponent<renderTarget>();
				l_render->changePlayState(play_state::enum_feed);
			}
		}
	}

	////test
	//std::cout << "current_play_time: " << m_current_time_stamp << std::endl;

	////get the list of gameobjects with a jumbotron component
	//GameObjectFiltering::componentTypeSet l_jumbos;
	//l_jumbos.setType(jumbotronComponent::getType());

	////get the list of gameobjects with a camera component
	//GameObjectFiltering::componentTypeSet l_cameras;
	//l_cameras.setType(camera::getType());

	////get the list of gameobjects with a rendertarget component
	//GameObjectFiltering::componentTypeSet l_renders;
	//l_renders.setType(renderTarget::getType());

	////ask object gatherer for all objects with jumbotron components, all cameras, and all rendertargets
	//auto l_jumbo_objects = getSystemManager()->getGameObjectGatherer()->getList(l_jumbos);
	//auto l_camera_objects = getSystemManager()->getGameObjectGatherer()->getList(l_cameras);
	//auto l_render_objects = getSystemManager()->getGameObjectGatherer()->getList(l_renders);

	////for each jumbotron component, check timers
	//for (auto & l_obj : l_jumbo_objects)
	//{
	//	//get correct components
	//	auto l_jumbo_comp = l_obj->getComponent<jumbotronComponent>();

	//	//update their current timestamp by delta time
	//	l_jumbo_comp->UpdateTimeStamp(dt());

	//	//if time stamps match, set as live
	//	if (l_jumbo_comp->GetTimeStamp() == m_current_time_stamp)
	//	{
	//		l_jumbo_comp->SetIsLive(true);
	//		std::cout << "Live" << std::endl;

	//		//test
	//		if (static_cast<int>(m_current_time_stamp) % 10 == 0)
	//		{
	//			std::cout << "Testing start replay" << std::endl;

	//			l_jumbo_comp->BeginInstantReplay(1.0f);

	//			//get each camera, and save recording
	//			for(auto & l_camera : l_camera_objects)
	//			{
	//				//get correct components
	//				auto l_camera_comp = l_camera->getComponent<camera>();

	//				l_camera_comp->saveCapture();
	//			}

	//			//get each rendertarget, and change state to playing
	//			for(auto & l_rendertarget : l_render_objects)
	//			{
	//				auto l_render_comp = l_rendertarget->getComponent<renderTarget>();

	//				l_render_comp->changePlayState(play_state::enum_playback);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		std::cout << "Replay Time: " << l_jumbo_comp->GetTimeStamp() << std::endl;
	//		std::cout << "Replay Duration: " << l_jumbo_comp->GetReplayDuration() << std::endl;
	//		//if time stamps are within delta, set to live
	//		if(l_jumbo_comp->GetReplayDuration() == 0)
	//		{
	//			l_jumbo_comp->SetTimeStamp(m_current_time_stamp);
	//			l_jumbo_comp->SetIsLive(true);
	//		}
	//	}
	//}
}

void instantReplaySystem::onShutdown()
{

}
