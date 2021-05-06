/*!***************************************************************************************
\file       InspectorWindow.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the inspector window class
*****************************************************************************************/
#include "InspectorWindow.hpp"
#include "HierarchyWindow.hpp"
#include "EditorWindow.hpp"
#include "Reflection.hpp"
#include "../Editor/EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>

#include "EditorObjectManager.hpp"
#include "EditorObject.hpp"

#include "ObjectCommand.hpp"
#include "UndoRedo.hpp"

Editor::inspectorWindow::inspectorWindow(editorWindow * p_parent_editor)
	: windowBase(p_parent_editor),
	m_renderer(p_parent_editor), m_engine_data_render(true), m_engine_data_timestep(10), m_timer(0.0f)
{
	m_renderer.setRemoveHandler([&](std::string p_component_name)
	{
		//get game object id
		objID l_selection_id = getSelectionKeeper().getSelectionId();
		getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->removeGameObjectComponent(l_selection_id, p_component_name);
	});
}

void Editor::inspectorWindow::onRender()
{
	if (getSelectionKeeper().isNoneSelected())
		return;

	objID l_selection_id = getSelectionKeeper().getSelectionId();

  //Ask engine for typeRT data of current id
	//typeRT l_type_data = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(l_selection_id);

  //Ask EditorObjectManager for typeRT data of current id
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  EditorObject * l_editor_object = l_editor_object_manager.getEditorObject(l_selection_id);

  bool l_playing = getEngineController().isPlaying();
  auto l_selection_keeper = getSelectionKeeper();

  if(l_editor_object != nullptr)
  {
    typeRT & l_type_data = l_editor_object->getData();

    if (l_playing)
    {
      if(m_engine_data_render)
      {
        float l_timestep = 1.0f / static_cast<float>(m_engine_data_timestep);
        float l_last_frame = ImGui::GetIO().DeltaTime;
        m_timer += l_last_frame;
        //Only update the typeRT after the timestep has elapsed 
        if (m_timer >= l_timestep)
        {
          m_timer = 0.0f;
          l_type_data = l_editor_object_manager.getTypeRTFromEngine(l_selection_id);
        }
      }
      
    }

    // display the typeRT structure and record any changes into the l_newData parameter.

    if (l_selection_keeper.isSceneSelected())
    {
      m_renderer.renderScene(l_type_data, l_selection_id);
    }
    else if (l_selection_keeper.isSpaceSelected())
    {
      m_renderer.renderSpace(l_type_data, l_selection_id);
    }
    else if (l_selection_keeper.isGameObjectSelected())
    {
      m_renderer.renderGameObject(l_type_data, l_selection_id);
    }
  }

  //Send dirty typeRT data for any object that changed back to engine
  l_editor_object_manager.parseCommandsInQueue();
  
  l_editor_object_manager.sendAllDirtyObjectDataToEngine();

  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();

  if (l_selection_keeper.isGameObjectSelected())
  {
    // display hierarchy menu
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("Create"))
      {
        auto l_component_types = getEngineController().getEngineRunner()->getEngine()->getEngineMetadataManipulator().lock()->getComponentTypes();

        // loop over all component types from the engine
        for (auto l_comp_type : l_component_types)
        {
          // create a menu item if the editor should be able to add a type of the component
          if (l_comp_type.allowEditorAdd)
          {
            if (ImGui::MenuItem(l_comp_type.friendlyName.c_str()))
            {
              //record object state before change
              std::shared_ptr<Command> l_new_command = std::make_shared<ObjectAddComponentCommand>();
              l_new_command->Init(&getEngineController(), l_editor_object);

              // menu item clicked, request engine to add the component
              l_scene_manipulator->addGameObjectComponent(l_selection_id,
                l_comp_type.name);

              //Ask for typeRT from engine to override ours
              typeRT l_new_data = l_scene_manipulator->getTypeRT(l_selection_id);
              l_editor_object->setData(l_new_data);

              //record object state after change for undo redo purposes
              l_new_command->Record();
              UndoRedoManager::GetInstance().RecordState(l_new_command, &getEngineController());

              if(l_comp_type.friendlyName == "Rigid Body")
              {
                l_scene_manipulator->addPhysicsObjectFromEditor(l_editor_object->getObjectID());
              }
            }
          }
        }
        ImGui::EndMenu();
      }
      ImGui::PushItemWidth(50.0f);
      ImGui::Checkbox("Display Engine Data", &m_engine_data_render);
      ImGui::DragInt("Updates per second", &m_engine_data_timestep, 1, 1, 60);
      ImGui::PopItemWidth();
      ImGui::EndMenuBar();
    }
  }
}

