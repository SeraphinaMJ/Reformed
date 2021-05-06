///*!***************************************************************************************
//\file       EnemySpotted.cpp
//\author     Charles Boudousquie
//\date       11/4/19
//\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par        Project: Boomerang
//\brief      Decorator that tells you if an enemy is near the npc.
//*****************************************************************************************/
//#include "EnemySpotted.hpp"
//#include "Allegiance.hpp"
//#include "GameObjectGatherer.hpp"
//#include "Transform.hpp"
//
//const SpecialData EnemySpotted::specialData{ {SpecialData::Entry{ DATA_TYPE::FLOAT, "Spotting Distance", -1.0f}} };
//
//const char spottedDistanceStr[] = "Spotting Distance";
//
//void EnemySpotted::Init()
//{
//    GameObjectFiltering::componentTypeSet node_pattern;
//    node_pattern.setType(Allegiance::getType());
//
//    std::vector<std::shared_ptr<gameObject>> actors = this->getObjectGatherer()->getList(node_pattern);
//
//    auto actor = getActor();
//
//    Faction myFaction = actor->getComponent<Allegiance>()->getFaction();
//
//    GameObjectPtr closestEnemy = nullptr;
//
//    auto myPos = actor->getComponent<transform>()->getPosition();
//
//    for (auto otherActor : actors)
//    {
//        Faction theirFaction = otherActor->getComponent<Allegiance>()->getFaction();
//        
//        // if confirmed to be enemy
//        if (myFaction != theirFaction && theirFaction != Faction::NONE)
//        {
//            // get their position
//            auto enemyPos = otherActor->getComponent<transform>()->getPosition();
//
//            auto distance = (myPos - enemyPos).distance();
//
//            if (distance < spottingDistance)
//            {
//              // enemy spotted, we are done
//              Decorator::Init();
//              return;
//            }
//        }
//    }
//
//    // no enemy found
//    getTask()->SetPhase(BehaviorPhase::DONE);
//    getTask()->SetResult(BehaviorResult::FAILURE);
//}
//
//
//
//
//void EnemySpotted::Update(float dt)
//{
//  // assumed that child has finished by this point
//  getTask()->SetPhase(BehaviorPhase::DONE);
//}
//
//
//
//
//
//
////// returns default render data
////static typeRT getRenderData()
////{
////    // get default data
////    auto defaultRenderData = Decorator::decoratorOnRender();
////
////    // add specialized data
////    typeRT l_spotting_dist(spottedDistanceStr, float(-1));
////    defaultRenderData.member("Node Render Data").insertMember(l_spotting_dist);
////
////    // return combined typeRT data
////    return defaultRenderData;
////}
////static void fillSpecialRenderData(const rapidjson::Value &JSON, typeRT& data)
////{
////    assert(data.getVariableName() == "Node Render Data");
////
////    // if json does not have max time variable then we are screwed
////    if (JSON.HasMember(spottedDistanceStr))
////    {
////        // set typert to have max time value
////        data.member(spottedDistanceStr).setFloat(JSON[spottedDistanceStr].GetFloat());
////    }
////    else
////    {
////        throw std::runtime_error("Missing spotting distance");
////    }
////}
////
////// serialization
////static void serializeSpecialData(typeRT& data, rapidjson::Document& doc)
////{
////    assert(data.getVariableName() == "Node Render Data");
////
////    float spotting_distance = data.member(spottedDistanceStr).getFloat();
////
////    if (doc.HasMember(spottedDistanceStr))
////    {
////        doc[spottedDistanceStr].SetFloat(spotting_distance);
////    }
////    else
////    {
////        doc.AddMember(spottedDistanceStr, spotting_distance, doc.GetAllocator());
////    }
////}
//
//// when reading from file this function will be called
//void EnemySpotted::updateFromFile(const rapidjson::Value & value)
//{
//    this->spottingDistance = value[spottedDistanceStr].GetFloat();
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
