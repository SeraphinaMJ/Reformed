/*!***************************************************************************************
\file       ButtonRenderer.cpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#include "ButtonRenderer.hpp"
#include "../EngineController.hpp"
#include "../HierarchySelectionKeeper.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../StyleKeeper.hpp"
#include <Reflection.hpp>
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "../EditorWindow.hpp"
#include "../EditorObjectManager.hpp"
#include "../EditorObject.hpp"

Editor::buttonRenderer::buttonRenderer(editorWindow* p_parent_window)
: componentRenderer(p_parent_window)
{
  auto & l_styles = getStyleKeeper();

  l_styles.setStyle("*", "neighborRight", "readonly", 1);
  l_styles.setStyle("*", "neighborLeft", "readonly", 1);
  l_styles.setStyle("*", "neighborUp", "readonly", 1);
  l_styles.setStyle("*", "neighborDown", "readonly", 1);

  l_styles.setStyle("*", "pressed", "readonly", 1);

  l_styles.setStyle("*", "parent", "readonly", 1);
}

void Editor::buttonRenderer::onRender(typeRT & p_type_data, objID p_editor_object_id)
{
  auto l_selection = getSelectionKeeper();
  
  if (ImGui::RadioButton("Set Right Neighbor", m_rightButton))
  {
    setButtons(m_rightButton);
    m_buttonText = 0;
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("Remove Right Neighbor", m_rightButtonRemove))
  {
    setButtons(m_rightButtonRemove);
    m_buttonText = 4;
  }

  if (ImGui::RadioButton("Set Left Neighbor", m_leftButton))
  {
    setButtons(m_leftButton);
    m_buttonText = 1;
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("Remove Left Neighbor", m_leftButtonRemove))
  {
    setButtons(m_leftButtonRemove);
    m_buttonText = 5;
  }

  if (ImGui::RadioButton("Set Up Neighbor", m_upButton))
  {
    setButtons(m_upButton);
    m_buttonText = 2;
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("Remove Up Neighbor", m_upButtonRemove))
  {
    setButtons(m_upButtonRemove);
    m_buttonText = 6;
  }

  if (ImGui::RadioButton("Set Down Neighbor", m_downButton))
  {
    setButtons(m_downButton);
    m_buttonText = 3;
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("Remove Down Neighbor", m_downButtonRemove))
  {
    setButtons(m_downButtonRemove);
    m_buttonText = 7;
  }

  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();

  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  EditorObject * l_editor_object = l_editor_object_manager.getEditorObject(p_editor_object_id);

  //Have to press enter while in the text field to run this code
  std::string buffer;
  if (ImGui::InputText("Name of Button", &buffer, ImGuiInputTextFlags_EnterReturnsTrue))
  {
    objID l_my_id = getSelectionKeeper().getSelectionId();
    if (m_buttonText >= 0 && m_buttonText <= 3)
    {
      //Find the button with the given name
      objID l_neighbor_id = l_scene_manipulator->getObjectID(std::string(buffer.c_str()));

      if (l_neighbor_id != 0)
      {
        l_scene_manipulator->setButtonNeighbor(l_my_id, l_neighbor_id, m_buttonText);

        //Need new typeRT
        //typeRT l_my_new_data = l_scene_manipulator->getTypeRT(l_my_id);
        l_editor_object->setNeedsUpdate(true);

        //typeRT l_neighbor_new_data = l_scene_manipulator->getTypeRT(l_neighbor_id);
        EditorObject * l_neighbor_object = l_editor_object_manager.getEditorObject(l_neighbor_id);

        if(l_neighbor_object != nullptr)
        {
          l_neighbor_object->setNeedsUpdate(true);
        }

      }
    }
    else if (m_buttonText >= 4 && m_buttonText <= 7)
    {
      objID l_neighbor_id = l_scene_manipulator->removeButtonNeighbor(l_my_id, m_buttonText);

      //Need new typeRT
      typeRT l_my_new_data = l_scene_manipulator->getTypeRT(l_my_id);
      l_editor_object->setData(l_my_new_data);

      if(l_neighbor_id != 0)
      {
        typeRT l_neighbor_new_data = l_scene_manipulator->getTypeRT(l_neighbor_id);
        EditorObject * l_neighbor_object = l_editor_object_manager.getEditorObject(l_neighbor_id);

        if (l_neighbor_object != nullptr)
        {
          l_neighbor_object->setData(l_neighbor_new_data);
        }
      }
    }

  }
}

void Editor::buttonRenderer::setButtons(bool & p_buttonToBeTrue)
{
  m_downButton = false;
  m_upButton = false;
  m_leftButton = false;
  m_rightButton = false;
  m_rightButtonRemove = false;
  m_leftButtonRemove = false;
  m_downButtonRemove = false;
  m_upButtonRemove = false;
  p_buttonToBeTrue = true;
}