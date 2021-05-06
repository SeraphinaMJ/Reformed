/*!***************************************************************************************
\file       MoveRandomly.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior just tells the npc to move in some random direction.
*****************************************************************************************/
#include "MoveRandomly.hpp"
#include "BehaviorSystem/BehaviorHelpers.hpp"
#include "GameObjectGatherer.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "WaypointNode.hpp"

#include <random>

const char* maxDistStr = "Max Distance";
const char* minDistStr = "Min Distance";

const SpecialData MoveRandomly::specialData =
{
    {
        {DATA_TYPE::FLOAT, maxDistStr, -1.0f},
        {DATA_TYPE::FLOAT, minDistStr, -1.0f}
    }
};

int chooseRandomIndex(const std::vector<std::shared_ptr<gameObject>>& list)
{
    // if no waypoints then return an invalid index
    if (list.empty())
    {
        return -1;
    }

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, list.size() - 1); // define the range

    return distr(eng);
}

void MoveRandomly::updateFromFile(const rapidjson::Value & val)
{
  this->maxDistance = val[maxDistStr].GetFloat();
  this->minDistance = val[minDistStr].GetFloat();
  
}

void MoveRandomly::Init()
{
    GameObjectFiltering::componentTypeSet pattern;
    pattern.setType(waypointNode::getType());

  // find random position
  auto list = this->getObjectGatherer()->getList(pattern);
  auto newDestination = list[chooseRandomIndex(list)]->getComponent<transform>()->getPosition();
  
  getBlackboard().setValue<vector3>("randomDestination", newDestination);
  
  Leaf::Init();
}

void MoveRandomly::Update(float)
{
  // epsilon is the acceptable distance we can be from a target.
  float epsilon = 3.0f;
  
  auto actor = getActor(); 
  
  auto destination = getBlackboard().getValue<vector3>("randomDestination");
  
  float distance = BH::GetDistance(actor, destination);
  
  if(distance <= epsilon)
  {
    getTask()->SetPhase(BehaviorPhase::DONE);
    getTask()->SetResult(BehaviorResult::SUCCESS);
  }
}
    
    
    
    
    
    
    
    
    
    