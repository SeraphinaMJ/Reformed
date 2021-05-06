/*!***************************************************************************************
\file       EnemyLeader.cpp
\author     Charles Boudousquie
\date       12/7/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leader component for enemy squads.
*****************************************************************************************/
#include "EnemyLeader.hpp"

typeRT enemyLeader::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void enemyLeader::updateFromTypeRT(typeRT& p_type)
{
}