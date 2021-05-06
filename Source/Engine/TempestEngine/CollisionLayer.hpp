/*!***************************************************************************************
\file       CollisionLayer.hpp
\author     Ryan Booth
\date       11/25/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Defines the rigid body of the object
*****************************************************************************************/

#pragma once

#define BIT(x) (1<<(x))

enum collisionLayer
{
  enm_nothing = 0,
  enm_world = BIT(1),
  enm_player = BIT(2),
  enm_enemy = BIT(3),
  enm_trigger = BIT(4),
  enm_layerMax
};
