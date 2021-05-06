/*!***************************************************************************************
\file       ScoreTracker.hpp
\author     Alex Kudebeh
\date       12/3/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "ScriptCPP.hpp"
#include "UISprite.hpp"
#include "EnemyDeathEvent.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: scoreTracker
\brief   Tracks the score, and ends the game when player dies
*****************************************************************************************/
class scoreTracker final : public scriptCPP
{
/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/
public:
	/*!***************************************************************************************
	\brief  Default constructor
	*****************************************************************************************/
	scoreTracker();

	void HandleEvent(EnemyDeathEvent& event);

	/*!***************************************************************************************
	\brief  Conversion constructor
	\param  p_data - The typeRT data to convert from
	*****************************************************************************************/
	scoreTracker(typeRT & p_data);

	/*!***************************************************************************************
	\brief  Type getter
	\return The type of component this is
	*****************************************************************************************/
	componentType const type() const override { return scoreTracker::getType(); }

	/*!***************************************************************************************
	\brief  Static type getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forInherited<scoreTracker, scriptCPP>(); return s_type; }

	/*!***************************************************************************************
	\brief  Override for type detail getter
	\retrun The component type details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Score Tracker", true, true, true); }

	/*!***************************************************************************************
	\brief  Converts the script to a typeRT.
	\return The script as a typeRT
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Updates the script from a typeRT
	\return The script as a typeRT
	*****************************************************************************************/
	void updateFromTypeRT(typeRT & p_type) override;


	void addKill();

	void showPauseMenu();

    void hidePauseMenu();

    void setShowKills(bool p_show_kills) { m_show_kills = p_show_kills; }

	/*!***************************************************************************************
	\brief  Override for onUpdate
	*****************************************************************************************/
	void onUpdate() override;

	void onStart() override;

private:
	int m_killCount = 0;

	bool m_gameOver = false;

    bool m_pause_open = false;

    float m_timer = 0.0f;

    bool m_show_kills = false;
};
