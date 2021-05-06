/*!***************************************************************************************
\file       GizmoRenderer.hpp
\author     Aaron Damyen
\date       2/7/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the gimzo renderer functionality for the editor
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../EditorState.hpp"
#include "../Scaffolding/RenderableBase.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor
{
	//======== Forward Declarations=======================================================//
	class editorWindow;
	class engineController;
	class hierarchySelectionKeeper;

/*!***************************************************************************************
\par class: gizmoRenderer
\brief   This is responsible for rendering a manipulation gizmo in the scene window and the
	transform component breakdown in the inspector window (same data source)
*****************************************************************************************/
class gizmoRenderer : public renderableBase
{
public:
	/*!***************************************************************************************
	\brief  Constructs this renderer
	\param p_parent_window - 
	*****************************************************************************************/
	gizmoRenderer(editorWindow * p_parent_window);

	/*!***************************************************************************************
	\brief  Pulls data of the current selected object from the engine
	*****************************************************************************************/
	void pullData();
	/*!***************************************************************************************
	\brief  Pushes modified data of the current selected object back into the engine
	*****************************************************************************************/
	void pushData();

	/*!***************************************************************************************
	\brief  Renders the gimzo in the window
	*****************************************************************************************/
	void renderInWindow();
	/*!***************************************************************************************
	\brief  Renders the transform components
	*****************************************************************************************/
	void renderProperties();

	/*!***************************************************************************************
	\brief  Retrieves the flag if the gizmo is currently in use by the user
	*****************************************************************************************/
	bool getUsingGizmo() const;
	/*!***************************************************************************************
	\brief  Sets the position and size of the window the gizmo is rendering in (for proper positioning and scaling)
	\param p_x - the x position of the window
	\param p_y - the y position of the window
	\param p_width - the width of the window
	\param p_height - the height of the window
	*****************************************************************************************/
	void setDimensions(float p_x, float p_y, float p_width, float p_height);

	/*!***************************************************************************************
	\brief  Sets the current operation of the gizmo (transform, rotate, scale)
	\param p_operation - the new current operation
	*****************************************************************************************/
	void setOperation(guizmoOperation p_operation);
	/*!***************************************************************************************
	\brief  Retrieves the current operation of the gizmo
	\param guizmoOperation - the current operation
	*****************************************************************************************/
	guizmoOperation getOperation() const;
	/*!***************************************************************************************
	\brief  Sets the current mode of the gizmo (local or global)
	\param p_mode - the new current mode
	*****************************************************************************************/
	void setMode(guizmoMode p_mode);
	/*!***************************************************************************************
	\brief  Retrieves the current mode of the gizmo
	\return guizmoMode - the current mode
	*****************************************************************************************/
	guizmoMode getMode() const;

	/*!***************************************************************************************
	\brief  Retrieves the current operation as a reference for direct updating
	\return guizmoOperation & - Reference to the current operation
	*****************************************************************************************/
	guizmoOperation & getOperationRef();
	/*!***************************************************************************************
	\brief  Retrieves the current mode as a reference for direct updating
	\return guizmoMode & - Reference to the current mode
	*****************************************************************************************/
	guizmoMode & getModeRef();

  bool getIsChanged() const { return m_changed; }

private:
	void onRender() override { /* do nothing because rendering happens elsewhere */ }
	/*!***************************************************************************************
	\brief  Sets the indicator that the gizmo is currently in use by the user
	\param p_state - the new indicator value
	*****************************************************************************************/
	void setUsingGizmo(bool p_state);

	float m_x, m_y, m_width, m_height; //!< Current position and size of the window to render in

	guizmoOperation m_operation; //!< Current operation the gizmo is doing.
	guizmoMode m_mode; //!< Current mode the gizmo is working with

	bool m_is_using_guizmo; //!< Indicator if the user is currently interacting with the gizmo

    //bool m_user_used_gizmo; //!< User used gizmo to move/translate/rotate the object.
    bool m_user_started_gizmo; //!< when user starts to use the gizmo, this will be set

	float m_camera_view_matrix[16] = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 }; //!< current camera view matrix
	float m_camera_projection_matrix[16] = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 }; //!< current camera projection matrix
	float m_model_matrix[16] = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 }; //!< current model matrix
	float m_identity_matrix[16] = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 }; //!< identity matrix for testing

  bool m_changed;
  unsigned int m_using_id;
};
	
}  // namespace Editor
