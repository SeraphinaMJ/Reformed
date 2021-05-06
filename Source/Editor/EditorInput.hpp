/*!***************************************************************************************
\file       EditorInput.hpp
\author     Charles Boudousquie
\date       9/9/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the editor's delegate object that handles special user input.
*****************************************************************************************/
#pragma once

// 
namespace Editor
{

    class engineController;

    /*!***************************************************************************************
    \par class: EditorInput
    \brief  queries imgui for any important info related to user input
            like ctrl + z for undo
    *****************************************************************************************/
    class EditorInput
    {
        engineController* controller;
    public:
        EditorInput(engineController * eController);

        void Update();

    };

};












