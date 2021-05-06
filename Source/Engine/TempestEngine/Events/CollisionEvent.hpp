/*!***************************************************************************************
\file       CollisionEvent.hpp
\author     Ryan Booth
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
// #include "../Events/EventSystem.hpp"
#include <Vector3.hpp>
#include <DataTypes.hpp>
#include "EventBase.hpp"


class collisionEvent : public EventSystem3::EventBase
{
public:
  collisionEvent(std::string p_message, objID p_object1, objID p_object2, const int & p_numCollisions, std::vector<vector3> & p_collisionPoints, std::vector<vector3> & p_collisionNormals)
                : m_message(p_message), object1(p_object1), object2(p_object2), numCollisions(p_numCollisions), collisionPoints(p_collisionPoints), collisionNormals(p_collisionNormals) {}
  std::string m_message;
  //data here
  objID object1;
  objID object2;
  int numCollisions;
  std::vector<vector3> collisionPoints;
  std::vector<vector3> collisionNormals;
};
