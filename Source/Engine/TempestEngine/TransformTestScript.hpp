/*!***************************************************************************************
\file       TransformTestScript.hpp
\author     Aaron Damyen
\date       6/28/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This script tests the functionality of the transform component.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "Components/ScriptCPP.hpp"

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: transformTestScript
\brief   This script component is used to exercise all functionality of the transform component.
*****************************************************************************************/
class transformTestScript : public scriptCPP
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	transformTestScript() = default;
	transformTestScript(typeRT p_data);
	componentType const type() const override { return transformTestScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<transformTestScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "transformTestScript", true, true, false); }

	/*!***************************************************************************************
	\brief  Handles the start event to initialize itself.
	*****************************************************************************************/
	void onStart() override;
	/*!***************************************************************************************
	\brief  Handles the update event to update itself.
	*****************************************************************************************/
	void onUpdate() override;
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	int mode = 0;
	float value = 0.0f;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};


