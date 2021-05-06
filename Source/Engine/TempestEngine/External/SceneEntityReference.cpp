/*!***************************************************************************************
\file       SceneEntityReference.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "SceneEntityReference.hpp"

sceneEntityReference::sceneEntityReference()
	: m_type{entityType::None}, m_id{0}
{
}

sceneEntityReference::sceneEntityReference(entityType p_type, unsigned int p_id)
	: m_type{p_type}, m_id{p_id}
{
}

bool sceneEntityReference::isValid() const
{
	return m_type != entityType::None;
}

bool sceneEntityReference::isScene() const
{
	return m_type == entityType::Scene;
}

bool sceneEntityReference::isSpace() const
{
	return m_type == entityType::Space;
}

bool sceneEntityReference::isObject() const
{
	return m_type == entityType::Object;
}

bool sceneEntityReference::isComponent() const
{
	return m_type == entityType::Component;
}

sceneEntityReference::entityType sceneEntityReference::getType() const
{
	return m_type;
}

unsigned int sceneEntityReference::getID() const
{
	return m_id;
}
