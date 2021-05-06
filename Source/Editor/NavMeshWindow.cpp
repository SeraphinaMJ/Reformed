/*!***************************************************************************************
\file       NavMeshWindow.cpp
\author     Henry Brobeck
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "NavMeshWindow.hpp"
#include "Utility/NavMeshGenerator.hpp"
#include "Engine.hpp"
#include "EngineRunner.hpp"
#include "EngineController.hpp"
#include "WaypointNode.hpp"
#include "EditorObjectManager.hpp"
#include "EditorWindow.hpp"
#include "EditorObject.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Editor::navMeshWindow::navMeshWindow(editorWindow * p_parent_editor)
		: windowBase(p_parent_editor)
	{

	}

	void Editor::navMeshWindow::onRender()
	{
		/*
		// render stuff here
		if (ImGui::Button("Generate NavMesh"))
		{
			navMeshGenerator::buildNavMesh();
		}
		if (ImGui::Button("Show Voxel Debug"))
		{
			navMeshGenerator::showVoxelDebug();
		}
		if (ImGui::Button("Hide Voxel Debug"))
		{
			navMeshGenerator::hideVoxelDebug();
		}
		if (ImGui::Button("Show Span Debug"))
		{
			navMeshGenerator::showSpanDebug();
		}
		if (ImGui::Button("Hide Span Debug"))
		{
			navMeshGenerator::hideSpanDebug();
		}
		if (ImGui::Button("Show Top Debug"))
		{
			navMeshGenerator::showTopDebug();
		}
		if (ImGui::Button("Hide Top Debug"))
		{
			navMeshGenerator::hideTopDebug();
		}
		*/
		/*
		if (ImGui::Button("Generate Starting Path"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphCreatePath();
		}
		*/
		if (ImGui::Button("Generate Node Set Path"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphCreateSetPaths();
		}
		/*
		if (ImGui::Button("Stitch Paths"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphStitchPaths();
		}
		*/
		
		if (ImGui::Button("Clear"))
		{
			std::vector<objID> toDelete = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphClear();

			auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
			auto & l_selection_keeper = getSelectionKeeper();
			EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();

			//Just in case something is selected
			l_selection_keeper.clearSelection();

			for (objID d : toDelete)
			{
				objID l_space_id = l_scene_manipulator->getSpaceIDForObject(d);

				l_scene_manipulator->removeGameObject(d);
				l_editor_object_manager.removeEditorObject(d);
				
				//Need to get the new space data from engine (since its now missing an object)
				typeRT l_space_data = l_scene_manipulator->getTypeRT(l_space_id);
				EditorObject * l_space_object = l_editor_object_manager.getEditorObject(l_space_id);
				l_space_object->setData(l_space_data);
			}
		}
		
		if (ImGui::Button("Delete Random Node Set"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphDeleteRandomNodeSet();
		}
		if (ImGui::Button("Find Path"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphTestFunction();
		}
	}

}
