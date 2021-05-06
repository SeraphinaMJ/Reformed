/*!***************************************************************************************
\file       CameraScript.hpp
\author     Dillon Piette
\date       2/05/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Script to control the camera
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Components/ScriptCPP.hpp"
#include <Vector3.hpp>
#include <Quaternion.hpp>
//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: inputTestScript
\brief   Uses the input system to move an object around based on keyboard controls.
*****************************************************************************************/
class cameraScript final : public scriptCPP
{
    ////////========================================================================////////
    ////////   Public                                                               ////////
    ////////========================================================================////////
public:

    /*!***************************************************************************************
    \brief  Handles the update event to process input controls.
    *****************************************************************************************/
	cameraScript() = default;
	cameraScript(typeRT p_data);
	componentType const type() const override { return cameraScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<cameraScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "cameraScript", true, true, true); }

	void onStart() override;
	void onUpdate() override;
	void updateCamera();

    //////==============================================================================//////
    //////    (Non-)Static                                                              //////
    //////==============================================================================//////
	float m_zoom = 1;
	float m_minZoom = 75;
	float m_maxZoom = 250;

    ////////========================================================================////////
    ////////   Private                                                              ////////
    ////////========================================================================////////
private:

    //////==============================================================================//////
    //////    (Non-)Static                                                              //////
    //////==============================================================================//////

   bool m_firedOnce = false;
   bool m_dynCamOn = true;
   float m_maxDist = 0;

	// hack
   int m_maxPlayers = 2;

   vector3 m_focusPoint = vector3(0, 0, 0);
   vector3 m_startPos = vector3(0, 0, 0);

   quaternion m_startRot;

};


