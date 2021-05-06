/*!***************************************************************************************
\file       LightTestScript.hpp
\author     Cody Cannell
\date       10/20/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Script to change propertys of a light
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Components/ScriptCPP.hpp"

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: lightTestScript
\brief   Uses the input system to change the light based on keyboard controls.
*****************************************************************************************/
class lightTestScript final : public scriptCPP
{
    ////////========================================================================////////
    ////////   Public                                                               ////////
    ////////========================================================================////////
public:

	lightTestScript() = default;
	lightTestScript(typeRT p_data);
	componentType const type() const override { return lightTestScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<lightTestScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "lightTestScript", true, true, false); }

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


