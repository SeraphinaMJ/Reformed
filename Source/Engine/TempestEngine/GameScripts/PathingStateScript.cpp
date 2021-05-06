/*!***************************************************************************************
\file       PathingStateScript.cpp
\author     Alex Kudebeh
\date       11/04/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief   Script that holds the current pathing state for an agent
*****************************************************************************************/
//======== Self Include ================================================================//
#include "PathingStateScript.hpp"

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

pathingStateScript::pathingStateScript(typeRT p_data)
{
}

typeRT pathingStateScript::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

//======== Getters & Setters ===========================================================//
void pathingStateScript::setState(pathingStateScript::PathState newState)
{
	m_state = newState;
}

pathingStateScript::PathState pathingStateScript::getState() const
{
	return m_state;
}


//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//

