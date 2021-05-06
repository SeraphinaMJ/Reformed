/*!***************************************************************************************
\file       HierarchySelectionKeeper.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for a selection manager for hierarchy entries
*****************************************************************************************/
#include "HierarchySelectionKeeper.hpp"
#include <memory>
#include <SceneManagement/Scene.hpp>
#include <SceneManagement/Space.hpp>
#include <SceneManagement/GameObject.hpp>
#include "EditorLogger.hpp"
#include "EngineController.hpp"
#include "EngineRunner.hpp"
#include "Engine.hpp"

Editor::hierarchySelectionKeeper::hierarchySelectionKeeper(EditorLogger & p_logger)
	: m_selection_type{SelectionType::None}, m_selection_id{0}, m_logger(p_logger)
{ }

void Editor::hierarchySelectionKeeper::clearSelection()
{
	m_selection_id = 0;
	m_selection_type = SelectionType::None;
  m_selection_name = std::string();

  m_logger.AddLog("[NOTICE] Selection cleared.\n");
}

bool Editor::hierarchySelectionKeeper::isNoneSelected() const
{
	return m_selection_type == SelectionType::None;
}

bool Editor::hierarchySelectionKeeper::isSceneSelected() const
{
	return m_selection_type == SelectionType::Scene;
}

bool Editor::hierarchySelectionKeeper::isSpaceSelected() const
{
	return m_selection_type == SelectionType::Space;
}

bool Editor::hierarchySelectionKeeper::isGameObjectSelected() const
{
	return m_selection_type == SelectionType::GameObject;
}

bool Editor::hierarchySelectionKeeper::isSelected(std::shared_ptr<scene> p_scene) const
{
	return m_selection_type == SelectionType::Scene
		&& m_selection_id == p_scene->getID();
}

bool Editor::hierarchySelectionKeeper::isSelected(std::shared_ptr<space> p_space) const
{
	return m_selection_type == SelectionType::Space
		&& m_selection_id == p_space->getID();
}

bool Editor::hierarchySelectionKeeper::isSelected(std::shared_ptr<gameObject> p_object) const
{
	return m_selection_type == SelectionType::GameObject
		&& m_selection_id == p_object->getID();
}

void Editor::hierarchySelectionKeeper::setSelected(std::shared_ptr<scene> p_scene)
{
  setSelectedNextFrame(SelectionType::Scene, p_scene->getID(), p_scene->getName());

	//m_selection_type = SelectionType::Scene;
	//m_selection_id = p_scene->getID();
  //m_selection_name = p_scene->getName();

  
}

void Editor::hierarchySelectionKeeper::setSelected(std::shared_ptr<space> p_space)
{
  setSelectedNextFrame(SelectionType::Space, p_space->getID(), p_space->getName());

	//m_selection_type = SelectionType::Space;
	//m_selection_id = p_space->getID();
  //m_selection_name = p_space->getName();

  
}

void Editor::hierarchySelectionKeeper::setSelected(std::shared_ptr<gameObject> p_object)
{
  setSelectedNextFrame(SelectionType::GameObject, p_object->getID(), p_object->getName());

	//m_selection_type = SelectionType::GameObject;
	//m_selection_id = p_object->getID();
  //m_selection_name = p_object->getName();

  
}

void Editor::hierarchySelectionKeeper::setSelected(unsigned p_object_id, engineController & p_engine_controller)
{
  std::shared_ptr<scene> l_scene = p_engine_controller.getEngineRunner()->getEngine()->getCurrentScene().lock();

  std::shared_ptr<gameObject> l_game_object = l_scene->findGameObjectByID(p_object_id);

  if(l_game_object != nullptr)
  {
    setSelected(l_game_object);
  }
}

void Editor::hierarchySelectionKeeper::setSelectedNextFrame(SelectionType p_selection_type, unsigned p_selection_id,
  const std::string& p_selection_name)
{
  m_selection_type_next_frame = p_selection_type;
  m_selection_id_next_frame = p_selection_id;
  m_selection_name_next_frame = p_selection_name;
  m_next_frame_needs_update = true;
}

void Editor::hierarchySelectionKeeper::updateSelected()
{
  //if(//gizmo being used)
  //{
  //  m_next_frame_needs_update = false;
  //  return;
  //}

  if(m_next_frame_needs_update)
  {
    m_selection_type = m_selection_type_next_frame;
    m_selection_id = m_selection_id_next_frame;
    m_selection_name = m_selection_name_next_frame;
    m_next_frame_needs_update = false;

    if(m_selection_type == SelectionType::Scene)
    {
      m_logger.AddLog("[NOTICE] %s scene selected.\n", m_selection_name.c_str());
    }
    else if(m_selection_type == SelectionType::Space)
    {
      m_logger.AddLog("[NOTICE] %s space selected.\n", m_selection_name.c_str());
    }
    else if(m_selection_type == SelectionType::GameObject)
    {
      m_logger.AddLog("[NOTICE] %s game object selected.\n", m_selection_name.c_str());
    }

  }
}

