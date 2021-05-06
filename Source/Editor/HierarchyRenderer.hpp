/*!***************************************************************************************
\file       HierarchyRenderer.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the renderer of the hierarchy window content.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Scaffolding/RenderableBase.hpp"
//======== 3rd Party Includes ==========================================================//
#include "SceneManagement/Space.hpp"
#include <memory>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class scene;

namespace Editor
{
	//======== Forward Declarations=======================================================//
	class hierarchySelectionKeeper;
	class editorWindow;

	/*!***************************************************************************************
	\par class: hierarchyRenderer
	\brief   This class renders the content of the hierarchy window
	*****************************************************************************************/
	class hierarchyRenderer : public renderableBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the renderer with the needed selection keeper
		\param p_parent_window - pointer to the top level window
		*****************************************************************************************/
		hierarchyRenderer(editorWindow * p_parent_window);

    void createAllEditorObjects(std::vector<std::shared_ptr<scene>> p_scenes);

	protected:

		void onRender() override;

	private:

		/*!***************************************************************************************
		\brief  Renders the contents of the hierarchy window
		\param p_scene - the scene whose contents should be rendered
		*****************************************************************************************/
		void onRender(std::vector<std::shared_ptr<scene>> p_scene);

		/*!***************************************************************************************
		\brief  Renders the contents of a scene object to the hierarchy
		\param p_scene - The scene object to be rendered
		*****************************************************************************************/
		void renderScene(std::shared_ptr<scene> p_scene);
		/*!***************************************************************************************
		\brief  Renders the contents of a space object to the hierarchy
		\param p_space - The space object to be rendered
		*****************************************************************************************/
		void renderSpace(std::shared_ptr<space> p_space);
		/*!***************************************************************************************
		\brief  Renders the contents of a game object to the hierarchy
		\param p_object - The game object to be rendered
		*****************************************************************************************/
		void renderGameObject(std::shared_ptr<gameObject> p_object);



    objID m_scene_id;
	};

} // namespace Editor


