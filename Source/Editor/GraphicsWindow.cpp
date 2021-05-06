#include "GraphicsWindow.hpp"
#include "EngineController.hpp"
#include "EngineRunner.hpp"
#include "Engine.hpp"
#include "GraphicsManipulatorInterface.hpp"

#include <imgui.h>
#include <algorithm>

namespace Editor
{
	graphicsWindow::graphicsWindow(editorWindow * p_parent_window)
		: windowBase{ p_parent_window }
	{
	}

	void graphicsWindow::onRender()
	{
		auto&& manip = getEngineController().getEngineRunner()->getEngine()->getGraphicsManipulator().lock();
		
		[&manip]
		{
			if (ImGui::Button("Reload Shaders"))
			{
				manip->reloadShaders();
			}
		}();

		[&manip]
		{
			const auto names = manip->getRenderTextureNames();
			const auto modNames = [&names]() -> std::vector<const char *>
			{
				std::vector<const char *> modNames(names.size() + 1, nullptr);
				modNames.front() = "Don't Display";
				std::transform(names.cbegin(), names.cend(), ++modNames.begin(),
					[](const auto & name)
					{
						return name.c_str();
					}
				);
				return modNames;
			}();

			if (int index = manip->getRenderTextureIndex() + 1; 
				ImGui::Combo("Debug Output Texture", &index, modNames.data(), modNames.size()))
			{
				manip->setRenderTextureIndex(index - 1);
			}
		}();

		[&manip]
		{
			if (bool show = manip->isDisplayingDebugLines();
				ImGui::Checkbox("Show Debug Lines", &show))
				manip->displayDebugLines(show);

			ImGui::SameLine();

			if (bool show = manip->isDisplayingWireframes();
				ImGui::Checkbox("Show Debug Wireframes", &show))
				manip->displayWireframes(show);
		}();

		//ImGui::End();
	}
}
