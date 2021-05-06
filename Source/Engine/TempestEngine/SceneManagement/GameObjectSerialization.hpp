/*!***************************************************************************************
\file       GameObjectSerialization.hpp
\author     Aaron Damyen
\date       10/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the serialization functions for GameObject.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "GameObject.hpp"
//======== 3rd Party Includes ==========================================================//
#include <memory>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//


/*!***************************************************************************************
\brief  Converts the parameter game object into a type representation
\param p_go - the game object to read
\return typeRT - the typeRT structure representing the game object.
*****************************************************************************************/
typeRT toTypeRT(const std::shared_ptr<gameObject> & p_go);

/*!***************************************************************************************
\brief  Synchronizes the given game object to be exactly as prescribed by the type representation.
         This can remove existing components and add needed components.
\param p_go - the game object to update
\param p_type - the type representation for the game object
*****************************************************************************************/
void synchronizeGameObject(std::shared_ptr<gameObject> & p_go, typeRT & p_type);


// namespace <namespace name>

