/*!***************************************************************************************
\file       RenderableBase.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a base class for anything renderable in the Editor
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor
{
	//======== Forward Declarations=======================================================//
	class editorWindow;
    struct EditorLogger;
	class engineController;
	class hierarchySelectionKeeper;
	class editorState;
	class styleKeeper;

	/*!***************************************************************************************
	\par class: renderableBase
	\brief   This is a base class for renderable objects in the editor
	*****************************************************************************************/
	class renderableBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs this object with the containing editorWindow reference
		\param p_top_window - reference to the top-most window in the editor
		*****************************************************************************************/
		renderableBase(editorWindow * p_top_window);
		/*!***************************************************************************************
		\brief  Destroys this object, free resources as required
		*****************************************************************************************/
		virtual ~renderableBase() = default;

		/*!***************************************************************************************
		\brief  Renders this object to the screen
		*****************************************************************************************/
		void render();

    bool isRunning();

    EditorLogger & getLogger();

	protected:
		/*!***************************************************************************************
		\brief  Handles the render event in subclasses to perform specialized rendering
		*****************************************************************************************/
		virtual void onRender() = 0;

		/*!***************************************************************************************
		\brief  Retrieves the reference to the top-most, editorWindow object
		\return editorWindow * - pointer to the editorWindow this renderable is contained within.
		*****************************************************************************************/
		editorWindow * getTopWindow() const { return m_top_window; }

		/*!***************************************************************************************
		\brief  Retrieves the reference to the engine controller
		\return engineController & - reference to the engine controller
		*****************************************************************************************/
		engineController & getEngineController() const;

		/*!***************************************************************************************
		\brief  Retrieves the reference to the selection keeper
		\return hierarchySelectionKeeper & - reference to the selection keeper
		*****************************************************************************************/
		hierarchySelectionKeeper & getSelectionKeeper() const;

		/*!***************************************************************************************
		\brief  Retrieves a reference to the style keeper object within this editor
		\return styleKeeper & - Reference to the style keeper
		*****************************************************************************************/
		styleKeeper & getStyleKeeper() const;


		/*!***************************************************************************************
		\brief  Retrieves the reference to the editor state
		\return editorState & - reference to the editor state
		*****************************************************************************************/
		editorState & getEditorState() const;

	private:
		editorWindow * m_top_window;
	};


} // namespace Editor

