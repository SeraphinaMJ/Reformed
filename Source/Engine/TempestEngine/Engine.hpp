/*!***************************************************************************************
\file       Engine.hpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the engine public interface.  This class is the only entry for the
        external runners to keep the engine running.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "ScriptMaps.hpp"

//========3rd Party Includes============================================================//
#include <memory>
#include <string>
#include "External/SceneManipulatorInterface.hpp"
#include "External/bundleManipulatorInterface.hpp"
#include "DLLControl.hpp"
#include "PersistenceManipulator.hpp"
#include "SystemManagement/SystemManagerInterface.hpp"
#include "External/EngineMetadataManipulatorInterface.hpp"
#include "External/CameraManipulatorInterface.hpp"
#include "External/AssetManipulatorInterface.hpp"
#include "External/NodeManipulatorInterface.hpp"
#include "External/GraphicsManipulatorInterface.hpp"
#include "External/BehaviorTreeManipulatorInterface.hpp"
#include <Configuration.hpp>


// forward declarations
namespace GameObjectFiltering {
	class gameObjectFilter;
}
class scene;
class systemManager;
class inputSourceInterface;
class externalWindowInterface;
class renderContextInterface;
class inputSourceInterface;
class icomponentRepository;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: engine
\brief   This is the primary execution organizer for the engine.
*****************************************************************************************/
class engine final
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Constructs engine.
	*****************************************************************************************/
	engine(externalWindowInterface * p_window_ptr, renderContextInterface * p_render_context_ptr, inputSourceInterface * p_input_source_ptr, icomponentRepository * p_component_repository);

	/*!***************************************************************************************
	\brief  Destroys the engine.
	*****************************************************************************************/
	virtual ~engine();

	/*!***************************************************************************************
	\brief  Initializes the engines and all of its systems.  Engine will be in a
	        ready-to-execute state
	*****************************************************************************************/
	void initialize(configuration p_config);

	/*!***************************************************************************************
	\brief  Performs one update step on all objects and systems using the supplied amount of
	        time passed
	*****************************************************************************************/
	void update();

	/*!***************************************************************************************
	\brief  Performs one render step on all objects and systems
	*****************************************************************************************/
	void render();

	/*!***************************************************************************************
	\brief  Shuts down the engine and all of its systems.  Engine will not function afterwards
	        and would need to be re-initialized.
	*****************************************************************************************/
	void shutdown();

	/*!***************************************************************************************
	\brief  Checks if the engine wants to shutdown, often initiated by objects within the
	        engine.
	\return bool - Returns true if the engine requests a shutdown, false otherwise.
	*****************************************************************************************/
	bool isQuitRequested() const;

    bool isPauseRequested() const;

    bool isEnginePaused() const;

    bool isResumeRequested() const;

	/*!***************************************************************************************
	\brief  Retrieves the scene manipulator instance from the engine.
	\return sceneManipulatorInterface - pointer to scene manipulator instance
	*****************************************************************************************/
	std::weak_ptr<sceneManipulatorInterface> getSceneManipulator();

	/*!***************************************************************************************
	\brief  Retrieves the bundle manipulator instance from the engine.
	\return bundleManipulatorInterface - pointer to the bundle manipulator instance.
	*****************************************************************************************/
	std::weak_ptr<bundleManipulatorInterface> getBundleManipulator();

	/*!***************************************************************************************
	\brief  Retrieves the persistence manipulator instance from the engine.
	\return persistenceManipulatorInterface - pointer to the persistence manipulator instance.
	*****************************************************************************************/
	std::weak_ptr<persistenceManipulatorInterface> getPersistenceManipulator();

	/*!***************************************************************************************
	\brief  Retrieves the engine metadata manipulator instance from the engine.
	\return engineMetadataManipulatorInterface - pointer to the engine metadata manipulator instance.
	*****************************************************************************************/
	std::weak_ptr<engineMetadataManipulatorInterface> getEngineMetadataManipulator();

	/*!***************************************************************************************
	\brief  Retrieves the camera manipulator instance from the engine.
	\return cameraManipulatorInterface - pointer to the camera manipulator instance.
	*****************************************************************************************/
	std::weak_ptr<cameraManipulatorInterface> getCameraManipulator();

    /*!***************************************************************************************
    \brief  Retrieves the camera manipulator instance from the engine.
    \return cameraManipulatorInterface - pointer to the camera manipulator instance.
    *****************************************************************************************/
    std::weak_ptr<assetManipulatorInterface> getAssetManipulator();


  /*!***************************************************************************************
  \brief  Retrieves the node manipulator instance from the engine.
  \return nodeManipulatorInterface - pointer to the node manipulator instance.
  *****************************************************************************************/
  std::weak_ptr<nodeManipulatorInterface> getNodeManipulator();

	std::weak_ptr<graphicsManipulatorInterface> getGraphicsManipulator();

    std::weak_ptr<behaviorTreeManipulatorInterface> getBehaviorManipulator();

	/*!***************************************************************************************
	\brief  Requests that the engine shutdown.  Effect is not immediate and not guaranteed.
	*****************************************************************************************/
	void requestQuit();

    void requestPause();

    void requestResumeFromPause();

	/*!***************************************************************************************
	\brief  Retrieves the current scene the engine is working with
	\return std::weak_ptr<scene> - The reference to the current scene, may be invalid.
	*****************************************************************************************/
	std::weak_ptr<scene> getCurrentScene();

	/*!***************************************************************************************
	\brief  Retrieves the configuration settings currently in use
	\return configuration - the current configuration settings
	*****************************************************************************************/
	configuration const & getConfiguration() const { return m_config; }


	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	systemManagerInterface * m_system_manager; //!< Reference to the system manager.
	GameObjectFiltering::gameObjectFilter * m_go_filter = nullptr; /*!< Reference to current *
	                                                                * game object filter     *
	                                                                * handler. */
	bool m_is_quit_requested = false; /*!< Is there a request to shutdown the engine (from   *
	                                   * client code) */

    bool m_is_pause_requested = false;
    bool m_is_engine_paused = false;

    bool m_is_resume_requested = false;

	configuration m_config; //!< The current configuration settings
	std::shared_ptr<sceneManipulatorInterface> m_scene_manipulator_ptr; //!< pointer to the scene manipulator interface for external control
	std::shared_ptr<bundleManipulatorInterface> m_bundle_manipulator_ptr; //!< pointer to the bundle manipulator interface for external control
	std::shared_ptr<persistenceManipulator> m_persistence_manipulator_ptr; //!< pointer to the persistence manipulator interface for external control
	std::shared_ptr<engineMetadataManipulatorInterface> m_engine_metadata_manipulator_ptr; //!< pointer to the engine metadata manipulator interface for external control
	std::shared_ptr<cameraManipulatorInterface> m_camera_manipulator_ptr; //!< pointer to the camera manipulator interface for external control
    std::shared_ptr<assetManipulatorInterface> m_asset_manipulator_ptr;
    std::shared_ptr<nodeManipulatorInterface> m_node_manipulator_ptr; //!< pointer to the node manipulator interface for external control
	std::shared_ptr<graphicsManipulatorInterface> m_graphics_manipulator_ptr;
    std::shared_ptr<behaviorTreeManipulatorInterface> m_behavior_manipulator;

	externalWindowInterface * m_window_ptr = nullptr;
	renderContextInterface * m_render_context_ptr = nullptr;
	inputSourceInterface * m_input_source_ptr = nullptr;
	icomponentRepository * m_component_repository_ptr = nullptr;



	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

