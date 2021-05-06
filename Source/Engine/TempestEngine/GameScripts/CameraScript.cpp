/*!***************************************************************************************
\file       CameraScript.cpp
\author     Dillon Piette
\date       2/05/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the CameraScript class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "CameraScript.hpp"
//========1st Party Includes============================================================//
#include "../Components/InputComponent.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/Camera.hpp"

#include "PlayerControllerScript.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

cameraScript::cameraScript(typeRT p_data): scriptCPP(p_data)
{
}

void cameraScript::onStart()
{
	m_startPos = getGameObject().lock()->getComponent<transform>()->getPosition();
	m_startRot = getGameObject().lock()->getComponent<transform>()->getRotation();
	m_maxPlayers = playerControllerScript::getTotalPlayerNumber();
}

void cameraScript::onUpdate()
{
	// This is so I can easily turn it on and off for now, will remove after it works.
	updateCamera();
}

// This is so I can easily turn it on and off for now, will remove after it works.
void cameraScript::updateCamera()
{
	m_focusPoint = vector3(0, 0, 0);

  if(m_maxPlayers == 0)
  {
    m_maxPlayers = playerControllerScript::getTotalPlayerNumber();
    return;
  }

	// Get all players
	for (auto i_player : findObjectsByComponent<playerControllerScript>())
	{
		// Average the position of players together
		m_focusPoint += i_player->getComponent<transform>()->getPosition();
	}

  vector3 l_players(static_cast<float>(m_maxPlayers), static_cast<float>(m_maxPlayers), static_cast<float>(m_maxPlayers));
  m_focusPoint /= l_players;

	m_maxDist = 0;

	// Get all players
	for (auto i_player : findObjectsByComponent<playerControllerScript>())
	{
		// Find the farthest a player is away from the "averaged position" of the players
		vector3 l_calc_dist = m_focusPoint - i_player->getComponent<transform>()->getPosition();
		float l_dist = l_calc_dist.distance();
		//l_dist = std::abs(l_dist);

		// If this is larger than last frame's max distance
		if (l_dist > m_maxDist)
		{
			m_maxDist = l_dist;
		}
	}

	// Set new camera	transform based on "average position" of all players
	getGameObject().lock()->getComponent<transform>()->setPosition(m_focusPoint);

	// Calculate camera zoom based on max distance from the "average position"
	float l_currZoom = m_maxDist * m_zoom;
	l_currZoom = std::clamp(l_currZoom, m_minZoom, m_maxZoom);

	// Calculate and set the new camera position
	vector3 l_newPos = getGameObject().lock()->getComponent<transform>()->getPosition();
	l_newPos += getGameObject().lock()->getComponent<transform>()->getForward() * l_currZoom;
	getGameObject().lock()->getComponent<transform>()->setPosition(l_newPos);
}