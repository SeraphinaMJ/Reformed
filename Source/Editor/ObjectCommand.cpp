/*!***************************************************************************************
\file       ObjectCommand.cpp
\author     Charles Boudousquie, Ryan Booth
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation for the object command classes.
*****************************************************************************************/

#include "ObjectCommand.hpp"
#include "EngineController.hpp"
#include "UndoRedo.hpp"
#include "GameObject.hpp"
#include "HierarchySelectionKeeper.hpp"

void Editor::ObjectDeleteCommand::SetSpace(objID id)
{
  spaceID = id;
}

void Editor::ObjectDeleteCommand::SetObjectSelector(hierarchySelectionKeeper * objectSelector)
{
  this->objSelector = objectSelector;
}



void Editor::ObjectDeleteCommand::undo()
{
  // recreate object
  auto newObject = GET_MANIPULATOR->addEmptyGameObject();

  // get the id of the "new" object
  id = newObject->getID();
  GET_MANIPULATOR->applyTypeRT(id, beforeState);
  GET_MANIPULATOR->setGameObjectName(id, this->originalName);

  auto spaceName = GET_MANIPULATOR->getSpaceName(spaceID);
  GET_MANIPULATOR->moveObjectToSpace(id, spaceName);
}

void Editor::ObjectDeleteCommand::redo()
{
    // delete object again
  GET_MANIPULATOR->removeGameObject(id);
  objSelector->clearSelection();

  m_editor_object->setNeedsDeletion(true);
}

void Editor::ObjectDeleteCommand::Record()
{
}

void Editor::ObjectGizmoCommand::undo()
{
  GET_MANIPULATOR->applyTypeRT(id, beforeState);
  GET_MANIPULATOR->setGameObjectName(id, originalName);

  m_editor_object->setData(beforeState);
}

void Editor::ObjectGizmoCommand::redo()
{
  GET_MANIPULATOR->applyTypeRT(id, afterState);
  GET_MANIPULATOR->setGameObjectName(id, newName);

  m_editor_object->setData(afterState);
}

void Editor::ObjectAddComponentCommand::undo()
{
  GET_MANIPULATOR->applyTypeRT(id, beforeState);
  GET_MANIPULATOR->setGameObjectName(id, originalName);

  m_editor_object->setData(beforeState);
}

void Editor::ObjectAddComponentCommand::redo()
{
  GET_MANIPULATOR->applyTypeRT(id, afterState);
  GET_MANIPULATOR->setGameObjectName(id, newName);

  m_editor_object->setData(afterState);
}


void Editor::ObjectRemoveParentCommand::SetParentID(objID parentObjectID)
{
  this->parentID = parentObjectID;
}

void Editor::ObjectRemoveParentCommand::undo()
{
    // attach parent
  GET_MANIPULATOR->addParentToGameObject(parentID, id);

  m_editor_object->setNeedsUpdate(true);
}

void Editor::ObjectRemoveParentCommand::redo()
{
    // remove parent
  GET_MANIPULATOR->removeParent(id);

  m_editor_object->setNeedsUpdate(true);
}

void Editor::ObjectAddParentCommand::SetParentID(objID parentObjectID)
{
  this->parentID = parentObjectID;
}

void Editor::ObjectAddParentCommand::undo()
{
  GET_MANIPULATOR->removeParent(id);

  m_editor_object->setNeedsUpdate(true);
}

void Editor::ObjectAddParentCommand::redo()
{
  GET_MANIPULATOR->addParentToGameObject(parentID, id);

  m_editor_object->setNeedsUpdate(true);
}


void Editor::ObjectMoveToSpaceCommand::SetOldSpace(objID old_space)
{
  oldSpace = old_space;
}

void Editor::ObjectMoveToSpaceCommand::SetNewSpace(objID new_space)
{
  newSpace = new_space;
}

void Editor::ObjectMoveToSpaceCommand::undo()
{
  GET_MANIPULATOR->moveObjectToSpace(id, oldSpace);

  m_editor_object->setNeedsUpdate(true);
}

void Editor::ObjectMoveToSpaceCommand::redo()
{
  GET_MANIPULATOR->moveObjectToSpace(id, newSpace);

  m_editor_object->setNeedsUpdate(true);
}



