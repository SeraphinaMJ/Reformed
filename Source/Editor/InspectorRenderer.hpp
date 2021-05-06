/*!***************************************************************************************
\file       InspectorRenderer.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the renderer for the inspector window
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "HierarchySelectionKeeper.hpp"
#include "Scaffolding/RenderableBase.hpp"
#include "Components/ComponentRenderer.hpp"
#include "DataTypes.hpp"
//======== 3rd Party Includes ==========================================================//
#include <string>
#include <unordered_map>
#include <functional>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class typeRT;

namespace Editor
{
  //======== Forward Declarations=======================================================//
	class editorWindow;
	class engineController;
	class styleKeeper;
	class windowBase;
  class EditorObject;

/*!***************************************************************************************
\par class: inspectorRenderer
\brief   This class renders the contents of the inspector window
*****************************************************************************************/
	class inspectorRenderer : public renderableBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the object with appropriate values for bases
		\param p_parent_window - reference to the editorWindow this is contained within
		*****************************************************************************************/
		inspectorRenderer(editorWindow * p_parent_window);
		/*!***************************************************************************************
		\brief  Destroys the renderer, releasing resources as appropriate
		*****************************************************************************************/
		~inspectorRenderer() override;

		/*!***************************************************************************************
		\brief  This just for giving an example of a function format.
		\param exampleIn - dat old boring int
		\return What is this returning?
		*****************************************************************************************/
		void setFilterOption(std::string p_name, bool p_enable);
		/*!***************************************************************************************
		\brief  This just for giving an example of a function format.
		\param exampleIn - dat old boring int
		\return What is this returning?
		*****************************************************************************************/
		void setRemoveHandler(std::function<void(std::string /*component_name*/)> p_function);

		/*!***************************************************************************************
		\brief  Renders the contents of a particular game object to the window
		\param p_type_data - the game object data to be rendered
    \param p_editor_object_id - the editor object id that holds onto the data
		*****************************************************************************************/
		void renderGameObject(typeRT & p_type_data, objID p_editor_object_id);

    /*!***************************************************************************************
    \brief  Renders the contents of a particular scene to the window
    \param p_type_data - the scene data to be rendered
    \param p_editor_object_id - the editor object that holds onto the data
    *****************************************************************************************/
    void renderScene(typeRT & p_type_data, objID p_editor_object_id);

    /*!***************************************************************************************
    \brief  Renders the contents of a particular space to the window
    \param p_type_data - the space data to be rendered
    \param p_editor_object_id - the editor object that holds onto the data
    *****************************************************************************************/
    void renderSpace(typeRT & p_type_data, objID p_editor_object_id);

	private:
		/*!***************************************************************************************
		\brief  Handles the render event to actually render content.  Hidden to prevent use, use
		        renderGameObject(typeRT &) instead.
		*****************************************************************************************/
		void onRender() override { /* do nothing, require call to renderGameObject */ }
		/*!***************************************************************************************
		\brief  Renders type data under the specified component type context.
		\param p_current_component_type - the context the data should be rendered within.
		\param p_type_data - the data that should be rendered
    \param p_editor_object_id - the editor object that holds onto the data
		*****************************************************************************************/
		void render(const std::string & p_current_component_type, typeRT & p_type_data, objID p_editor_object_id);
		/*!***************************************************************************************
		\brief  Determines if a particular filter option has been enabled
		\param p_name - the name of the filter option to check
		\return bool - true if the specified filter option is set, false otherwise
		*****************************************************************************************/
		bool isFilterOptionSet(std::string p_name) const;

		/*!***************************************************************************************
		\brief  Configures styling for a component type and variable
		\param p_current_component_type - the component type to setup styling for
		\param p_variable_name - the variable name to setup styling for
		*****************************************************************************************/
		void beginStyle(const std::string& p_current_component_type, const std::string& p_variable_name);
		/*!***************************************************************************************
		\brief  Restores styling after rendering a component type and variable
		\param p_current_component_type - the component type to restore styling from
		\param p_variable_name - the variable name to restore styling from
		*****************************************************************************************/
		void endStyle(const std::string& p_current_component_type, const std::string& p_variable_name);

		/*!***************************************************************************************
		\brief  Configures disabled styling
		*****************************************************************************************/
		void startDisabled();
		/*!***************************************************************************************
		\brief  Restores styling from disabled
		*****************************************************************************************/
		void endDisabled();
		int m_disable_depth = 0;

		std::unordered_map<std::string /*option name*/, bool /*enabled*/> m_filter_options;
		std::function<void(std::string /*component_name*/)> m_remove_handler;

    std::unordered_map<std::string /* componentRenderer name */, componentRenderer * /* componentRenderer instance */>  m_component_renderers;
    bool m_inside_special_renderer;
    std::string m_current_special_renderer;
	};


} // namespace Editor


