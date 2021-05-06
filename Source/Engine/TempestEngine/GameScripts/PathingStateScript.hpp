/*!***************************************************************************************
\file       PathingStateScript.hpp
\author     Alex Kudebeh
\date       11/04/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief   Script that holds the current pathing state for an agent
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../Components/ScriptCPP.hpp"
#include "Vector3.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class pathingStateScript final : public scriptCPP
{
public:
	enum PathState {
		PSHome = 0,
		PSPathing = 1,
		PSStop = 2,
		PSCount,
	};

	pathingStateScript() = default;

	/*!***************************************************************************************
	\brief  Conversion constructor. Copies move speed
	\param  p_data - The typeRT to convert this from
	*****************************************************************************************/
	pathingStateScript(typeRT p_data);

	/*!***************************************************************************************
	\brief  Type getter
	\return The type of component this is
	*****************************************************************************************/
	componentType const type() const override { return pathingStateScript::getType(); }

	/*!***************************************************************************************
	\brief  Static type getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forInherited<pathingStateScript, scriptCPP>(); return s_type; }

	/*!***************************************************************************************
	\brief  Override for type detail getter
	\retrun The component type details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "PathingStateScript", true, true, true); }

	/*!***************************************************************************************
	\brief  Converts the script to a typeRT.
	\return The scripts as a typeRT
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Set the state this agent is in
	\param  newState - The new state for this agent
	*****************************************************************************************/
	void setState(PathState newState);

	/*!***************************************************************************************
	\brief  Getter for the agent's state
	\return The state the agent is in
	*****************************************************************************************/
	PathState getState() const;





protected:
private:
	//!The current state of this agent
	PathState m_state = PathState::PSHome;

};
/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

