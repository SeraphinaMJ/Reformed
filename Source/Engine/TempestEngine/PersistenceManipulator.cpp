/*!***************************************************************************************
\file       PersistenceManipulator.cpp
\author     David Roberts
\date       4/13/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Pure virtual base class for persistence requests. Used by PersistenceSystem.
*****************************************************************************************/
#include "PersistenceManipulator.hpp"
#include "SystemManagement/SystemManagerInterface.hpp"
#include "PersistenceSystem.hpp"
#include "SceneManagement/SceneSystem.hpp"
#include "SceneManagement/Scene.hpp"
#include "PersistenceRequest.hpp"

// headers for new_scene
#include "Components/Camera.hpp"
#include "Components/InputComponent.hpp"

persistenceManipulator::persistenceManipulator(systemManagerInterface * p_system_manager)
	: m_system_manager{p_system_manager}, m_is_storing(false)
{
}

void persistenceManipulator::setPersistenceLocation(const std::string& p_data_location)
{
	m_persistence_location = p_data_location;
}

void persistenceManipulator::saveCurrentScene()
{
	saveCurrentSceneAs(m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock()->getName());
}

void persistenceManipulator::saveCurrentSceneAs(std::string p_name)
{
	// TODO (David/Ryan): Should this functionality be moved into a System (somewhere) so it can be accessed by scripts and the engine itself (without going through the manipulator)?  Aaron
	auto scene_to_save = m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock();
	auto l_request = persistenceRequest<scene>(Direction::Write, "scenes/", p_name + ".cee", scene_to_save);
	logger("persistenceManipulator").info() << "Write Location from Manipulator: " << m_persistence_location << std::endl;
	l_request.execute(m_persistence_location);
}

std::shared_ptr<scene> persistenceManipulator::loadScene(std::string p_name)
{
	return m_system_manager->getSystem<sceneSystem>()->loadScene(m_persistence_location, p_name);
}

void persistenceManipulator::load(std::string p_name)
{
	// TODO (David/Ryan): Should this functionality be moved into a System (somewhere) so it can be accessed by scripts and the engine itself (without going through the manipulator)?  Aaron
	//m_system_manager->getSystem<sceneSystem>()->clearAllScenes();
  auto l_last_scene = m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock();
  std::string l_last_scene_name = l_last_scene->getName();


	auto l_new_scene = loadScene(p_name);
	m_system_manager->getSystem<sceneSystem>()->setCurrentScene(l_new_scene);

	// clear out the last scene
  m_system_manager->getSystem<sceneSystem>()->unloadScene(l_last_scene);
}

void persistenceManipulator::new_scene()
{
	m_system_manager->getSystem<sceneSystem>()->reset();
}

void persistenceManipulator::delete_scene(const std::string & name)
{
    // if the file system exists
    if (std::filesystem::exists(m_persistence_location) == true)
    {
        // find particular scene and delete it from the file system.
        auto scene = m_system_manager->getSystem<sceneSystem>()->getScene(name);
        if (scene != nullptr)
        {
            // if scene exists then we use the std filesystem to delete it.

        }
    }
}

void persistenceManipulator::quickStoreTo()
{
	if(!m_is_storing)
	{
		std::string l_old_loc = m_persistence_location;
    m_persistence_location = l_old_loc + "/temp/";
		saveCurrentSceneAs("1");
		m_persistence_location = l_old_loc;
		m_is_storing = true;
	}
	else
	{
		logger("persistenceManipulator").warn() << "Skipping a second consecutive call to the quickStoreTo function.";
	}
}

void persistenceManipulator::quickRestoreFrom()
{
	if(m_is_storing)
	{
    std::string l_old_loc = m_persistence_location;
    m_persistence_location = l_old_loc + "/temp/";
		// if location doesn't exist, skip loading
		if(std::filesystem::exists(m_persistence_location))
			load("1");
		m_persistence_location = l_old_loc;
		m_is_storing = false;
	}
	else
	{
		logger("persistenceManipulator").warn() << "Skipping a second consecutive call to the quickRestoreFrom function.";
	}
}
