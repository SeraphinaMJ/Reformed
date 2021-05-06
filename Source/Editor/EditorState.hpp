/*!***************************************************************************************
\file       EditorState.hpp
\author     Aaron Damyen
\date       1/26/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a structure for holding state of the editor
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

namespace Editor
{
	enum class guizmoOperation
	{
		Translate,
		Rotate,
		Scale,
	};

	enum class guizmoMode
	{
		Local,
		Global,
	};

	/*!***************************************************************************************
	\par class: editorState
	\brief   This is a class for holding the current state of the editor
	*****************************************************************************************/
	class editorState
	{
	public:
		guizmoOperation m_guizmo_operation;
		guizmoMode m_guizmo_mode;

		/*!***************************************************************************************
		\brief  This just for giving an example of a function format.
		\param exampleIn - dat old boring int
		\return What is this returning?
		*****************************************************************************************/
	};
} // namespace Editor
