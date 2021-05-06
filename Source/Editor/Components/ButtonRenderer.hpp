/*!***************************************************************************************
\file       ButtonRenderer.hpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#pragma once
#include "ComponentRenderer.hpp"

namespace Editor
{
  class buttonRenderer : public componentRenderer
  {
  public:
    buttonRenderer(editorWindow * p_parent_window);

  protected:
    void onRender(typeRT & p_type_data, objID p_editor_object_id) override;
    
  private:
    int m_buttonText = -1;
    bool m_rightButton = false, m_leftButton = false, m_upButton = false, m_downButton = false;
    bool m_rightButtonRemove = false, m_leftButtonRemove = false, m_upButtonRemove = false, m_downButtonRemove = false;

    void setButtons(bool & p_buttonToBeTrue);
  };
}
