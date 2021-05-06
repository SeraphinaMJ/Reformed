/*!***************************************************************************************
\file       WaypointNodeSet.cpp
\author     Alex Kudebeh
\date       9/24/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implementation for a waypoint node set. This is a set of nodes that creates a
		path around an object
*****************************************************************************************/

//======== Self Include ================================================================//
#include "WaypointNodeSet.hpp"
//======== 1st Party Includes ==========================================================//
#include "Vector3Serialization.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

void waypointNodeSet::updateFromTypeRT(typeRT & p_type)
{
	auto l_members = p_type.members();

	if (l_members.find("debugName") != l_members.end())
	{
		debugName = p_type.member("debugName").getString();
	}
	if (l_members.find("Node 1") != l_members.end())
	{
		nodeOne = (::toVector3(p_type.member("Node 1")));
	}
	if (l_members.find("Node 2") != l_members.end())
	{
		nodeTwo = (::toVector3(p_type.member("Node 2")));
	}
	if (l_members.find("Node 3") != l_members.end())
	{
		nodeThree = (::toVector3(p_type.member("Node 3")));
	}
	if (l_members.find("Node 4") != l_members.end())
	{
		nodeFour = (::toVector3(p_type.member("Node 4")));
	}
}

typeRT waypointNodeSet::toTypeRT() const
{
	typeRT l_result;
	l_result.setTypeName(type().name());
	l_result.setVariableName(type().name());

	l_result.insertMember(typeRT("debugName", debugName));

	typeRT one = ::toTypeRT(nodeOne);
	one.setVariableName("Node 1");
	l_result.insertMember(one);

	typeRT two = ::toTypeRT(nodeTwo);
	two.setVariableName("Node 2");
	l_result.insertMember(two);

	typeRT three = ::toTypeRT(nodeThree);
	three.setVariableName("Node 3");
	l_result.insertMember(three);

	typeRT four = ::toTypeRT(nodeFour);
	four.setVariableName("Node 4");
	l_result.insertMember(four);

	return l_result;
}

//======== Helper Functions ============================================================//


