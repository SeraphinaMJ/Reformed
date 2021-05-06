/*!***************************************************************************************
\file       ScriptCPP.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the ScriptCPP class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "ScriptCPP.hpp"
//========1st Party Includes============================================================//
#include "../Scripting/ScriptingSystem.hpp"
#include "../SceneManagement/SceneSystem.hpp"
#include  "../SceneManagement/Scene.hpp"
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

void scriptCPP::start(scriptingSystem* p_parent_system, systemManagerInterface * p_system_manager)
{
    m_system = p_parent_system;
    m_system_manager = p_system_manager;

    onStart();

    m_state = RUNNING;
}

void scriptCPP::update(scriptingSystem * p_parent_system, systemManagerInterface * p_system_manager, float p_dt)
{
    m_system = p_parent_system;
    m_system_manager = p_system_manager;
    m_dt = p_dt;

    onUpdate();
}

void scriptCPP::shutdown(scriptingSystem* p_parent_system, systemManagerInterface* p_system_manager)
{
	m_system = p_parent_system;
	m_system_manager = p_system_manager;

	onShutdown();

	m_state = UNSTARTED;
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

scriptCPP::scriptCPP() : EventSystem3::EventHandler("scriptCPP"), m_state{UNSTARTED}, m_dt{0}
{
    // EH_INITIALIZE();
}

scriptCPP::scriptCPP(typeRT& p_data): EventSystem3::EventHandler("scriptCPP")
{
    // EH_INITIALIZE();

	m_state = UNSTARTED;
	updateFromTypeRT(p_data);
}

bool scriptCPP::isUnstarted() const
{
    return m_state == UNSTARTED;
}

bool scriptCPP::isRunning() const
{
    return m_state == RUNNING;
}


scriptingSystem* scriptCPP::getSystem() const
{
    return m_system;
}

systemManagerInterface* scriptCPP::getSystemManager() const
{
    return m_system_manager;
}

float scriptCPP::dt() const
{
    return m_dt;
}

std::shared_ptr<gameObject> scriptCPP::findObject(std::string p_object_name)
{
	return m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock()->findGameObject(p_object_name);
}

std::shared_ptr<gameObject> scriptCPP::findObject(objID p_objId)
{
	return m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock()->findGameObjectByID(p_objId);
}

space* scriptCPP::getMySpace()
{
	return getGameObject().lock()->getSpace();
}

std::shared_ptr<gameObject> scriptCPP::createGameObjectOnMySpace()
{
	return getMySpace()->instantiateGameObject();
}

std::shared_ptr<gameObject> scriptCPP::createGameObjectOnMySpace(std::string p_name)
{
	return getMySpace()->instantiateGameObject(p_name);
}

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


void scriptCPP::updateFromTypeRT(typeRT&)
{
}

typeRT scriptCPP::toTypeRT() const
{
	typeRT l_result;
	l_result.setTypeName(type().name());
	l_result.setVariableName(type().name());

	return l_result;
}
