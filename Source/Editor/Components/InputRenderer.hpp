/*!***************************************************************************************
\file       InputRenderer.hpp
\author     Aaron Damyen
\date       3/15/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is an inspector renderer override for the inputComponent
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "ComponentRenderer.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor
{
	/*!***************************************************************************************
	\par class: inputRenderer
	\brief   Overrides the default rendering for an inputComponent so the lists can render properly.
	*****************************************************************************************/
	class inputRenderer : public componentRenderer
	{
	public:
		inputRenderer(editorWindow * p_parent_window);

	protected:
		void onRender(typeRT & p_type_data, objID p_editor_object_id) override;
	private:

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/
	};

} // namespace <namespace name>

