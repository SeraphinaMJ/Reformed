/*!***************************************************************************************
\file       SampleRenderer.hpp
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
  class sampleRenderer : public componentRenderer
  {
  public:
    sampleRenderer(editorWindow * p_parent_window);

  protected:
    void onRender(typeRT & p_type_data, objID p_editor_object_id) override;

  private:

  };
}
