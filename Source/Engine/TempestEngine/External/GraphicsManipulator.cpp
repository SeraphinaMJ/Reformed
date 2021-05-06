#include "GraphicsManipulator.hpp"
#include "GraphicsSystem.hpp"

#include "SystemManagerInterface.hpp"

#include <algorithm>

graphicsManipulator::graphicsManipulator(systemManagerInterface * p_system_manager)
	: m_system_manager{p_system_manager}
{
}

std::vector<std::string> graphicsManipulator::getRenderTextureNames()
{
	if (getGraphicsSystem())
	{
		static std::vector<std::string> names{
			graphicsSystem::textureNames.cbegin(),
			graphicsSystem::textureNames.cend()
		};

		return names;
	}
	else
	{
		return std::vector<std::string>{};
	}

}

void graphicsManipulator::setRenderTextureIndex(int index)
{
	if (auto l_graphics = getGraphicsSystem(); l_graphics)
		l_graphics->m_textureIndex = std::clamp<int>(index, -1, graphicsSystem::s_texCount);
}

int graphicsManipulator::getRenderTextureIndex()
{
	if (auto l_graphics = getGraphicsSystem(); l_graphics)
		return l_graphics->m_textureIndex;
	else
		return -1;
}

void graphicsManipulator::reloadShaders()
{
	if (auto l_graphics = getGraphicsSystem(); l_graphics)
	{
		l_graphics->shutdownShaders();
		l_graphics->initShaders();
	}
}

bool graphicsManipulator::isDisplayingWireframes()
{
	if (auto l_graphics = getGraphicsSystem(); l_graphics)
		return l_graphics->m_showWireframes;
	else
		return false;
}

void graphicsManipulator::displayWireframes(bool show)
{
	if (auto l_graphics = getGraphicsSystem(); l_graphics)
		l_graphics->m_showWireframes = show;
}

bool graphicsManipulator::isDisplayingDebugLines()
{
	if (auto l_graphics = getGraphicsSystem(); l_graphics)
		return l_graphics->m_showDebugLines;
	else
		return false;
}

void graphicsManipulator::displayDebugLines(bool show)
{
	if(auto l_graphics = getGraphicsSystem(); l_graphics)
		l_graphics->m_showDebugLines = show;
}

viewportGeometry const& graphicsManipulator::getViewport()
{
	return getGraphicsSystem()->getViewport();
}

graphicsSystem * graphicsManipulator::getGraphicsSystem()
{
	return m_system_manager->getSystem<graphicsSystem>();
}
