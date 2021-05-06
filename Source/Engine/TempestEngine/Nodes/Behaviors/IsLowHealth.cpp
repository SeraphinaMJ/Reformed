/*!***************************************************************************************
\file       IsLowHealth.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if npc has low health.
*****************************************************************************************/
#include "IsLowHealth.hpp"
#include "Health.hpp"

const char minHealthStr[] = "minimumHealth";

const SpecialData IsLowHealth::specialData =
{
    {SpecialData::Entry{DATA_TYPE::INT, "minimumHealth", -1}}
};

void IsLowHealth::Init()
{
    InitDebug();

    auto actor = getActor();

    auto health = actor->getComponent<Health>()->GetHealth();

    // if health is low then give to child
    if (health < minimumHealth)
    {
        Decorator::Init();
    }
    // otherwise we are not low on health so return false
    else
    {
        getTask()->SetPhase(BehaviorPhase::DONE);
    }

}

void IsLowHealth::Update(float dt)
{
    UpdateDebug();
    // at this point child has given task back to us.
    // Child has already set "result" in task so just exit normally
    getTask()->SetPhase(BehaviorPhase::DONE);
}

//typeRT IsLowHealth::getRenderData()
//{
//    // get default data
//    auto defaultRenderData = Decorator::decoratorOnRender();
//
//    // add specialized data
//    typeRT l_minimum_health(minHealthStr, float(-1));
//    defaultRenderData.member("Node Render Data").insertMember(l_minimum_health);
//
//    // return combined typeRT data
//    return defaultRenderData;
//}
//
//void IsLowHealth::fillSpecialRenderData(const rapidjson::Value &JSON, typeRT& data)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    // if json does not have minimumHealth variable then we are screwed
//    if (JSON.HasMember(minHealthStr))
//    {
//        // set typert to have minimumHealth value
//        data.member(minHealthStr).setFloat(JSON[minHealthStr].GetFloat());
//    }
//    else
//    {
//        throw std::runtime_error("Missing minimumHealth");
//    }
//}
//
//// serialization
//void IsLowHealth::serializeSpecialData(typeRT& data, rapidjson::Document& doc)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    float minimum_health = data.member(minHealthStr).getFloat();
//
//    if (doc.HasMember(minHealthStr))
//    {
//        doc[minHealthStr].SetFloat(minimum_health);
//    }
//    else
//    {
//        doc.AddMember(minHealthStr, minimum_health, doc.GetAllocator());
//    }
//}

// when reading from file this function will be called
// to set minimum_health
void IsLowHealth::updateFromFile(const rapidjson::Value &value)
{
    this->minimumHealth = value[minHealthStr].GetFloat();
}