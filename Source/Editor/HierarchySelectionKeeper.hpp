/*!***************************************************************************************
\file       HierarchySelectionKeeper.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for a selection manager for hierarchy entries
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <memory>
#include <string>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class scene;
class space;
class gameObject;

namespace Editor
{
  struct EditorLogger;
  class engineController;
	/*!***************************************************************************************
	\par class: hierarchySelectionKeeper
	\brief   This class manages the selected items in the hierarchy
	*****************************************************************************************/
	class hierarchySelectionKeeper
	{
	public:
		enum class SelectionType { None = 0, Scene, Space, GameObject };

		/*!***************************************************************************************
		\brief  Constructs the selection manager
		*****************************************************************************************/
		hierarchySelectionKeeper(EditorLogger & p_logger);

		/*!***************************************************************************************
		\brief  Retrieves the type of the current selection
		\return SelectionType - enumeration of the type of the current selection
		*****************************************************************************************/
		SelectionType getSelectionType() const { return m_selection_type; }
		/*!***************************************************************************************
		\brief  Retrieves the id number of the current selection
		\return unsigned int - ID number of the current selection
		*****************************************************************************************/
		unsigned int getSelectionId() const { return m_selection_id; }

    const std::string & getSelectionName() const { return m_selection_name; }
		/*!***************************************************************************************
		\brief  Clears the currrent selection
		*****************************************************************************************/
		void clearSelection();

		/*!***************************************************************************************
		\brief  Indicates if no objects are currently selected
		\return bool - true if no objects are selected, false otherwise
		*****************************************************************************************/
		bool isNoneSelected() const;
		/*!***************************************************************************************
		\brief  Indicates if a scene type object is currently selected
		\return bool - true if a scene type object is selected, false otherwise
		*****************************************************************************************/
		bool isSceneSelected() const;
		/*!***************************************************************************************
		\brief  Indicates if a space type object is currently selected
		\return bool - true if a space type object is selected, false otherwise
		*****************************************************************************************/
		bool isSpaceSelected() const;
		/*!***************************************************************************************
		\brief  Indicates if a game object type is currently selected
		\return bool - true if a game object type is selected, false otherwise
		*****************************************************************************************/
		bool isGameObjectSelected() const;

		/*!***************************************************************************************
		\brief  Determines if a specific scene is currently selected
		\param p_scene - the scene to check for selection
		\return bool - true if the parameter scene is currently selected, false otherwise
		*****************************************************************************************/
		bool isSelected(std::shared_ptr<scene> p_scene) const;
		/*!***************************************************************************************
		\brief  Determines if a specific space is currently selected
		\param p_space - the space to check for selection
		\return bool - true if the parameter space is currently selected, false otherwise
		*****************************************************************************************/
		bool isSelected(std::shared_ptr<space> p_space) const;
		/*!***************************************************************************************
		\brief  Determines if a specific game object is currently selected
		\param p_object - the game objec tto check for selection
		\return bool - true if the parameter game object is currently selected, false otherwise
		*****************************************************************************************/
		bool isSelected(std::shared_ptr<gameObject> p_object) const;

		/*!***************************************************************************************
		\brief  Specifies a scene as currently selected
		\param p_scene - the scene to be selected
		*****************************************************************************************/
		void setSelected(std::shared_ptr<scene> p_scene);
		/*!***************************************************************************************
		\brief  Specifies a space as currently selected
		\param p_space - the space to be selected
		*****************************************************************************************/
		void setSelected(std::shared_ptr<space> p_space);
		/*!***************************************************************************************
		\brief  Specifies a game object as currently selected
		\param p_object - the game object to be selected
		*****************************************************************************************/
		void setSelected(std::shared_ptr<gameObject> p_object);

    void setSelected(unsigned int p_object_id, engineController & p_engine_controller);

    void setSelectedNextFrame(SelectionType p_selection_type, unsigned int p_selection_id, const std::string & p_selection_name);

    void updateSelected();

    bool getNeedsUpdateNextFrame() const { return m_next_frame_needs_update; }

	private:
		SelectionType m_selection_type = SelectionType::None;
		unsigned int m_selection_id = 0;
    std::string m_selection_name;

    EditorLogger & m_logger;

    SelectionType m_selection_type_next_frame = SelectionType::None;
    unsigned int m_selection_id_next_frame = 0;
    std::string m_selection_name_next_frame;
    bool m_next_frame_needs_update = false;
	};

} // namespace Editor

