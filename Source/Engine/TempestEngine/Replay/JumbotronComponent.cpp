/*!**********************************************************************************************************
\file       JumbotronComponent.cpp
\author     Cody Cannell
\date       11/2/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief 
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "JumbotronComponent.hpp"
#include "InstantReplaySystem.hpp"
#include "../Components/Camera.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"

//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Static Deceleration  ==========================================================================//
int jumbotronComponent::m_object_count = 0; //begin count at 0, but include logic to skip main camera index of 0

//========  Static Class Methods  =========================================================================//


jumbotronComponent::jumbotronComponent(float p_initial_offset) : m_is_live(true), m_needs_recording(false), m_replay_duration(0.0f)
{
	if (p_initial_offset)
		BeginInstantReplay(p_initial_offset);

	//set id, increment
	m_id = m_object_count++;
}

jumbotronComponent::jumbotronComponent(typeRT& p_type)
{
	m_is_live = p_type.member("IsLive").getBool();
	m_id = p_type.member("ID").getInt();
	m_needs_recording = p_type.member("NeedsRecording").getBool();
	m_replay_duration = p_type.member("ReplayDuration").getFloat();
}

void jumbotronComponent::BeginInstantReplay(float p_replay_duration)
{
	m_is_live = false;
	m_needs_recording = true;

	//dont need the global timestamp yet.
	//OffsetTimeStamp(p_replay_duration);

	//set the replay duration to the offset
	m_replay_duration = p_replay_duration;
}

bool jumbotronComponent::IsLive()
{
	return m_is_live;
}

void jumbotronComponent::SetIsLive(bool p_is_live)
{
	m_is_live = p_is_live;
}

void jumbotronComponent::SetNeedsRecording(bool p_needs_recording)
{
	m_needs_recording = p_needs_recording;
}

int jumbotronComponent::GetID()
{
	return m_id;
}

void jumbotronComponent::UpdateReplayDuration(float p_time_difference)
{
	m_replay_duration -= p_time_difference;

	if(m_replay_duration <= 0)
	{
		m_replay_duration = 0;
		m_is_live = true;
		m_needs_recording = false;
	}
}

float jumbotronComponent::GetReplayDuration()
{
	return m_replay_duration;
}

void jumbotronComponent::SetCameraCapture()
{

}

bool jumbotronComponent::NeedsRecording()
{
	return m_needs_recording;
}

typeRT jumbotronComponent::toTypeRT() const
{
	typeRT l_result;
	l_result.setTypeName(getType().name());
	l_result.setVariableName(getType().name());

	std::vector<typeRT> l_members;
	typeRT l_is_live("IsLive", m_is_live);
	l_members.push_back(l_is_live);
	typeRT l_id("ID", m_id);
	l_members.push_back(l_id);
	typeRT l_needs_recording("NeedsRecording", m_needs_recording);
	l_members.push_back(l_needs_recording);
	typeRT l_replay_duration("ReplayDuration", m_replay_duration);
	l_members.push_back(l_replay_duration);

	for (auto & i_member : l_members)
	{
		l_result.insertMember(i_member);
	}

	return l_result;
}

void jumbotronComponent::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if (l_members.find("IsLive") != l_members.end())
	{
		m_is_live = p_type.member("IsLIve").getBool();
	}
	if (l_members.find("ID") != l_members.end())
	{
		m_id = p_type.member("ID").getBool();
	}
	if (l_members.find("NeedsRecording") != l_members.end())
	{
		m_needs_recording = p_type.member("NeedsRecording").getBool();
	}
	if (l_members.find("ReplayDuration") != l_members.end())
	{
		m_replay_duration = p_type.member("ReplayDuration").getBool();
	}
}
