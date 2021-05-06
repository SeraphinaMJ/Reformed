/*!***************************************************************************************
\file       RenderTargetScript.hpp
\author     Cody Cannell
\date       11/14/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Script to control the camera
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Components/ScriptCPP.hpp"

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: inputTestScript
\brief   Uses the input system to move an object around based on keyboard controls.
*****************************************************************************************/
class renderTargetScript final : public scriptCPP
{
    ////////========================================================================////////
    ////////   Public                                                               ////////
    ////////========================================================================////////
public:


	renderTargetScript() = default;
	renderTargetScript(typeRT p_data);
	componentType const type() const override { return renderTargetScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<renderTargetScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "renderTargetScript", true, true, false); }

    /*!***************************************************************************************
    \brief  Handles the update event to process input controls.
    *****************************************************************************************/
    void onUpdate() override;

    //////==============================================================================//////
    //////    (Non-)Static                                                              //////
    //////==============================================================================//////

    ////////========================================================================////////
    ////////   Private                                                              ////////
    ////////========================================================================////////
private:

    //////==============================================================================//////
    //////    (Non-)Static                                                              //////
    //////==============================================================================//////

};


