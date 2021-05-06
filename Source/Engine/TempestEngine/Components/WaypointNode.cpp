/*!***************************************************************************************
\file       WaypointNode.cpp
\author     Alex Kudebeh
\date       9/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Implementation for the nodes in the waypoint graph
*****************************************************************************************/

//======== Self Include ================================================================//
#include "WaypointNode.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
void waypointNode::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if (l_members.find("debugName") != l_members.end())
	{
		debugName = p_type.member("debugName").getString();
	}
}

typeRT waypointNode::toTypeRT() const
{
	typeRT l_result;
	l_result.setTypeName(type().name());
	l_result.setVariableName(type().name());

	l_result.insertMember(typeRT("debugName", debugName));

	return l_result;
}

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


