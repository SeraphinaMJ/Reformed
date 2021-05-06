/*!***************************************************************************************
\file       HierarchyWindow.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the hierarchy window class
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Scaffolding/WindowBase.hpp"
#include "HierarchySelectionKeeper.hpp"
#include "HierarchyRenderer.hpp"
//======== 3rd Party Includes ==========================================================//
#include <memory>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class scene;
class space;
class gameObject;

namespace Editor
{
	//======== Forward Declarations=========================================================//
	class engineController;
	class editorWindow;

	/*!***************************************************************************************
	\par class: hierarchyWindow
	\brief   This class implements the hierarchy window in the editor
	*****************************************************************************************/
	class hierarchyWindow : public windowBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the window object with appropriate parameters for base class
		\param p_parent_editor - reference to the editorWindow this class is contained within
		*****************************************************************************************/
		hierarchyWindow(editorWindow * p_parent_editor);

    void createEditorObjects();

	protected:
		/*!***************************************************************************************
		\brief  Renders the window to the screen
		*****************************************************************************************/
		void onRender() override;


	private:
		hierarchyRenderer m_renderer;
	};


} // namespace Editor

