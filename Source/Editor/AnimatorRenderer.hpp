/*!**********************************************************************************************************
\file       AnimatorRenderer.hpp
\author     Henry Brobeck
\date       3/6/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once
#include "Components/ComponentRenderer.hpp"

namespace Editor
{
	class animatorRenderer : public Editor::componentRenderer
	{
	public:
		animatorRenderer(editorWindow * p_parent_window);
	protected:
		void onRender(typeRT& p_type_data, objID p_editor_object_id) override;

	private:

	};
}



// namespace <namespace name>

