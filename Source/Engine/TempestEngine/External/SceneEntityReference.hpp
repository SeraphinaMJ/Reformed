/*!***************************************************************************************
\file       SceneEntityReference.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

class sceneEntityReference final
{
public:
	enum class entityType { Scene = 1, Space = 2, Object = 3, Component = 4, None = 0 };

	sceneEntityReference();
	sceneEntityReference(entityType p_type, unsigned int p_id);

	bool isValid() const;
	bool isScene() const;
	bool isSpace() const;
	bool isObject() const;
	bool isComponent() const;
	entityType getType() const;
	unsigned int getID() const;

private:
	entityType m_type;
	unsigned int m_id;
};

