/*!***************************************************************************************
\file       GizmoRenderer.cpp
\author     Aaron Damyen
\date       2/7/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the gizmo rendering class members
*****************************************************************************************/

//======== Self Include ================================================================//
#include "GizmoRenderer.hpp"
//======== 1st Party Includes ==========================================================//
#include "../EngineController.hpp"
#include "../HierarchySelectionKeeper.hpp"
#include "../ObjectCommand.hpp"
#include "../UndoRedo.hpp"
#include "../EditorWindow.hpp"
//======== 3rd Party Includes ==========================================================//
#include <imgui.h>
#include <ImGuizmo.h>
#include <EngineRunner.hpp>
#include <Engine.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//
static void TransposeMatrix(float * p_dest)
{
	std::swap(p_dest[1], p_dest[4]);
	std::swap(p_dest[2], p_dest[8]);
	std::swap(p_dest[3], p_dest[12]);
	std::swap(p_dest[6], p_dest[9]);
	std::swap(p_dest[7], p_dest[13]);
	std::swap(p_dest[11], p_dest[14]);
}

static void SetIdentity(float * p_dest)
{
	p_dest[0] = p_dest[5] = p_dest[10] = p_dest[15] = 1;
	p_dest[1] = p_dest[2] = p_dest[3] = p_dest[4] = p_dest[6] = p_dest[7]
		= p_dest[8] = p_dest[9] = p_dest[11] = p_dest[12] = p_dest[13] = p_dest[14] = 0;
}

static ImGuizmo::OPERATION convertOperation(Editor::guizmoOperation p_op)
{
	if (p_op == Editor::guizmoOperation::Translate) return ImGuizmo::OPERATION::TRANSLATE;
	if (p_op == Editor::guizmoOperation::Rotate) return ImGuizmo::OPERATION::ROTATE;
	if (p_op == Editor::guizmoOperation::Scale) return ImGuizmo::OPERATION::SCALE;
	return ImGuizmo::OPERATION::TRANSLATE;
}

static ImGuizmo::MODE convertMode(Editor::guizmoMode p_mode)
{
	if (p_mode == Editor::guizmoMode::Local) return ImGuizmo::MODE::LOCAL;
	if (p_mode == Editor::guizmoMode::Global) return ImGuizmo::MODE::WORLD;
	return ImGuizmo::MODE::LOCAL;
}

//======== Constructors & Destructor ===================================================//
Editor::gizmoRenderer::gizmoRenderer(editorWindow * p_parent_window)
	: renderableBase(p_parent_window), m_changed(false), m_using_id(0)
{
}

//======== Getters & Setters ===========================================================//
bool Editor::gizmoRenderer::getUsingGizmo() const
{
	return m_is_using_guizmo;
}

void Editor::gizmoRenderer::setDimensions(float p_x, float p_y, float p_width, float p_height)
{
	m_x = p_x;
	m_y = p_y;
	m_width = p_width;
	m_height = p_height;
}

void Editor::gizmoRenderer::setOperation(guizmoOperation p_operation)
{
	m_operation = p_operation;
}

Editor::guizmoOperation Editor::gizmoRenderer::getOperation() const
{
	return m_operation;
}

void Editor::gizmoRenderer::setMode(guizmoMode p_mode)
{
	m_mode = p_mode;
}

Editor::guizmoMode Editor::gizmoRenderer::getMode() const
{
	return m_mode;
}

Editor::guizmoOperation& Editor::gizmoRenderer::getOperationRef()
{
	return m_operation;
}

Editor::guizmoMode& Editor::gizmoRenderer::getModeRef()
{
	return m_mode;
}

