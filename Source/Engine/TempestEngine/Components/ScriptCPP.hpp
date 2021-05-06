/*!***************************************************************************************
\file       ScriptCPP.hpp
\author     Aaron Damyen
\date       5/17/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of a script component that is specifically a C++ script.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SceneManagement/GameObject.hpp"
#include "../Scripting/ScriptingSystem.hpp"

//========3rd Party Includes============================================================//
#include <ComponentTypeDetails.hpp>
#include <IComponentRepository.hpp>

// forward declaration
class scriptingSystem;
class systemManagerInterface;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: scriptCPP
\brief   A script component representing C++ code
*****************************************************************************************/
class scriptCPP : public componentCRTP<scriptCPP>, public EventSystem3::EventHandler
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	virtual void updateFromTypeRT(typeRT& p_type) override;

	virtual typeRT toTypeRT() const override;
	/*!***************************************************************************************
	\brief  Allocates the script and sets its initial state.
	*****************************************************************************************/
	scriptCPP();
	scriptCPP(typeRT& p_data);
	/*!***************************************************************************************
	\brief  Destroys the script, freeing any allocated resources.
	*****************************************************************************************/
	virtual ~scriptCPP() = default;

	static componentType const getType() { static componentType const s_type = componentType::forAbstract<scriptCPP>(); return s_type; }

	/*!***************************************************************************************
	\brief  Notifies the script to start processing.  Guaranteed to be called before any updates are sent.
	\param p_parent_system - Reference to the parent system for accessing resources.
	\param p_system_manager - Reference to the system manager for accessing other systems.
	*****************************************************************************************/
	void start(scriptingSystem * p_parent_system, systemManagerInterface * p_system_manager);
	/*!***************************************************************************************
	\brief  Notifies the script to perform one update.
	\param p_parent_system - Reference to the parent system for accessing resources.
	\param p_system_manager - Reference to the system manager for accessing other systems.
	\param p_dt - The amount of time that has passed since the last call to update.
	*****************************************************************************************/
	void update(scriptingSystem * p_parent_system, systemManagerInterface * p_system_manager, float p_dt);
	/*!***************************************************************************************
	\brief  Notifies the script to stop processing.
	\param p_parent_system - Reference to the parent system for accessing resources.
	\param p_system_manager - Reference to the system manager for accessing other systems.
	*****************************************************************************************/
	void shutdown(scriptingSystem * p_parent_system, systemManagerInterface * p_system_manager);


	/*!***************************************************************************************
	\brief  Identifies if the script has not yet started.
	\return bool - Returns true if the script has not yet started, false otherwise.
	*****************************************************************************************/
	bool isUnstarted() const;
	/*!***************************************************************************************
	\brief  Identifies if the script is in a running state.
	\return bool - Returns true if the script is running, false otherwise.
	*****************************************************************************************/
	bool isRunning() const;

	//virtual std::string const sub_type() const = 0;


	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Notifies the script implementation of the start event.  Default implementation
	        is empty, making it optional.
	*****************************************************************************************/
	virtual void onStart() {} // default no-op behavior
	/*!***************************************************************************************
	\brief  Notifies the script implementation of the update event.  Required implementation.
	*****************************************************************************************/
	virtual void onUpdate() {} // default no-op behavior
	/*!***************************************************************************************
	\brief  Notifies the script implementation of the shutdown event.  Default implementation
				  is empty, making it optional.
	*****************************************************************************************/
	virtual void onShutdown() {} // default no-op behavior

	/*!***************************************************************************************
	\brief  Retrieves the scripting system this script belongs to.
	\return scriptingSystem * - The parent scripting system.
	*****************************************************************************************/
	scriptingSystem * getSystem() const;
	/*!***************************************************************************************
	\brief  Retrieves the system manager the parent system belongs to.
	\return systemManager * - The parent system's manager.
	*****************************************************************************************/
	systemManagerInterface * getSystemManager() const;
	/*!***************************************************************************************
	\brief Retrieves the change in time of the last update event.
	\return float - The amount of time change, in seconds.
	*****************************************************************************************/
	float dt() const;

	// TODO(coseo) move this away from using game object?
	template <typename T>
	componentHandle<T> getComponent() {return getGameObject().lock()->getComponent<T>();}
/*
	template <typename T>
	T* getScript()
	{
		for (auto i_script : getGameObject().lock()->getComponents<scriptCPP>())
		{
			if (i_script->sub_type() == T::getSubType())
			{
				return reinterpret_cast<T*>(i_script);
			}
		}
		return nullptr;
	}*/

	template <typename T>
	T* getSystem() {return  getSystemManager()->getSystem<T>();}

	template <typename componentType>
	std::vector<std::shared_ptr<gameObject>> findObjectsByComponent() { return  m_system->findObjectsByComponent<componentType>(); }

	std::shared_ptr<gameObject> findObject(std::string p_object_name);
	std::shared_ptr<gameObject> findObject(objID p_objId);

	space* getMySpace();
	std::shared_ptr<gameObject> createGameObjectOnMySpace();
	std::shared_ptr<gameObject> createGameObjectOnMySpace(std::string p_name);

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	/*!***************************************************************************************
	\par enum: scriptCPPState
	\brief  State of the script for ensuring proper ordering of events.
	*****************************************************************************************/
	enum scriptCPPState { UNSTARTED, RUNNING };

	scriptCPPState m_state; //!< The current state of this script
	scriptingSystem * m_system = nullptr; //!< Reference to the scripting system this script belongs to.
	systemManagerInterface * m_system_manager = nullptr; //!< Reference to the system manager
	float m_dt; //!< The current dt value for this script.
};

