/*!***************************************************************************************
\file       Health.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Health component for npc or player.
*****************************************************************************************/
#include "Health.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "ScoreTracker.hpp"
#include "Enemy.hpp"
#include "EventBus.hpp"
#include "EnemyDeathEvent.hpp"
#include "InputFramework.hpp"
#include "MinionLeader.hpp"

#include "BehaviorSystem/BehaviorHelpers.hpp"
#include "PikminPlayerControllerScript.hpp"

#include "EnemyLeader.hpp"

Health::Health() : health(-1), m_pain_timer(0.0f), m_max_pain_time(0.25f)
{
}

void Health::Update(float dt)
{
    m_pain_timer += dt;

    auto actor = this->getGameObject().lock();

    auto l_render = actor->getComponent<renderer>();

    // if renderer exists
    if (l_render)
    {
        // if pain timer is greater than max timer
        if (m_pain_timer < m_max_pain_time)
        {
            // object will flash showing pain
            l_render->setMaterialName("pinkglow.mtl");
        }
        // reset back to original material
        else
        {

            // player is always green
            if (BH::IsPlayer(actor))
            {
                l_render->setMaterialName("solidgreen.mtl");
            }
            else if (BH::IsEnemy(actor))
            {
                // get enemy component
                //auto enemy_comp = actor->getComponent<enemy>();

                // check if leader or not
                if (actor->getComponent<enemyLeader>())
                {
                    l_render->setMaterialName("duck.mtl");
                }
                else
                {
                    l_render->setMaterialName("solidred.mtl");
                }

            }
            else if (BH::IsMinion(actor))
            {
                // if leader
                if (actor->getComponent<minionLeader>())
                {
                    // TODO
                    l_render->setMaterialName("duck.mtl");
                }
                // else turn blue
                else
                {
                l_render->setMaterialName("solidblue.mtl");
            }
            }

        }
    }

}

void Health::DecreaseHealth(int amount)
{
    health -= amount;

    // if object died then mark it for deletion
    if (this->IsDead())
    {
        componentHandle<enemy> enemyComp = getGameObject().lock()->getComponent<enemy>();
        if (enemyComp)
        {
            //THROW ENEMY DIED EVENT HERE
            EVENT_BUS.fire(new EnemyDeathEvent());
        }
        this->getGameObject().lock()->markForDeletion();

		componentHandle<pikminPlayerControllerScript> playerController = getGameObject().lock()->getComponent<pikminPlayerControllerScript>();
		if (playerController)
		{
			inputFramework::disableMouseCapture();
		}
    }
    else
    {
        // reset pain timer
        m_pain_timer = 0.0f;
    }

}

void Health::IncreaseHealth(int amount)
{
    health += amount;
}

int Health::GetHealth() const
{
    return health;
}

void Health::SetHealth(int amount)
{
    health = amount;
}

bool Health::IsDead() const
{
    return health <= 0;
}

void Health::Kill()
{
    DecreaseHealth(1000);
}

componentType const Health::type() const
{
    return getType();
}

typeRT Health::toTypeRT() const
{
    // set type and variable name
    typeRT result;
    result.setTypeName(getType().name());
    result.setVariableName(getType().name());

    // create health
    typeRT healthValue("health", (int)this->health);

    // this should set health
    result.insertMember(healthValue);

    return result;
}

void Health::updateFromTypeRT(typeRT & p_type)
{
    if (p_type.members().find("health") != p_type.members().end())
    {
        this->health = p_type.member("health").getInt();
    }
    else
    {
        throw std::runtime_error("ERROR updatefrom type rt in health component was missing health value");
    }
}

std::string Health::getQueryableType()
{
    return std::string(type().name());
}

std::string Health::getQueryableID()
{
    if (!getGameObject().expired())
    {
        return std::to_string(getGameObject().lock()->getID());
    }
    else { return "orphan health component"; }
}

std::string Health::getQueryableProperty(std::string p_name)
{
    return toTypeRT().getString();
}

std::vector<std::string> Health::getQueryablePropertyNames()
{
    return { "health" };
}
