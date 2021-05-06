/*!***************************************************************************************
\file       EditorObjectManager.cpp
\author     Ryan Booth
\date       10/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include "EditorObjectManager.hpp"

#include "EditorObject.hpp"
#include "EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "EditorLogger.hpp"
#include "ObjectCommand.hpp"
#include "UndoRedo.hpp"

Editor::EditorObjectManager::EditorObjectManager(Editor::engineController & p_engine_controller, Editor::EditorLogger & p_logger)
  : m_engine_controller(p_engine_controller), m_logger(p_logger)
{

}

Editor::EditorObject* Editor::EditorObjectManager::getEditorObject(objID p_object_id)
{
  auto i_object = m_objects.find(p_object_id);

  if(i_object != m_objects.end())
  {
    return i_object->second;
  }
  else
  {
    m_logger.AddLog("[ERROR] Unable to find object with given id: %lu \n", p_object_id);
  }

  return nullptr;
}

void Editor::EditorObjectManager::addEditorObject(EditorObject * p_object)
{
  if(p_object != nullptr)
  {
    objID l_object_id = p_object->getObjectID();

    auto i_object = m_objects.find(l_object_id);

    //object already exists
    if (i_object != m_objects.end())
    {
      //m_logger.AddLog("[ERROR] Attempting to add object to Editor Object Manager that already exists with id: %lu \n", l_object_id);

      return;
    }
    else
    {
      m_objects.insert({ l_object_id, p_object });
    }
  }
}

void Editor::EditorObjectManager::removeEditorObject(objID p_object_id)
{
  auto i_object = m_objects.find(p_object_id);

  if (i_object != m_objects.end())
  {
    delete i_object->second;

    i_object->second = nullptr;

    m_objects.erase(i_object);
  }
  else
  {
    m_logger.AddLog("[ERROR] Unable to find object with given id: %lu \n", p_object_id);
  }
}

void Editor::EditorObjectManager::removeEditorObject(EditorObject* p_object)
{
  if (p_object != nullptr)
  {
    objID l_object_id = p_object->getObjectID();

    auto i_object = m_objects.find(l_object_id);

    if (i_object != m_objects.end())
    {
      delete i_object->second;

      i_object->second = nullptr;

      m_objects.erase(i_object);
    }
    else
    {
      m_logger.AddLog("[ERROR] Unable to find object with given id: %lu \n", l_object_id);
    }

  }
  else
  {
    m_logger.AddLog("[ERROR] NULL ptr passed to Editor Object Manager.\n");
  }
}

void Editor::EditorObjectManager::clearObjectList()
{
  for (auto i_object = m_objects.begin(); i_object != m_objects.end(); ++i_object)
  {
    delete i_object->second;
  }

  m_objects.clear();
}

std::map<objID, Editor::EditorObject*> Editor::EditorObjectManager::getEditorObjectsOfType(EditorObjectType p_object_type)
{
  std::map<objID, EditorObject *> l_result;

  for(auto i_object = m_objects.begin(); i_object != m_objects.end(); ++i_object)
  {
    if(i_object->second->getObjectType() == p_object_type)
    {
      l_result.insert({i_object->first, i_object->second});
    }
  }

  return l_result;
}

void Editor::EditorObjectManager::updateAllEditorObjectsThatNeedUpdateFromEngine()
{
  for (auto i_object = m_objects.begin(); i_object != m_objects.end(); ++i_object)
  {
    if (i_object->second->getNeedsUpdate())
    {
      typeRT l_new_data = m_engine_controller.getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(i_object->second->getObjectID());
      i_object->second->setData(l_new_data);

      i_object->second->setNeedsUpdate(false);
      i_object->second->setDirty(false);
    }
  }
}

void Editor::EditorObjectManager::updateAllEditorObjectsFromEngine()
{
  for (auto i_object = m_objects.begin(); i_object != m_objects.end(); ++i_object)
  {
    typeRT l_new_data = m_engine_controller.getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(i_object->second->getObjectID());
    i_object->second->setData(l_new_data);

    i_object->second->setNeedsUpdate(false);
    i_object->second->setDirty(false);
  }
}

void Editor::EditorObjectManager::updateEditorObjectFromEngine(objID p_object_id, const typeRT & p_data)
{
  auto i_object = m_objects.find(p_object_id);

  if (i_object != m_objects.end())
  {
    i_object->second->setData(p_data);

    i_object->second->setDirty(false);
  }
  else
  {
    m_logger.AddLog("[ERROR] Unable to find object with given id: %lu \n", p_object_id);
  }
}

void Editor::EditorObjectManager::sendDirtyObjectDataToEngine(objID p_object_id)
{
  auto i_object = m_objects.find(p_object_id);

  if (i_object != m_objects.end())
  {
    if (i_object->second->getDirty())
    {
      m_engine_controller.getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(i_object->first, i_object->second->getData());
      i_object->second->setDirty(false);
    }
  }
  else
  {
    //m_logger.AddLog("[ERROR] Unable to find object with given id: %lu \n", p_object_id);
  }
}

void Editor::EditorObjectManager::sendAllDirtyObjectDataToEngine()
{
  //Check for any objects that need to be deleted
  for (auto i_object = m_objects.begin(); i_object != m_objects.end(); ++i_object)
  {
    if (i_object->second->getNeedsDeletion())
    {
      removeEditorObject(i_object->second);
      break;
    }
  }

  for(auto i_object = m_objects.begin(); i_object != m_objects.end(); ++i_object)
  {
    if(i_object->second->getDirty())
    {
      m_engine_controller.getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(i_object->first, i_object->second->getData());
      i_object->second->setDirty(false);
    }
  }
}

bool Editor::EditorObjectManager::ifEditorObjectExists(objID p_object_id)
{
  auto i_object = m_objects.find(p_object_id);

  if (i_object != m_objects.end())
  {
    return true;
  }

  return false;
}

void Editor::EditorObjectManager::addCommandToQueue(Editor::CommandPtr p_command)
{
  m_command_queue.push(p_command);
}

void Editor::EditorObjectManager::parseCommandsInQueue()
{
  while(!m_command_queue.empty())
  {
    //Grab the top command
    Editor::CommandPtr l_current_command = m_command_queue.front();
    m_command_queue.pop();

    //Execute command
    l_current_command->execute();

    //Record command
    l_current_command->Record();
    UndoRedoManager::GetInstance().RecordState(l_current_command, &m_engine_controller);
  }
}

typeRT Editor::EditorObjectManager::getTypeRTFromEngine(objID p_obj_id) const
{
  typeRT l_type_data = m_engine_controller.getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(p_obj_id);

  return l_type_data;
}
