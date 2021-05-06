/*!***************************************************************************************
\file       RigidBodyRenderer.hpp
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
  class rigidBodyRenderer : public componentRenderer
  {
  public:
    rigidBodyRenderer(editorWindow * p_parent_window);

  protected:
    void onRender(typeRT & p_type_data, objID p_editor_object_id) override;

  private:
    bool m_collisionLayerSelfWorld = false, m_collisionLayerSelfPlayer = false, m_collisionLayerSelfEnemy = false, m_collisionLayerSelfTrigger = false;
    bool m_collisionLayerWithWorld = false, m_collisionLayerWithPlayer = false, m_collisionLayerWithEnemy = false, m_collisionLayerWithTrigger = false;

    void setCollisionLayers(bool & p_collisionLayerToBeTrue);
    void setCollisionLayersWith(bool & p_collisionLayerToBeTrue);

    unsigned int getCollisionLayerSelf();
    unsigned int getCollisionLayerWith();

  };
}
