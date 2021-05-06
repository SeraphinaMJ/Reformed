/*!***************************************************************************************
\file       ComponentRenderer.hpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      
*****************************************************************************************/
#pragma once
#include <vector>
#include <unordered_set>
#include "../Scaffolding/RenderableBase.hpp"
#include <DataTypes.hpp>

class typeRT;


namespace Editor {
  class hierarchySelectionKeeper;
  class styleKeeper;
  class editorWindow;
  class editorState;
  class engineController;
  class EditorObject;

  /*!***************************************************************************************
  \brief Component Rendering class for any component that needs any special (non-default)
         rendering.
  *****************************************************************************************/
  class componentRenderer : public renderableBase
  {
  public:
    /*!***************************************************************************************
    \brief Default Constructor for the componentRenderer.
    \param p_parent_window Pointer to the top editor window.
    *****************************************************************************************/
    componentRenderer(Editor::editorWindow * p_parent_window);

    /*!***************************************************************************************
    \brief Destructor.
    *****************************************************************************************/
    virtual ~componentRenderer() = default;

    /*!***************************************************************************************
    \brief Calls the onRender function for any componentRenderer.
    \param p_type_data The component's typeRT data.
    \param p_editor_object The editor object that owns this data.
    *****************************************************************************************/
    void render(typeRT & p_type_data, objID p_editor_object);

    bool overridesMember(const std::string & p_member);

  protected:
    void onRender() override { /* do nothing because we will redefine it */ }
    /*!***************************************************************************************
    \brief Renders special fields of a component.
    \param p_type_data The component's typeRT data.
    \param p_editor_object The editor object that owns this data.
    *****************************************************************************************/
    virtual void onRender(typeRT & p_type_data, objID p_editor_object_id) = 0;

    /*!***************************************************************************************
    \brief List of special component members that you want to render yourself and not be default
           handled by the editor.
    *****************************************************************************************/
    std::unordered_set<std::string> m_override_members;

  private:
  };
}



