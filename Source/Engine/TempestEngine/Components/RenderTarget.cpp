/*!***************************************************************************************
\file       RenderTarget.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "RenderTarget.hpp"

const char * PERSISTENCENAME_PLAYBACKCHANNEL = "playBackChannel";
const char * PERSISTENCENAME_FEEDCHANNEL = "feedChannel";
const char * PERSISTENCENAME_PLAYSTATE = "playState";

renderTarget::renderTarget(typeRT & p_type_data)
{
	updateFromTypeRT(p_type_data);
}

typeRT renderTarget::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	typeRT playBackChannelRT = typeRT(PERSISTENCENAME_PLAYBACKCHANNEL, static_cast<int>(m_playBackChannel));
	result.insertMember(playBackChannelRT);

	typeRT feedChannelRT = typeRT(PERSISTENCENAME_FEEDCHANNEL, static_cast<int>(m_feedChannel));
	result.insertMember(feedChannelRT);

	typeRT playStateRT = typeRT(PERSISTENCENAME_PLAYSTATE, static_cast<int>(m_playState));
	result.insertMember(playStateRT);

	return result;
}

void renderTarget::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if (l_members.find(PERSISTENCENAME_PLAYBACKCHANNEL) != l_members.end())
	{
		m_playBackChannel = static_cast<unsigned int>(p_type.member(PERSISTENCENAME_PLAYBACKCHANNEL).getInt());
	}
	if (l_members.find(PERSISTENCENAME_FEEDCHANNEL) != l_members.end())
	{
		m_feedChannel = static_cast<unsigned int>(p_type.member(PERSISTENCENAME_FEEDCHANNEL).getInt());
	}
	if (l_members.find(PERSISTENCENAME_PLAYSTATE) != l_members.end())
	{
		m_playState = static_cast<play_state>(p_type.member(PERSISTENCENAME_PLAYSTATE).getInt());
	}
}
