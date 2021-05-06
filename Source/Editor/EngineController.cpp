/*!***************************************************************************************
\file       EngineController.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the engine controller class
*****************************************************************************************/

#include "EngineController.hpp"
#include "EditorLogger.hpp"
#include <External/CameraManipulatorInterface.hpp>
#include <EngineRunner.hpp>
#include <Engine.hpp>

Editor::engineController::engineController(engineRunner* p_engine, EditorLogger & p_logger)
	: m_engine{ nullptr }, m_is_playing{ false }, m_logger(p_logger)
{
	setEngine(p_engine);
}

void Editor::engineController::setEngine(engineRunner* p_engine)
{
	m_engine = p_engine;
	stop();  // disable appropriate systems so they don't interfere with editor functionality.
	injectEditorCamera();
}

engineRunner * Editor::engineController::getEngineRunner() const
{
	return m_engine;
}

void Editor::engineController::releaseEngine()
{
	removeEditorCamera();
	start(true);
	m_engine = nullptr;
}

void Editor::engineController::processEvents()
{
	if (m_engine == nullptr)
		return;

	m_engine->update();
	m_engine->render();
}

void Editor::engineController::start(bool p_override_temporary_store)
{
	if (m_engine == nullptr)
		return;

	m_logger.AddLog("[ENGINE] Resuming the engine.\n");

	removeEditorCamera();
	if(!p_override_temporary_store)
		storeTemporaryScene();
	m_is_playing = true;
	// turn on all bundles
	try
	{
		auto l_bundle_manip = m_engine->getEngine()->getBundleManipulator().lock();
		auto l_bundle_names = l_bundle_manip->getAllBundles();
    auto l_scene_manip = m_engine->getEngine()->getSceneManipulator().lock();
		for (auto l_bundle : l_bundle_names)
		{
			l_bundle_manip->enableBundle(l_bundle);
		}

    l_scene_manip->setPhysicsEngineRunning(true);
    //l_scene_manip->setPhysicsSystemTimeStep(1.0f / 60.0f);
    l_scene_manip->setPhysicsGravity(vector3(0.0f, -400.0f, 0.0f));
	}
	catch (std::exception eStart)
	{
		m_logger.AddLog("Start Bunddle Manipulator exception throwed.\n");
		m_logger.AddLog("Editor::engineController::start in line53.\n");
		m_logger.AddLog("%s \n", eStart.what());
	}

}

void Editor::engineController::stop()
{
	if (m_engine == nullptr)
		return;

	m_logger.AddLog("[ENGINE] Pausing the engine.\n");
	// turn off all bundles except graphics
	try
	{
		auto l_bundle_manip = m_engine->getEngine()->getBundleManipulator().lock();
		auto l_bundle_names = l_bundle_manip->getAllBundles();
    auto l_scene_manip = m_engine->getEngine()->getSceneManipulator().lock();
		for (auto l_bundle : l_bundle_names)
		{
      if (l_bundle == "physics Bundle")
      {
        //Timestep to 0 for physics, Maybe gravity to 0?
        
        l_scene_manip->setPhysicsGravity(vector3(0.0f, 0.0f, 0.0f));
        //l_scene_manip->setPhysicsSystemTimeStep(0.0f);
      }
			else if (l_bundle == "particleSystem::getName()")
			{
        l_bundle_manip->disableBundle(l_bundle);
			}
			else if (l_bundle != "graphics & display")
			{
				l_bundle_manip->suspendBundle(l_bundle);
			}
		}


		m_is_playing = false;
		restoreTemporaryScene();
		injectEditorCamera();
	}
	catch (std::exception eStop)
	{
		m_logger.AddLog("Stop Bundle Manipulator exception throwed.\n");
		m_logger.AddLog("Editor::engineController::stop in line98.\n");
		m_logger.AddLog("%s \n", eStop.what());
	}

}

bool Editor::engineController::isPlaying()
{
  return m_is_playing;
}

void Editor::engineController::injectEditorCamera()
{
	if (m_engine == nullptr)
		return;

	try
	{
		m_engine->getEngine()->getCameraManipulator().lock()->enableEditorCamera();
	}
	catch (std::exception eInject)
	{
		m_logger.AddLog("InjectEditorCamera Exception throwed.\n");
		m_logger.AddLog("Editor::engineController::injectEditorCamera in line 114.\n");
		m_logger.AddLog("%s\n", eInject.what());
	}
}

void Editor::engineController::removeEditorCamera()
{
	if (m_engine == nullptr)
		return;
	try
	{
		m_engine->getEngine()->getCameraManipulator().lock()->disableEditorCamera();
	}
	catch (std::exception eRemoveCamera)
	{
		m_logger.AddLog("RemoveEditorCamera Exception throwed.\n");
		m_logger.AddLog("Editor::engineController::removeEditorCamera in line 128.\n");
		m_logger.AddLog("%s\n", eRemoveCamera.what());
	}
}

void Editor::engineController::storeTemporaryScene()
{
	if (m_engine == nullptr)
		return;

	 m_logger.AddLog("[EDITOR] Storing temporary scene.\n");
	// change to our temp persistence location
	// write the contents of the engine
	 try
	 {
		m_engine->getEngine()->getPersistenceManipulator().lock()->quickStoreTo();
	 }
	 catch (std::exception eStore)
	 {
		 m_logger.AddLog("StoreTemporaryScene Exception throwed.\n");
		 m_logger.AddLog("Editor::engineController::storeTemporaryScene in line 144.\n");
		 m_logger.AddLog("%s\n", eStore.what());
	 }

	// change back to the previous persistence location
}

void Editor::engineController::restoreTemporaryScene()
{
	if (m_engine == nullptr)
		return;

	m_logger.AddLog("[EDITOR] Restoring temporary scene.\n");
	// change to our temp persistence location
	// read the contents of the engine
	try
	{
		m_engine->getEngine()->getPersistenceManipulator().lock()->quickRestoreFrom();
	}
	catch (std::exception eRestore)
	{
		m_logger.AddLog("RestoreTemporaryScene Exception throwed.\n");
		m_logger.AddLog("Editor::engineController::restoreTemporaryScene in line 172.\n");
		m_logger.AddLog("%s\n", eRestore.what());
	}
	// change back to the previous persistence location
}


