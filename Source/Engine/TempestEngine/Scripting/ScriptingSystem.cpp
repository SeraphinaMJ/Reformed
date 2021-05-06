/*!***************************************************************************************
\file       ScriptingSystem.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the ScriptingSystem class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "ScriptingSystem.hpp"
//========1st Party Includes============================================================//
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/ScriptCPP.hpp"
#include "../SystemManager.hpp"
//========3rd Party Includes============================================================//
#include <sstream>
#include <windows.h>
#include <ComponentFactory.hpp>
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

void scriptingSystem::onInitialize()
{
}

void scriptingSystem::onUpdate()
{
	GameObjectFiltering::componentTypeSet l_set;
	l_set.setType(scriptCPP::getType());
	auto l_list = getSystemManager()->getGameObjectGatherer()->getList(l_set);
	for(auto & l_script_object : l_list)
	{
		auto l_scripts = l_script_object->getComponents(scriptCPP::getType());
		for(auto l_script_component : l_scripts)
		{
			componentHandle<scriptCPP> l_script_ptr = castHandle<scriptCPP>(l_script_component);
			if (l_script_ptr->isUnstarted())
			{
				l_script_ptr->start(this, this->getSystemManager());
			}
			l_script_ptr->update(this, this->getSystemManager(), getDt());
		}
	}
}

void scriptingSystem::onLevelUnload(const levelUnloadEvent & p_event)
{
	for(auto i_scriptType: componentFactory::getScriptTypeDetails())
	{
		GameObjectFiltering::componentTypeSet l_set;
		l_set.setType(i_scriptType.type);
		auto l_list = getSystemManager()->getGameObjectGatherer()->getList(l_set, p_event.getScene());

		for(auto & l_script_object : l_list)
		{
			auto l_scripts = l_script_object->getComponents(i_scriptType.type);
			for(auto & l_script_component : l_scripts)
			{
				componentHandle<scriptCPP> scriptHandle = castHandle<scriptCPP>(l_script_component);
				(scriptHandle)->shutdown(this, this->getSystemManager());
			}
		}
	}
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

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


