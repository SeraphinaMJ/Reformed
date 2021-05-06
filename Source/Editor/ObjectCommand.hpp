/*!***************************************************************************************
\file       ObjectCommand.hpp
\author     Charles Boudousquie, Ryan Booth
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the object command abstract class.
*****************************************************************************************/
#pragma once

#include "Command.hpp"
#include "EditorObject.hpp"
#include "EngineRunner.hpp"
#include "EngineController.hpp"
#include "Engine.hpp"

namespace Editor
{
#define GET_MANIPULATOR eController->getEngineRunner()->getEngine()->getSceneManipulator().lock()

    class hierarchySelectionKeeper;

    /*!***************************************************************************************
    \par class: ObjectCommand
    \brief   This class is the abstract representation of modifying an object.
    *****************************************************************************************/
    class ObjectCommand : public Command
    {
    public:
      
    };

    /*!***************************************************************************************
    \par class: ObjectMoveToSpaceCommand
    \brief   This class moving an object from one space to another.
    *****************************************************************************************/
    class ObjectMoveToSpaceCommand : public ObjectCommand
    {
        objID oldSpace;
        objID newSpace;

    public:

        void SetOldSpace(objID old_space);
        void SetNewSpace(objID new_space);

        /*!***************************************************************************************
        \brief Sends back to original space
        *****************************************************************************************/
        void undo() override;

        /*!***************************************************************************************
        \brief Sends object to new space.
        *****************************************************************************************/
        void redo() override;

    };


    /*!***************************************************************************************
    \par class: ObjectAddParentCommand
    \brief   This class represents the action of adding the parent of an object.
    *****************************************************************************************/
    class ObjectAddParentCommand : public ObjectCommand
    {
        objID parentID;

    public:

        void SetParentID(objID parentObjectID);

        /*!***************************************************************************************
        \brief Reattaches parent
        *****************************************************************************************/
        void undo();
        /*!***************************************************************************************
        \brief Detaches parent again
        *****************************************************************************************/
        void redo();
    };


    /*!***************************************************************************************
    \par class: ObjectRemoveParentCommand
    \brief   This class represents the action of removing the parent of an object.
    *****************************************************************************************/
    class ObjectRemoveParentCommand : public ObjectCommand
    {
        objID parentID;

    public:

        void SetParentID(objID parentObjectID);

        /*!***************************************************************************************
        \brief Reattaches parent
        *****************************************************************************************/
        void undo();
        /*!***************************************************************************************
        \brief Detaches parent again
        *****************************************************************************************/
        void redo();
    };

    /*!***************************************************************************************
    \par class: ObjectDeleteCommand
    \brief   This class represents the action of deleting an object.
    *****************************************************************************************/
    class ObjectDeleteCommand : public ObjectCommand
    {
        hierarchySelectionKeeper* objSelector;

        objID spaceID;

        // Do not call record on this object!
    public:

        void SetSpace(objID spaceID);

        /*!***************************************************************************************
        \brief Gives Delete command the object selector so it can clear it when object is deleted
        \param objectSelector - the object selector
        *****************************************************************************************/
        void SetObjectSelector(hierarchySelectionKeeper* objectSelector);

        /*!***************************************************************************************
        \brief Undos a change done to the object
        *****************************************************************************************/
        void undo() override;
        /*!***************************************************************************************
        \brief Redos a change done to the object
        *****************************************************************************************/
        void redo() override;

        /*!***************************************************************************************
        \brief There is nothing to record if object is deleted
        *****************************************************************************************/
        void Record() override;
    };

    class ObjectGizmoCommand : public ObjectCommand
    {
    public:
      void undo() override;

      void redo() override;
    };

    class ObjectAddComponentCommand : public ObjectCommand
    {
    public:
      void undo() override;

      void redo() override;
    };

    /*!***************************************************************************************
    \par class: ObjectDataChangeCommand
    \brief   This class represents the action of modifying an object.
    *****************************************************************************************/
    template <typename T>
    class ObjectDataChangeCommand : public ObjectCommand
    {
    public:
      template <typename T>
      ObjectDataChangeCommand(func_t<T> p_execute_function, typeRT & p_data, void* p_data_type, size_t p_size)
        :m_data(p_data), m_execute_function(p_execute_function)
      {
		  //std::memcpy(&m_data_type, (p_data_type), p_size);
		  m_data_type = *static_cast<T*>(p_data_type);
      }

	  template <typename T>
	  ObjectDataChangeCommand(func_t<T> p_execute_function, typeRT & p_data, T p_data_type)
		  :m_data_type(p_data_type), m_data(p_data), m_execute_function(p_execute_function)
	  {

	  }

      template<typename T>
      static CommandPtr createDataChangeCommand(engineController * p_engine_controller, EditorObject * p_editor_object, func_t<T> p_execute_function, typeRT & p_data, void * p_data_type, size_t p_size)
      {
        // create command to record change done to object inside inspector
        auto l_command = std::make_shared<ObjectDataChangeCommand<T>>(p_execute_function, p_data, p_data_type, p_size);
        l_command->Init(p_engine_controller, p_editor_object);

        CommandPtr l_return = l_command;

        return l_return;
      }

	  template<typename T>
	  static CommandPtr createDataChangeCommand(engineController * p_engine_controller, EditorObject * p_editor_object, func_t<T> p_execute_function, typeRT & p_data, T p_data_type)
	  {
		  // create command to record change done to object inside inspector
		  auto l_command = std::make_shared<ObjectDataChangeCommand<T>>(p_execute_function, p_data, p_data_type);
		  l_command->Init(p_engine_controller, p_editor_object);

		  CommandPtr l_return = l_command;

		  return l_return;
	  }

      /*!***************************************************************************************
      \brief Undos a change done to the object
      *****************************************************************************************/
      void undo() override
      {
        GET_MANIPULATOR->applyTypeRT(id, beforeState);
        GET_MANIPULATOR->setGameObjectName(id, originalName);

        m_editor_object->setData(beforeState);
      }

      /*!***************************************************************************************
      \brief Redos a change done to the object
      *****************************************************************************************/
      void redo() override
      {
        GET_MANIPULATOR->applyTypeRT(id, afterState);
        GET_MANIPULATOR->setGameObjectName(id, newName);

        m_editor_object->setData(afterState);
      }

      //template<typename T>
      void execute() override
      {
        if(m_editor_object != nullptr)
        {
          m_execute_function(m_data, m_data_type);

          m_editor_object->setDirty(true);
        }
      }

    private:
      T m_data_type;
      typeRT & m_data;
      func_t<T> m_execute_function;
    };
};