void Editor::gizmoRenderer::setUsingGizmo(bool p_state)
{
    // if user starts using the gizmo record that for future use
    if (m_is_using_guizmo == false && p_state == true)
    {
      CommandPtr newCommand = std::make_shared<ObjectGizmoCommand>();
      EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
      newCommand->Init(&getEngineController(), l_editor_object_manager.getEditorObject(getSelectionKeeper().getSelectionId()));
      UndoRedoManager::GetInstance().StoreCommand(newCommand);
      m_user_started_gizmo = true;
      m_using_id = getSelectionKeeper().getSelectionId();
    }
	m_is_using_guizmo = p_state;

  if(p_state)
  {
    m_changed = true;
    //getTopWindow()->getSceneWindow().setAllowSelection(false);
  }
  else
  {
    //getTopWindow()->getSceneWindow().setAllowSelection(true);
  }

}

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//
void Editor::gizmoRenderer::pullData()
{
  if(!getEngineController().isPlaying())
  {
    if (getSelectionKeeper().isGameObjectSelected())
    {
      if(ImGuizmo::IsOver())
      {
        getTopWindow()->getSceneWindow().setAllowSelection(false);
      }
      else
      {
        getTopWindow()->getSceneWindow().setAllowSelection(true);
      }

      m_using_id = getSelectionKeeper().getSelectionId();

      auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
      auto l_camera_manipulator = getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock();

      l_scene_manipulator->readObjectTransform(m_using_id, m_model_matrix);
      l_camera_manipulator->readCameraMatrices(m_camera_view_matrix, m_camera_projection_matrix);

      // make matrices column major order for ImGuizmo
      TransposeMatrix(m_camera_view_matrix);
      TransposeMatrix(m_camera_projection_matrix);
      TransposeMatrix(m_model_matrix);

      //// check if the gizmo "in use" state has changed
      //if (m_is_using_guizmo == true && ImGuizmo::IsUsing() == false)
      //{
      //    m_user_used_gizmo = true;
      //}
      //else
      //{
      //    m_user_used_gizmo = false;
      //}

      setUsingGizmo(ImGuizmo::IsUsing());
    }
    else
    {
      SetIdentity(m_camera_view_matrix);
      SetIdentity(m_camera_projection_matrix);
      SetIdentity(m_model_matrix);

      setUsingGizmo(false);
    }
  }
}

void Editor::gizmoRenderer::pushData()
{
  if(!getEngineController().isPlaying())
  {
    if (getSelectionKeeper().isGameObjectSelected())
    {
      if(m_changed)
      {
        auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();

        // make matrices row major order for engine
        TransposeMatrix(m_model_matrix);

        //TODO: This should not be every frame 
        // send it back to the engine
        l_scene_manipulator->writeObjectTransform(m_using_id, m_model_matrix);

        //Done using gizmo need to tell physics System to send new transform data to Bullet
        l_scene_manipulator->setPhysicsIsDirtyFlag(m_using_id);
        l_scene_manipulator->tellBulletToOverwriteTransform(m_using_id);

        EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
        l_editor_object_manager.getEditorObject(m_using_id)->setNeedsUpdate(true);

        // if user started using the gizmo and has now stopped
        if (m_user_started_gizmo == true && ImGuizmo::IsUsing() == false)
        {
          // get command that was saved previously
          auto command = UndoRedoManager::GetInstance().GetStoredCommand();
          // record the new position/rotation/scale of object
          command->Record();
          // record this completed command
          UndoRedoManager::GetInstance().RecordState(command, &getEngineController());
          m_user_started_gizmo = false; // reset gizmo state
          UndoRedoManager::GetInstance().ClearStoredCommand();


        }

        m_changed = false;
      }
      
    }
  }
}

void Editor::gizmoRenderer::renderInWindow()
{
  if(!getEngineController().isPlaying())
  {
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(m_x, m_y, m_width, m_height);
    //ImGuizmo::DrawGrid(m_camera_view_matrix, m_camera_projection_matrix, m_identity_matrix, 10.0f);
    ImGuizmo::Manipulate(m_camera_view_matrix,
      m_camera_projection_matrix,
      convertOperation(m_operation),
      convertMode(m_mode),
      m_model_matrix,
      NULL,
      /*useSnap ? &snap[0] :*/ NULL,
      /*boundSizing ? bounds :*/ NULL,
      /*boundSizingSnap ? boundsSnap :*/ NULL);
    //ImGuizmo::DrawCube(m_camera_view_matrix, m_camera_projection_matrix, m_model_matrix);
  }
}

void Editor::gizmoRenderer::renderProperties()
{
	float l_matrixTranslation[3], l_matrixRotation[3], l_matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(m_model_matrix, l_matrixTranslation, l_matrixRotation, l_matrixScale);

  //l_changed = false;
  
  if (ImGui::InputFloat3("Translation", l_matrixTranslation, 3))
    m_changed = true;

  if (ImGui::InputFloat3("Rotation", l_matrixRotation, 3))
    m_changed = true;

  if (ImGui::InputFloat3("Scale", l_matrixScale, 3))
    m_changed = true;

  //This doesn't need to be every frame, only if ImGui detects a change
  if(m_changed)
  {
    ImGuizmo::RecomposeMatrixFromComponents(l_matrixTranslation, l_matrixRotation, l_matrixScale, m_model_matrix);
  }
  
}

//======== Helper Functions ============================================================//








