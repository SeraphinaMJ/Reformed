#pragma once
#include "Scaffolding/WindowBase.hpp"

namespace Editor
{
	class graphicsWindow : public windowBase
	{
	public:
		graphicsWindow(editorWindow * p_parent_window);
		~graphicsWindow() override = default;

		void onRender() override;
	};
}
