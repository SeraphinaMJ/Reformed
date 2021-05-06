/*!***************************************************************************************
\file       UndoRedo.hpp
\author     Charles Boudousquie
\date       9/6/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the undo redo manager.
*****************************************************************************************/
#pragma once

//======== 3rd Party Includes ==========================================================//
//#include <deque>
#include <string>
#include <stack>
#include <memory> // std::shared_ptr

namespace Editor
{
    class engineController;
    class Command;

    typedef std::shared_ptr<Command> CommandPtr;

    /*!***************************************************************************************
    \par class: UndoRedoManager
    \brief   This is the class which knows how to undo and redo user changes.
    *****************************************************************************************/
    class UndoRedoManager
    {

        /*!***************************************************************************************
        \brief Constrcutor, private by default
        *****************************************************************************************/
        UndoRedoManager() {}

    public:

        /*!***************************************************************************************
        \brief Singleton access
        *****************************************************************************************/
        static UndoRedoManager& GetInstance()
        {
            static UndoRedoManager instance;
            return instance;
        }

        /*!***************************************************************************************
        \brief Records current state into a file
        *****************************************************************************************/
        void RecordState(CommandPtr com, engineController* controller);

        /*!***************************************************************************************
        \brief undoes an operation done by the user
        *****************************************************************************************/
        void Undo(engineController* controller);

        /*!***************************************************************************************
        \brief redos an operation done by the user
        *****************************************************************************************/
        void Redo(engineController* controller);

        /*!***************************************************************************************
        \brief if user wishes to store a command for later completion and use, like in the gizmo.
        *****************************************************************************************/
        void StoreCommand(CommandPtr incompleteCommand);

        /*!***************************************************************************************
        \brief if user wishes to modify a command they stored for later.
        *****************************************************************************************/
        CommandPtr GetStoredCommand();

        /*!***************************************************************************************
        \brief if user wishes to clear a command they stored.
        *****************************************************************************************/
        void ClearStoredCommand();

        /*!***************************************************************************************
        \brief Clear all undo redo history
        *****************************************************************************************/
        void ClearHistory();

    private:

        std::stack<CommandPtr> undoStack;
        std::stack<CommandPtr> redoStack;

        CommandPtr futureCommand; // when gizmo needs to store a command for future use
    };
}


