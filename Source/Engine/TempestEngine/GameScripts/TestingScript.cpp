/*!***************************************************************************************
\file       TestingScript.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the TestingScript class member function.
*****************************************************************************************/

//========Self Include==================================================================//
#include "TestingScript.hpp"
//========1st Party Includes============================================================//
#include "../Events/TestEvent.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../TransformTestScript.hpp"
#include "PlayerControllerScript.hpp"
//========3rd Party Includes============================================================//
#include "EventSubscribe.hpp"
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

void testingScript::onStart()
{
	// registerEventCallback<testingScript, testEvent, &testingScript::HandleEvent>();
}

void testingScript::onUpdate()
{
	//sendEvent(new testEvent("test", 5));

 /* auto t_test_scripts = getSystemManager()->getGameObjectGatherer()->getScriptList(testingScript::getSubType());

	for(auto i_script : t_test_scripts)
	{
		if(i_script == getGameObject().lock())
		{
            int i = 5;
            ++i;
		}
	}*/
}

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>

testingScript::testingScript(typeRT p_data) : scriptCPP(p_data)
{
}

void testingScript::HandleEvent(testEvent& ThisEvent)
{
	if(ThisEvent.m_int == 5)
	{
		//throw std::exception("error");
	}
}
SUBSCRIBE(testEvent, &testingScript::HandleEvent, EHCLASS(testingScript),);

