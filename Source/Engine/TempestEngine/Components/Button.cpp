/*!**********************************************************************************************************
\file       Button.cpp
\author     Ryan Booth
\date       10/19/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			This file contains the implementation of the button component.
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "Button.hpp"
//========  1st Party Includes  ===========================================================================//

//========  3rd Party Includes  ===========================================================================//
#include <Logger.hpp>
#include "UISprite.hpp"
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Static Deceleration  ==========================================================================//
//========  Static Class Methods  =========================================================================//

button::button()
{
	initializeData();
}

button::button(typeRT & p_type)
{
	initializeData();

	updateFromTypeRT(p_type);
}

typeRT button::toTypeRT() const
{
	typeRT result;

	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	typeRT selected("selected", m_isSelected);

	//Shouldn't create a pressed button (will auto run when created)
	typeRT pressed("pressed", false);

	typeRT active("active", m_isActive);

	typeRT selectable("selectable", m_isSelectable);

	for(int i = 0; i < DIRECTION_INVALID; ++i)
	{
		buttonDirection l_direction = buttonDirection(i);
		auto l_button = m_neighborMap.at(l_direction);

		if(l_button.first != nullptr)
		{
			std::shared_ptr<gameObject> l_neighbor = l_button.first;
      objID l_neighborID = m_neighborObjectIDs.at(l_direction);

      typeRT l_typeRTNeighbor(convertNeighborToString(l_direction), l_neighborID);

      typeRT l_neighborIDTypeRT("neighborID", l_neighborID);

			typeRT l_typeRTNeighborMirrored("mirrored", l_button.second);

      l_typeRTNeighbor.insertMember(l_neighborIDTypeRT);
			l_typeRTNeighbor.insertMember(l_typeRTNeighborMirrored);

			result.insertMember(l_typeRTNeighbor);
		}
	}

	result.insertMember(selected);
	result.insertMember(pressed);
	result.insertMember(active);
	result.insertMember(selectable);

	return result;
}

void button::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("selected") != l_members.end())
	{
		m_isSelected = (p_type.member("selected").getBool());
	}
	if (l_members.find("active") != l_members.end())
	{
		m_isActive = (p_type.member("active").getBool());
	}
	if (l_members.find("selectable") != l_members.end())
	{
		m_isSelectable = (p_type.member("selectable").getBool());
	}

	for(int i = 0; i < DIRECTION_INVALID; ++i)
	{
		buttonDirection l_direction = buttonDirection(i);
		std::string l_neighborString = convertNeighborToString(l_direction);

		if(l_members.find(l_neighborString) != l_members.end())
		{
      auto l_neighborMembers = p_type.member(l_neighborString).members();

      if(l_neighborMembers.find("neighborID") != l_neighborMembers.end())
      {
        objID l_neighborId = (p_type.member(l_neighborString).member("neighborID").getObjID());
        m_neighborObjectIDs[l_direction] = l_neighborId;

        if(this->getGameObject().lock() == nullptr)
        {
          continue;
        }

        //find game object in the scene with the id
        space * l_currentSpace = this->getGameObject().lock()->getSpace();

        if(l_currentSpace == nullptr)
        {
          continue;
        }

        std::shared_ptr<gameObject> l_neighborObject = l_currentSpace->getGameObjectByID(l_neighborId);

        if (l_neighborObject != nullptr)
        {
          m_neighborMap[l_direction].first = l_neighborObject;

          if (l_neighborMembers.find("mirrored") != l_neighborMembers.end())
          {
            bool l_isMirrored = (p_type.member(l_neighborString).member("mirrored").getBool());
            m_neighborMap[l_direction].second = l_isMirrored;

            if (l_isMirrored)
            {
              l_neighborObject->getComponent<button>()->m_neighborMap[reverseDirection(l_direction)].first = this->getGameObject().lock();
            }
          }
        }
      }
		}
	}
}

void button::setNeighbor(std::shared_ptr<gameObject> p_neighbor, buttonDirection p_direction, bool p_mirrored)
{
	if(p_direction < DIRECTION_INVALID)
	{
		m_neighborMap[p_direction].first = p_neighbor;
    m_neighborObjectIDs[p_direction] = p_neighbor->getID();

		if (p_mirrored)
		{
			p_neighbor->getComponent<button>()->m_neighborMap[reverseDirection(p_direction)].first = this->getGameObject().lock();
      p_neighbor->getComponent<button>()->m_neighborObjectIDs[reverseDirection(p_direction)] = this->getGameObject().lock()->getID();
			m_neighborMap[p_direction].second = p_mirrored;
		}
	}
}

objID button::removeNeighbor(buttonDirection p_direction)
{
  objID l_removed_neighbor_id = 0;

  if(m_neighborMap[p_direction].second == true)
  {
    auto l_neighbor = m_neighborMap[reverseDirection(p_direction)].first;
    if(l_neighbor != nullptr)
    {
      l_removed_neighbor_id = l_neighbor->getID();

      l_neighbor->getComponent<button>()->m_neighborMap[reverseDirection(p_direction)].first = nullptr;
      l_neighbor->getComponent<button>()->m_neighborMap[reverseDirection(p_direction)].second = false;
    }
  }

  m_neighborMap[p_direction].first = nullptr;
  m_neighborMap[p_direction].second = false;
  m_neighborObjectIDs[p_direction] = 0;

  return l_removed_neighbor_id;
}

void button::setSelected(bool p_selected)
{
	if(p_selected == false)
	{
		m_isPressed = false;
	}

  if(m_isActive)
  {
    m_isSelected = p_selected;
  }

	// update sprite highlight
	auto l_sprite = getGameObject().lock()->getComponent<uiSprite>();
	if(l_sprite)
	{
		if (m_isSelected) l_sprite->enableHighlight();
		else l_sprite->disableHighlight();
	}
}

void button::setPressed(bool p_pressed)
{
	if(p_pressed)
	{
    logger l_logger("UI");
    l_logger.debug(this->getGameObject().lock()->getName() + " was pressed.");
	}

  if(m_isActive)
  {
    m_isPressed = p_pressed;
  }
}

void button::setSelectable(bool p_selectable)
{
  m_isSelectable = p_selectable;
}

std::shared_ptr<gameObject> button::getNeighbor(buttonDirection p_direction)
{
	if(p_direction < DIRECTION_INVALID)
	{
		//Neighbor hasn't been set up yet but object now exists
		if (m_neighborMap[p_direction].first == nullptr && m_neighborObjectIDs[p_direction] != 0)
		{
			space * l_currentSpace = this->getGameObject().lock()->getSpace();
			std::shared_ptr<gameObject> l_neighborObject = l_currentSpace->getGameObjectByID(m_neighborObjectIDs[p_direction]);

			if (l_neighborObject != nullptr)
			{
				m_neighborMap[p_direction].first = l_neighborObject;
			}
		}

		return m_neighborMap[p_direction].first;
	}
	
	return nullptr;
}

void button::setActive(bool p_active)
{
	m_isActive = p_active;

	if(m_isActive == false)
	{
		m_isSelected = false;
		m_isPressed = false;
	}
}

button::buttonDirection button::reverseDirection(buttonDirection p_direction) const
{
	if (p_direction == DIRECTION_UP)
		return DIRECTION_DOWN;
	else if (p_direction == DIRECTION_DOWN)
		return DIRECTION_UP;
	else if (p_direction == DIRECTION_RIGHT)
		return DIRECTION_LEFT;
	else if (p_direction == DIRECTION_LEFT)
		return DIRECTION_RIGHT;

	return DIRECTION_INVALID;
}

void button::initializeData()
{
	//initialize maps
	for (int i = 0; i < DIRECTION_INVALID; ++i)
	{
		buttonDirection l_direction = buttonDirection(i);

		m_neighborMap[l_direction].first = nullptr;
		m_neighborMap[l_direction].second = false;
		m_neighborObjectIDs[l_direction] = 0;
	}
}

std::string button::convertNeighborToString(buttonDirection p_direction) const
{
	if (p_direction == DIRECTION_UP)
		return std::string("neighborUp");
	else if (p_direction == DIRECTION_DOWN)
		return std::string("neighborDown");
	else if (p_direction == DIRECTION_RIGHT)
		return std::string("neighborRight");
	else if (p_direction == DIRECTION_LEFT)
		return std::string("neighborLeft");


	return std::string("");
}
