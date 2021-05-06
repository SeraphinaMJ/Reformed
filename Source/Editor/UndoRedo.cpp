/*!***************************************************************************************
\file       UndoRedo.cpp
\author     Charles Boudousquie
\date       9/6/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the undo redo manager.
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
#include "UndoRedo.hpp"
#include "PersistenceManipulator.hpp"

#include "EngineController.hpp"
#include "Command.hpp"

//Editor::UndoRedoManager::UndoRedoManager() //: eventHandler(new UndoRedoEventHandler(*this))
//{
//    //EditorEventSystem::GetInstance().Subscribe(EventType::RECORD, eventHandler);
//    //EditorEventSystem::GetInstance().Subscribe(EventType::UNDO, eventHandler);
//    //EditorEventSystem::GetInstance().Subscribe(EventType::REDO, eventHandler);
//}

//std::deque<std::string> Editor::UndoRedoManager::CreateHistoryCopy(std::deque<std::string>& history)
//{
//    if (history.empty() == true)
//    {
//        return {};
//    }
//    else
//    {
//        std::deque<std::string> copy(history.begin(), currentState);
//        return copy;
//    }
//}

void Editor::UndoRedoManager::RecordState(CommandPtr com, engineController* controller)
{
    // push the command onto the undo stack since we will want to undo it
    undoStack.push(com);

    // clear redo stack for consistency
    redoStack = {};
}

void Editor::UndoRedoManager::Undo(engineController* controller)
{
    // if nothing to undo then do nothing
    if (undoStack.empty())
    {
        return;
    }

    auto command = undoStack.top();
    undoStack.pop();

    command->undo();

    redoStack.push(command);
    //// if no states to record then return
    //if (history.empty() == true)
    //{
    //    return;
    //}
    //// if no state to go back to then do nothing
    //if (currentState == history.begin())
    //{
    //    return;
    //}
    //// revert to previous state
    //currentState--;
    //// load previous state
    //// todo
    //controller->getEngine()->getPersistenceManipulator().lock()->load(*currentState);
}

void Editor::UndoRedoManager::Redo(engineController* controller)
{
    // if nothing to redo then return
    if (redoStack.empty())
    {
        return;
    }

    // get most recent command from the redo stack
    auto command = redoStack.top();
    redoStack.pop();

    // redo this command
    command->redo();

    // add command to undo stack
    undoStack.push(command);

    //// if no states to record then return
    //if (history.empty() == true)
    //{
    //    return;
    //}
    //// if no state to redo just return
    //if (std::next(currentState) == history.end())
    //{
    //    return;
    //}
    //// go to next state
    //currentState++;
    //// load the scene
    ////todo
    ////auto string = getEngineController()->getEngine()->getAssetManipulator().lock()->stringFromWide(l_path);
    //// todo
    ////persistenceManipulator::loadScene(*currentState);
    //controller->getEngine()->getPersistenceManipulator().lock()->load(*currentState);

}

void Editor::UndoRedoManager::StoreCommand(CommandPtr incompleteCommand)
{
    futureCommand = incompleteCommand;
}

Editor::CommandPtr Editor::UndoRedoManager::GetStoredCommand()
{
    return futureCommand;
}

void Editor::UndoRedoManager::ClearStoredCommand()
{
    futureCommand = nullptr;
}

void Editor::UndoRedoManager::ClearHistory()
{
    futureCommand = nullptr;
    undoStack = {};
    redoStack = {};
}











