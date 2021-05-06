/*!***************************************************************************************
\file       SampleRenderer.cpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#include "SampleRenderer.hpp"
#include "Reflection.hpp"	

Editor::sampleRenderer::sampleRenderer(editorWindow* p_parent_window)
  : componentRenderer(p_parent_window)
{
  //To set a given variable to read only so the default inspectorRenderer doesn't allow for changes to this parameter
  //auto & l_styles = getStyleKeeper();
  //l_styles.setStyle("*", "parent", "readonly", 1);

  //This tells the editor to not render a member variable called mass inside the component you are overriding, you are now 
  //responsible for displaying this variable. 
  //m_override_members.insert("mass");
}

//If you don't use type data then leave this commented out
void Editor::sampleRenderer::onRender(typeRT& /* p_type_data */, objID /* p_editor_object_id */)
{
  //auto l_engine_controller = getEngineController();
  //auto l_selection = getSelectionKeeper();

  //if(doSomething)
  //{

  //}
}
