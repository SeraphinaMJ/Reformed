/*!***************************************************************************************
\file       SceneSystem.hpp
\author     Aaron Damyen
\date       5/31/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the scene system class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SystemBase.hpp"

//========3rd Party Includes============================================================//
#include <memory>
#include <vector>

#include "EventBase.hpp"
#include "DataTypes.hpp"

// forward declarations
class scene;
class gameObjectCollector;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

class levelLoadEvent : public EventSystem3::EventBase
{
public:
	levelLoadEvent(std::shared_ptr<scene> p_scene) : m_scene{p_scene} {}
	std::shared_ptr<scene> getScene() const {	return m_scene;	}
private:
	std::shared_ptr<scene> m_scene;
};

class levelUnloadEvent : public EventSystem3::EventBase
{
public:
	levelUnloadEvent(std::shared_ptr<scene> p_scene) : m_scene{p_scene} {}
	std::shared_ptr<scene> getScene() const { return m_scene; }
private:
	std::shared_ptr<scene> m_scene;
};

/*!***************************************************************************************
\par class: sceneSystem
\brief   This is the definition of the sceneSystem class.
*****************************************************************************************/
class sceneSystem final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
	public:

		/*!***************************************************************************************
	\brief  Generates a new, empty scene and adds it ot the loaded scenes list.
	\return std::shared_ptr<scene> - Reference to the new scene.
	*****************************************************************************************/
		std::shared_ptr<scene> createEmptyScene();

		std::shared_ptr<scene> getScene(std::string p_name);

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

	/*!***************************************************************************************
	\brief  Constructs the system with a specified game object collector.
	\param p_collector - Reference to the collector to report game object changes to.
	*****************************************************************************************/
	sceneSystem(gameObjectCollector * p_collector);

	/*!***************************************************************************************
	\brief  Destroys the system, releasing resources
	*****************************************************************************************/
	virtual ~sceneSystem() = default;

	/*!***************************************************************************************
	\brief  Retrieves the current scene being displayed
	\return std::weak_ptr<scene> - Reference to the current scene, may not be valid
	*****************************************************************************************/
	std::weak_ptr<scene> getCurrentScene() const;

	/*!***************************************************************************************
	\brief  Loads a scene from disk into memory.  Does not set the scene as current.
	\param p_location - the directory structure to load from
	\param p_name - the name of the scene to load
	\return std::shared_ptr<scene> - The scene that was loaded
	*****************************************************************************************/
	std::shared_ptr<scene> loadScene(std::string p_location, std::string p_name);

	/*!***************************************************************************************
	\brief  Loads a scene from disk into memory.  Does not set the scene as current.
	\param p_name - the name of the scene to load
	\return std::shared_ptr<scene> - The scene that was loaded
	*****************************************************************************************/
	std::shared_ptr<scene> loadScene(std::string p_name);

	/*!***************************************************************************************
	\brief  Retrieves the current scene being displayed
	\return std::weak_ptr<scene> - Reference to the current scene, may not be valid
	*****************************************************************************************/
	void setCurrentScene(std::shared_ptr<scene> p_new_scene);
	void setCurrentScene(std::string p_name);

	/*!***************************************************************************************
	\brief  Retrieves the current game object collector
	\return gameObjectCollector * - the currently assigned game object collector
	*****************************************************************************************/
	gameObjectCollector * getGameObjectCollector() const;

    /*!***************************************************************************************
    \brief  Retrieves an object by id, useful helper for anyone who inherits from system base
    \return GameObjectPtr * - object corresponding to id
    *****************************************************************************************/
    std::weak_ptr<gameObject> getGameObjectByID(objID) const;

	/*!***************************************************************************************
	\brief  Assigns a different game object collector, for tracking changes to game objects
	\param p_go_collector - the reference to the new game object collector to use.
	*****************************************************************************************/
	void setGameObjectCollector(gameObjectCollector * p_go_collector);

	void clearAllScenes() { m_loaded_scenes.clear(); resetmCurrentScene(); }

  void unloadScene(std::shared_ptr<scene> p_scene);

    void requestResetCurrentScene();

    void resetCurrentScene();

    bool getCurrentSceneNeedsReset();

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("sceneSystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

	void reset();

	/*!***************************************************************************************
	\brief  Retrieves the number of spaces in this scene.  For monitoring.
	\return size_t - the number of spaces in the current scene.
	*****************************************************************************************/
	size_t getSpaceCount() const;
	/*!***************************************************************************************
	\brief  Retrieves the number of game objects in this scene.  For monitoring.
	\return size_t - the number of game objects in the current scene.
	*****************************************************************************************/
	size_t getGameObjectCount() const;

	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Handles the onInitialize event from the SystemManager, to initialize this system.
	*****************************************************************************************/
	void onInitialize() override;
	/*!***************************************************************************************
	\brief  Handles the onPreShutdown event from the SystemManager, to prepare for shutdown this system.
	*****************************************************************************************/
	void onPreShutdown() override;
	/*!***************************************************************************************
	\brief  Handles the onShutdown event from the SystemManager, to shutdown this system.
	*****************************************************************************************/
	void onShutdown() override;

	void onUpdate() override;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	void setmCurrentScene(std::shared_ptr<scene> p_new_scene);
	void resetmCurrentScene();

    // during gameplay some objects may be killed so their ids are flagged here
    //std::vector<objID> objectsToDelete;

	std::vector<std::shared_ptr<scene>> m_loaded_scenes; //!< List of currently loaded scenes.
	// DO NOT MODIFY m_current_scene DIRECTLY!  Events need to be sent when this changes.
	std::shared_ptr<scene> m_current_scene;
	gameObjectCollector * m_go_collector; //!< Reference to the game object collector being used.

    bool m_needs_reset;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	friend class sceneManipulator;
};


