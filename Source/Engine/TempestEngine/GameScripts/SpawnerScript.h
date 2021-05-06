/*!***************************************************************************************
\file       SpawnerScript.h
\author     Aaron Damyen
\date       7/31/18
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "Vector3.hpp"


class spawnerScript : public scriptCPP
{
public:
	spawnerScript() = default;
	spawnerScript(typeRT p_data);
	componentType const type() const override { return spawnerScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<spawnerScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "spawnerScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;
	void spawnEnemy(int p_count);
	void spawnEnemyAt(int p_count, vector3 p_position);
private:


	unsigned int m_spawnCount = 8;
	float m_spawnRate = 2.0f;
	float m_spawnTimer = 0.0f;
    size_t m_maxSpawn = 9;// 9;
	typeRT m_Prefab;
};
