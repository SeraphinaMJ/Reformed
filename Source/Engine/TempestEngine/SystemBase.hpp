/*!***************************************************************************************
\file       SystemBase.hpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Definition of the system public interface.  This is the base for all back-end systems
within the engine.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "SystemManagement/SystemManagerInterface.hpp"
//======== 3rd Party Includes ==========================================================//
#include <chrono>
#include <string>
#include "EventHandler.hpp"
//#include <EventTemplate.hpp>
//======== Types =======================================================================//

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
struct message;
class systemManager;
class gameObject;
class levelLoadEvent;
class levelUnloadEvent;
class preShutdownEvent;

typedef std::weak_ptr<gameObject> GameObjectPtr;

/*!*************************************************************************************
\par class: systemBase
\brief   This is the base class for all engine systems.  Provides a common interface into the system.
***************************************************************************************/
class systemBase: public EventSystem3::EventHandler
{
public:
	systemBase();

	/*!*******************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*********************************************************************************/
	virtual const std::string& name() const = 0;

	/*!***************************************************************************************
	\par enum: systemState
	\brief Records the current state of this system
	*****************************************************************************************/
	enum class systemState
	{
		enm_Uninitialized,
		enm_Running,
	};

	/*!***************************************************************************************
	\brief  Alows inherited clases te destory.
	*****************************************************************************************/
	virtual ~systemBase() = default;

	/*!***************************************************************************************
	\brief  Starts the initialization process for the derived system.
	\param p_system_manager - that owns this system
	*****************************************************************************************/
	void initialize(systemManagerInterface * p_system_manager);

	/*!***************************************************************************************
	\brief  Starts the frame for the derived system.
	*****************************************************************************************/
	void startFrame();

	/*!***************************************************************************************
	\brief  Starts the update process for the derived system.
	*****************************************************************************************/
	void update(float p_dt);

	/*!***************************************************************************************
	\brief  Ends the frame for the derived system.
	*****************************************************************************************/
	void endFrame();

	/*!***************************************************************************************
	\brief  Starts the shutdown process for the derived system.
	*****************************************************************************************/
	void shutdown();

	/*!***************************************************************************************
	\brief  Notifies the system on an incoming message
	*****************************************************************************************/
	void handleMessage(message & p_message);

	systemState getState();

	void handleLoadLevelEvent(levelLoadEvent & p_event);
	void handleUnloadLevelEvent(levelUnloadEvent & p_event);
	void handlePreShutdownEvent(preShutdownEvent & p_event);
	
protected:
	/*!***************************************************************************************
	\brief  Empty handling of the initialize message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onInitialize() {}

	/*!***************************************************************************************
	\brief  Empty handling of the level loading message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onLevelLoad(const levelLoadEvent & /*p_event*/) {};

	/*!***************************************************************************************
	\brief  Empty handling of the start frame message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onStartFrame() {};

	/*!***************************************************************************************
	\brief  Empty handling of the update message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onUpdate() {};

	/*!***************************************************************************************
	\brief  Empty handling of the end frame message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onEndFrame() {};

	/*!***************************************************************************************
	\brief  Empty handling of the level unloading message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onLevelUnload(const levelUnloadEvent & /*p_event*/) {};

	/*!***************************************************************************************
	\brief  Empty handling of the pre-shutdown message for systems that don't need to respond to it.
	*****************************************************************************************/
	virtual void onPreShutdown() {};

	/*!***************************************************************************************
	\brief  Empty handling of the shutdown message for systems that don't need to respond to
	        it.
	*****************************************************************************************/
	virtual void onShutdown();

	/*!***************************************************************************************
	\brief  Empty handling of the incoming message notification for systems that don't
	        need to respond to it.
	*****************************************************************************************/
	virtual void onHandleMessage(message & /*p_message*/) {}

	/*!***************************************************************************************
	\brief  Gets the engine.
	\return Gets systemManager that owns this.
	*****************************************************************************************/
	systemManagerInterface* getSystemManager() const;

	float getDt() const { return m_dt; }
        //virtual GameObjectPtr getGameObjectByID(objID) const;


private:
	systemManagerInterface* m_system_manager = nullptr; //!< System manager that owns this

	systemState m_state = systemState::enm_Uninitialized; //!< The current state of this system

	float m_dt; //!< dt value of last update call
};
