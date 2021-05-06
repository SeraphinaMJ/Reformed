/*!***************************************************************************************
\file       SystemBase.cpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Implementation of the system class public members.
*****************************************************************************************/
//======== Self Include ================================================================//
#include "SystemBase.hpp"
//======== 1st Party Includes ==========================================================//
#include "SystemManagement/SystemManagerInterface.hpp"
// #include "Events/EventSystem.hpp"
#include "SceneManagement/SceneSystem.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Logger.hpp>
#include <typeindex>
#include <thread>

#include "Message.hpp"

// #include <EventTemplate.hpp>
#include "EventBase.hpp"
#include "EventBus.hpp"
#include "EventSubscribe.hpp"

//======== Types =======================================================================//
class preShutdownEvent : public EventSystem3::EventBase
{};

//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

systemBase::systemBase(): EventHandler("systemBase") {
    EH_INITIALIZE();
}

void systemBase::initialize(systemManagerInterface *p_system_manager)
{
	if (m_state != systemState::enm_Uninitialized)
		logger("systemBase").error() << "Initializing a system that is already initialized: " << this->name();
	m_system_manager = p_system_manager;
	onInitialize();
	m_state = systemState::enm_Running;
}

void systemBase::startFrame()
{
	onStartFrame();
}

void systemBase::update(float p_dt)
{
	m_dt = p_dt;
	onUpdate();
}

void systemBase::endFrame()
{
	onEndFrame();
}

void systemBase::shutdown()
{
	if(m_state != systemState::enm_Running)
		logger("SystemBase").error() << "Shutting down " << this->name() << " on thread " << std::this_thread::get_id();
	onShutdown();
	m_state = systemState::enm_Uninitialized;
	//logger("SystemBase").warn("System Shutdown!!!");
}

void systemBase::handleMessage(message& p_message)
{
	onHandleMessage(p_message);
}
SUBSCRIBE(message, &systemBase::handleMessage, EHCLASS(systemBase),);

systemBase::systemState systemBase::getState()
{
  return m_state;
}


void systemBase::onShutdown()
{

}

systemManagerInterface* systemBase::getSystemManager() const
{
	return m_system_manager;
}

//std::weak_ptr<gameObject> systemBase::getGameObjectByID(objID id) const
//{
//
//    return this->m_system_manager->getSystem<sceneSystem>()->getGameObjectByID(id);
//}

void systemBase::handleLoadLevelEvent(levelLoadEvent& p_event)
{
	if(m_state == systemState::enm_Running)
		onLevelLoad(p_event);
}
SUBSCRIBE(levelLoadEvent, &systemBase::handleLoadLevelEvent, EHCLASS(systemBase),);

void systemBase::handleUnloadLevelEvent(levelUnloadEvent& p_event)
{
	if(m_state == systemState::enm_Running)
		onLevelUnload(p_event);
}
SUBSCRIBE(levelUnloadEvent, &systemBase::handleUnloadLevelEvent, EHCLASS(systemBase),);

void systemBase::handlePreShutdownEvent(preShutdownEvent& /*p_event*/)
{
	if(m_state == systemState::enm_Running)
		onPreShutdown();
}
SUBSCRIBE(preShutdownEvent, &systemBase::handlePreShutdownEvent, EHCLASS(systemBase),);

//======== Helper Functions ============================================================//
