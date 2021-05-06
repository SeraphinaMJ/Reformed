/*!***************************************************************************************
\file       SquadRing.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Each squad may have 1 or more ring formations around it. This class
            represents that formation.
*****************************************************************************************/
#pragma once
#include "Vector3.hpp"
#include "DataTypes.hpp"

#include <unordered_map>

class space;
class Squad;

// a structure representing a circle formation
// around the player.
class SquadRing
{

protected:

      int max_capacity;
      int m_radius;
      
      space* m_space;

      // squad that this squad ring is a part of
      Squad* m_parent_squad;

      // slots for each npc in ring
      std::unordered_map<objID, vector3> m_slots;
      
  
  public:

      void RemoveDeadUnits();
  
      objID AvailableUnit() const;
      
      SquadRing(int maxCapacity_, Squad* pS, int radius_, space* space_)
        : max_capacity(maxCapacity_), m_radius(radius_), m_space(space_), m_parent_squad(pS) {}
      
      bool full() const;
      bool empty() const;
      int spaceLeft() const;

      int unitCount() const;
      
      bool hasUnit(objID) const;
      
      void takeUnits(SquadRing& rhs);
      
      void insert(objID);
      void remove(objID);
      
      void Update(float dt);
      
      void calculatePositions();

      // Get slot position for character
      vector3 getSlotPosition(objID) const;
  
      std::unordered_map<objID, vector3> getSlots() const { return m_slots; }

};



