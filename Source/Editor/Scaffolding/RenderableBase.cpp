/*!***************************************************************************************
\file       RenderableBase.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a base class for anything renderable in the Editor
*****************************************************************************************/
#include "RenderableBase.hpp"
#include "../EditorWindow.hpp"

Editor::renderableBase::renderableBase(editorWindow* p_top_window)
	: m_top_window{p_top_window}
{
}

void Editor::renderableBase::render()
{
	// nothing to do yet, just notify derivative
	onRender();
}

bool Editor::renderableBase::isRunning()
{
  return m_top_window->isRunning();
}

Editor::EditorLogger& Editor::renderableBase::getLogger()
{
  return m_top_window->getLogger();
}

Editor::engineController & Editor::renderableBase::getEngineController() const
{ 
	return getTopWindow()->getEngineController(); 
}

Editor::hierarchySelectionKeeper & Editor::renderableBase::getSelectionKeeper() const
{
	return getTopWindow()->getSelectionKeeper();
}

Editor::editorState & Editor::renderableBase::getEditorState() const
{
	return getTopWindow()->getEditorState();
}

Editor::styleKeeper & Editor::renderableBase::getStyleKeeper() const
{
	return getTopWindow()->getStyleKeeper();
}

