/*!***************************************************************************************
\file       TestingScript.hpp
\author     Aaron Damyen
\date       6/19/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Script component to verify the scripting system is operating properly.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Components/ScriptCPP.hpp"
#include "../Events/TestEvent.hpp"

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: testingScript
\brief   This script component tests the scripting system to ensure operation.
*****************************************************************************************/
class testingScript : public scriptCPP
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	testingScript() = default;
	testingScript(typeRT p_data);
	componentType const type() const override { return testingScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<testingScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "testingScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;

	void HandleEvent(testEvent& ThisEvent);

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

