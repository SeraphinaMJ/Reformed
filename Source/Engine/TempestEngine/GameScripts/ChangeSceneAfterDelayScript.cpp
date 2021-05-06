/*!***************************************************************************************
\file       ChangeSceneAfterDelayScript.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "ChangeSceneAfterDelayScript.hpp"

changeSceneAfterDelayScript::changeSceneAfterDelayScript()
{
}

changeSceneAfterDelayScript::changeSceneAfterDelayScript(typeRT p_data)
{
	updateFromTypeRT(p_data);
}

typeRT changeSceneAfterDelayScript::toTypeRT() const
{
	// Generate the base script data
	typeRT l_result = scriptCPP::toTypeRT();

	// add data to the l_scriptData object here.
	l_result.insertMember(typeRT("delay", m_delay));
	l_result.insertMember(typeRT("sceneName", m_scene_name));

	return l_result;
}

void changeSceneAfterDelayScript::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("delay") != l_members.end())
	{
		m_delay = p_type.member("delay").getFloat();
	}
	if(l_members.find("sceneName") != l_members.end())
	{
		m_scene_name = p_type.member("sceneName").getString();
	}
}

void changeSceneAfterDelayScript::onStart()
{
	m_time_remaining = m_delay;
}

void changeSceneAfterDelayScript::onUpdate()
{
	m_time_remaining -= dt();
	if (m_time_remaining <= 0)
	{
		auto l_scene = getSystemManager()->getSystem<sceneSystem>()->getScene(m_scene_name);
		if (l_scene == nullptr)
		{
			// load the scene
			l_scene = getSystemManager()->getSystem<sceneSystem>()->loadScene(m_scene_name);
		}
		getSystemManager()->getSystem<sceneSystem>()->setCurrentScene(l_scene);
	}
}
