/*!***************************************************************************************
\file       HierarchyWindow.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the hierarchy window class
*****************************************************************************************/

#include "HierarchyWindow.hpp"
#include <imgui.h>
#include <memory>
#include <SceneManagement/Scene.hpp>
#include <SceneManagement/Space.hpp>
#include <SceneManagement/GameObject.hpp>
#include "EngineController.hpp"
#include "EditorLogger.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "EditorWindow.hpp"
#include "EditorObject.hpp"

Editor::hierarchyWindow::hierarchyWindow(editorWindow * p_parent_editor)
	: windowBase( p_parent_editor), m_renderer(p_parent_editor)
{
  
}

void Editor::hierarchyWindow::createEditorObjects()
{
  std::shared_ptr<scene> l_scene = getEngineController().getEngineRunner()->getEngine()->getCurrentScene().lock();
  m_renderer.createAllEditorObjects({ l_scene });
}

void Editor::hierarchyWindow::onRender()
{
  Editor::EditorLogger & l_log = getLogger();
  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
	// display hierarchy menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
      if (ImGui::MenuItem("Empty Space"))
      {
        if(getSelectionKeeper().isSceneSelected())
        {
          auto l_return = l_scene_manipulator->addEmptySpace(getSelectionKeeper().getSelectionId());

          if(l_return != nullptr)
          {
            l_log.AddLog("[NOTICE] New Empty Space created in scene: %s \n", getSelectionKeeper().getSelectionName().c_str());

            getSelectionKeeper().setSelected(l_return);

            //Need to create new Editor Object for the new space
            typeRT l_data = l_scene_manipulator->getTypeRT(l_return->getID());
            EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_space, l_data, l_return->getID());

            if(l_new_object != nullptr)
            {
              l_editor_object_manager.addEditorObject(l_new_object);
            }
          }
          else
          {
            l_log.AddLog("[ERROR] New Empty Space creation failed!\n");
          }

        }
        else
        {
          auto l_return = l_scene_manipulator->addEmptySpace();

          if (l_return != nullptr)
          {
            l_log.AddLog("[NOTICE] New Empty Space created.\n");

            getSelectionKeeper().setSelected(l_return);

            //Need to create new Editor Object for the new space
            typeRT l_data = l_scene_manipulator->getTypeRT(l_return->getID());
            EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_space, l_data, l_return->getID());

            if (l_new_object != nullptr)
            {
              l_editor_object_manager.addEditorObject(l_new_object);
            }
          }
          else
          {
            l_log.AddLog("[ERROR] New Empty Space creation failed!\n");
          }
        }
      }
			if (ImGui::MenuItem("Empty Game Object"))
			{
        if(getSelectionKeeper().isNoneSelected() || getSelectionKeeper().isSceneSelected())
        {
          auto l_return = l_scene_manipulator->addEmptyGameObject();

          if (l_return != nullptr)
          {
            getSelectionKeeper().setSelected(l_return);

            l_log.AddLog("[NOTICE] New Empty Game Object created.\n");

            //Need to create new Editor Object for the new game object
            typeRT l_data = l_scene_manipulator->getTypeRT(l_return->getID());
            EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_space, l_data, l_return->getID());

            if (l_new_object != nullptr)
            {
              l_editor_object_manager.addEditorObject(l_new_object);
            }
          }
          else
          {
            l_log.AddLog("[ERROR] Game Object creation failed!\n");
          }
        }
        else
        {
          auto l_return = l_scene_manipulator->addEmptyGameObject(getSelectionKeeper().getSelectionId());

          if (l_return != nullptr)
          {
            l_log.AddLog("[NOTICE] New Empty Game Object created to parent: %s.\n", getSelectionKeeper().getSelectionName().c_str());

            getSelectionKeeper().setSelected(l_return);

            //Need to create new Editor Object for the new game object
            typeRT l_data = l_scene_manipulator->getTypeRT(l_return->getID());
            EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_game_object, l_data, l_return->getID());

            if (l_new_object != nullptr)
            {
              l_editor_object_manager.addEditorObject(l_new_object);
            }
          }
          else
          {
            l_log.AddLog("[ERROR] Game Object creation failed!\n");
          }
        }
			}

			ImGui::EndMenu();
		}
		// ...
		ImGui::EndMenuBar();
	}

  m_renderer.render();
}




