/*!***************************************************************************************
\file       WindowBase.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a base for windows in the editor
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../EditorState.hpp"
#include "RenderableBase.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor
{
	//======== Forward Declarations=======================================================//
	class editorWindow;
	class engineController;

	/*!***************************************************************************************
	\par class: windowBase
	\brief   This is the base for windows in the editor
	*****************************************************************************************/
	class windowBase : public renderableBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the window with required base parameters
		\param p_parent_window - reference to the containing editor window
		\param p_engine_controller - reference to the engine controller
		*****************************************************************************************/
		windowBase(editorWindow * p_parent_window);
		/*!***************************************************************************************
		\brief  Destroys the window, releasing appropriate resources
		*****************************************************************************************/
		virtual ~windowBase() = default;

	protected:

        

	private:
	};

} // namespace Editor

