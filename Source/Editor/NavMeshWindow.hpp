/*!***************************************************************************************
\file       NavMeshWindow.hpp
\author     Henry Brobeck
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Scaffolding/WindowBase.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor {


	//======== Forward Declarations ======================================================//
	class editorWindow;
	class engineController;

	/*!***************************************************************************************
	\par class: audioWindow
	\brief   This is the interface for the audio window in the editor
	*****************************************************************************************/
	class navMeshWindow : public windowBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the audioWindow with appropriate base constructor parameters
		\param p_parent_editor - pointer to the containing editorWindow
		*****************************************************************************************/
		navMeshWindow(editorWindow * p_parent_editor);

	protected:
		/*!***************************************************************************************
		\brief  Handles the render event for the window
		*****************************************************************************************/
		void onRender() override;

	private:
	};


} // namespace Editor

