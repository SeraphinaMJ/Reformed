/*!***************************************************************************************
\file       InspectorWindow.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the inspector window class
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Scaffolding/WindowBase.hpp"
#include "InspectorRenderer.hpp"
#include "StyleKeeper.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class typeRT;

namespace Editor
{
	//======== Forward Declarations=======================================================//
	class hierarchySelectionKeeper;
	class editorWindow;
	class engineController;

	/*!***************************************************************************************
	\par class: inspectorWindow
	\brief   This class handles rendering the inspectorWindow in the editor
	*****************************************************************************************/
	class inspectorWindow : public windowBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the window with the parameters for the bases
		\param p_parent_editor - reference to the editorWindow parent
		*****************************************************************************************/
		inspectorWindow(editorWindow * p_parent_editor);

	protected:
		/*!***************************************************************************************
		\brief  Handles the render event for the window, rendering the window appropriately
		*****************************************************************************************/
		void onRender() override;

	private:

		inspectorRenderer m_renderer;
    bool m_engine_data_render;
    int m_engine_data_timestep;
    float m_timer;
	};


} // namespace Editor
