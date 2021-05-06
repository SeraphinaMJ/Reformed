/*!**********************************************************************************************************
\file       UISystem.cpp
\author     Ryan Booth
\date       10/29/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "UISystem.hpp"
//========  1st Party Includes  ===========================================================================//
#include "../Components/InputComponent.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../Components/Renderer.hpp"
#include "../Events/ButtonPressedEvent.hpp"
#include "EventBus.hpp"
//========  3rd Party Includes  ===========================================================================//
#include <Logger.hpp>
#include <Vector2.hpp>
#include <GamepadControlDefines.hpp>
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Static Deceleration  ==========================================================================//
//========  Static Class Methods  =========================================================================//

#define COOLDOWN 0.5f;

void uiSystem::onInitialize()
{
  m_init = false;
}

void uiSystem::onShutdown()
{
  m_init = false;
}

void uiSystem::onUpdate()
{
	//Get the UISytemSpace from the current scene
  systemManagerInterface * l_system = getSystemManager();
  sceneSystem * l_sceneSystem = l_system->getSystem<sceneSystem>();
  auto l_scene = l_sceneSystem->getCurrentScene().lock();
  auto l_space = l_scene->getSpace("UISystemSpace");

  logger l_logger("UISystem");

  if(l_space == nullptr)
  {
    l_logger.debug("UISystemSpace does not exist!");
    return;
  }

  //Get the uiGameObject
  std::shared_ptr<gameObject> l_uiGameObject = l_space->getGameObjectByName(m_uiGameObjectName);

  // get the input component
  componentHandle<inputComponent> l_input = l_uiGameObject->getComponent<inputComponent>();
  componentHandle<transform> l_transform = l_uiGameObject->getComponent<transform>();

  //Update list of buttons
  updateListOfButtons();

  if (m_currentSelectedButton == 0)
  {
    cleanListOfButtons();
    resetCurrentButton();
  }

  //Local copy of dt
  float l_dt = getDt();
  //Buttons aren't allowed to be pressed faster than this cooldown.
  //If cooldown is still happening update and return.
  if(m_coolDown > 0)
  {
    m_coolDown -= l_dt;
    return;
  }

  // check states and update current button
  if (l_input->getButtonState("up"))
  {
    //Set cooldown to 0.25 seconds.
    m_coolDown += COOLDOWN;
    //Get the currently selected button
    std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);

    if(l_currentButton != nullptr)
    {
      //Get the up neighbor of the currently selected button
      std::shared_ptr<gameObject> l_neighbor = l_currentButton->getComponent<button>()->getNeighbor(button::buttonDirection::DIRECTION_UP);
      //If the neighbor doesn't exist
      if (l_neighbor == nullptr)
      {
        //wrap around to the lowest button beneath us
        if (m_wrapping)
        {
          //Loop through the neighbors as long as we can down from the currently selected button
          componentHandle<button> l_wrapped = findButton(m_currentSelectedButton)->getComponent<button>();
          while (true)
          {
            std::shared_ptr<gameObject> l_belowMe = l_wrapped->getNeighbor(button::buttonDirection::DIRECTION_DOWN);

            //If the neighbor exists
            if (l_belowMe != nullptr)
            {
              //Reset the new current button
              l_wrapped = l_belowMe->getComponent<button>();
            }
            //Reached the end of the line
            else
            {
              break;
            }
          }

          l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
          l_logger.debug("Current button: " + l_wrapped->getGameObject().lock()->getName());
          //Set the current button to the final wrapped downwards button
          setCurrentSelectedButton(l_wrapped);
        }
        else
        {
          l_logger.debug(findButton(m_currentSelectedButton)->getName() + " does not have a up neighbor.");
          return;
        }
      }
      //The neighbor exists
      else
      {
        l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
        l_logger.debug("Current button: " + l_neighbor->getName());
        //Set the current button to the up neighbor of the old
        setCurrentSelectedButton(l_neighbor->getComponent<button>());
      }
    }
    else
    {
      cleanListOfButtons();
      resetCurrentButton();
    }
  }
  if (l_input->getButtonState("down"))
  {
    //Set cooldown to 0.25 seconds.
    m_coolDown += COOLDOWN;
    //Get the currently selected button
    std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);

    if (l_currentButton != nullptr)
    {
      //Get the down neighbor of the currently selected button
      std::shared_ptr<gameObject> l_neighbor = l_currentButton->getComponent<button>()->getNeighbor(button::buttonDirection::DIRECTION_DOWN);
      //If the neighbor doesn't exist
      if (l_neighbor == nullptr)
      {
        //wrap around to the highest button above us
        if (m_wrapping)
        {
          //Loop through the neighbors as long as we can up from the currently selected button
          componentHandle<button> l_wrapped = findButton(m_currentSelectedButton)->getComponent<button>();
          while (true)
          {
            std::shared_ptr<gameObject> l_aboveMe = l_wrapped->getNeighbor(button::buttonDirection::DIRECTION_UP);

            //If the neighbor exists
            if (l_aboveMe != nullptr)
            {
              //Reset the new current button
              l_wrapped = l_aboveMe->getComponent<button>();
            }
            //Reached the end of the line
            else
            {
              break;
            }
          }

          l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
          l_logger.debug("Current button: " + l_wrapped->getGameObject().lock()->getName());
          //Set the current button to the final wrapped downwards button
          setCurrentSelectedButton(l_wrapped);
        }
        else
        {
          l_logger.debug(findButton(m_currentSelectedButton)->getName() + " does not have a down neighbor.");
          return;
        }
      }
      //The neighbor exists
      else
      {
        l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
        l_logger.debug("Current button: " + l_neighbor->getName());
        //Set the current button to the down neighbor of the old
        setCurrentSelectedButton(l_neighbor->getComponent<button>());
      }
    }
    else
    {
      cleanListOfButtons();
      resetCurrentButton();
    }
  }
  if (l_input->getButtonState("right"))
  {
    //Set cooldown to 0.25 seconds.
    m_coolDown += COOLDOWN;
    //Get the currently selected button
    std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);

    if (l_currentButton != nullptr)
    {
      //Get the right neighbor of the currently selected button
      std::shared_ptr<gameObject> l_neighbor = l_currentButton->getComponent<button>()->getNeighbor(button::buttonDirection::DIRECTION_RIGHT);
      //If the neighbor doesn't exist
      if (l_neighbor == nullptr)
      {
        //wrap around to the left most button than us
        if (m_wrapping)
        {
          //Loop through the neighbors as long as we can left from the currently selected button
          componentHandle<button> l_wrapped = findButton(m_currentSelectedButton)->getComponent<button>();
          while (true)
          {
            std::shared_ptr<gameObject> l_leftMe = l_wrapped->getNeighbor(button::buttonDirection::DIRECTION_LEFT);

            //If the neighbor exists
            if (l_leftMe != nullptr)
            {
              //Reset the new current button
              l_wrapped = l_leftMe->getComponent<button>();
            }
            //Reached the end of the line
            else
            {
              break;
            }
          }

          l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
          l_logger.debug("Current button: " + l_wrapped->getGameObject().lock()->getName());
          //Set the current button to the final wrapped leftwards button
          setCurrentSelectedButton(l_wrapped);
        }
        else
        {
          l_logger.debug(findButton(m_currentSelectedButton)->getName() + " does not have a right neighbor.");
          return;
        }
      }
      //The neighbor exists
      else
      {
        l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
        l_logger.debug("Current button: " + l_neighbor->getName());
        //Set the current button to the right neighbor of the old
        setCurrentSelectedButton(l_neighbor->getComponent<button>());
      }
    }
    else
    {
      cleanListOfButtons();
      resetCurrentButton();
    }
  }
  if (l_input->getButtonState("left"))
  {
    //Set cooldown to 0.25 seconds.
    m_coolDown += COOLDOWN;
    //Get the currently selected button
    std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);

    if (l_currentButton != nullptr)
    {
      //Get the left neighbor of the currently selected button
      std::shared_ptr<gameObject> l_neighbor = l_currentButton->getComponent<button>()->getNeighbor(button::buttonDirection::DIRECTION_LEFT);
      //If the neighbor doesn't exist
      if (l_neighbor == nullptr)
      {
        //wrap around to the left most button than us
        if (m_wrapping)
        {
          //Loop through the neighbors as long as we can right from the currently selected button
          componentHandle<button> l_wrapped = findButton(m_currentSelectedButton)->getComponent<button>();
          while (true)
          {
            std::shared_ptr<gameObject> l_rightMe = l_wrapped->getNeighbor(button::buttonDirection::DIRECTION_RIGHT);

            //If the neighbor exists
            if (l_rightMe != nullptr)
            {
              //Reset the new current button
              l_wrapped = l_rightMe->getComponent<button>();
            }
            //Reached the end of the line
            else
            {
              break;
            }
          }

          l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
          l_logger.debug("Current button: " + l_wrapped->getGameObject().lock()->getName());
          //Set the current button to the final wrapped rightwards button
          setCurrentSelectedButton(l_wrapped);
        }
        else
        {
          l_logger.debug(findButton(m_currentSelectedButton)->getName() + " does not have a left neighbor.");
          return;
        }
      }
      //The neighbor exists
      else
      {
        l_logger.debug("Previous button: " + findButton(m_currentSelectedButton)->getName());
        l_logger.debug("Current button: " + l_neighbor->getName());
        //Set the current button to the left neighbor of the old
        setCurrentSelectedButton(l_neighbor->getComponent<button>());
      }
    }
    else
    {
      cleanListOfButtons();
      resetCurrentButton();
    }
  }

  if(l_input->getButtonState("enter"))
  {
    //Increment cooldown
    m_coolDown += COOLDOWN;
    //Set the current button pressed to true
    std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);
    if(l_currentButton != nullptr)
    {
      EVENT_BUS.fire(new ButtonPressedEvent("Enter was Pressed", l_currentButton->getID()));
      l_currentButton->getComponent<button>()->setPressed(true);
    }
  }

  if(l_input->getButtonState("lmb"))
  {
    //Increment cooldown
    m_coolDown += COOLDOWN;
    //Get the current mouse position
    std::pair<double, double> l_mousePosition = l_input->getCursorState("mouse");
    //Get the current button position
    const vector3 l_buttonPosition = l_transform->getPosition();
    const vector3 l_buttonScale = l_transform->getScale();
    //Test if the mouse is inside the button rectangle
    if(isInsideRectangle(l_buttonPosition, l_buttonScale, l_mousePosition))
    {
      //Set pressed to true
      std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);
      if (l_currentButton != nullptr)
      {
        l_currentButton->getComponent<button>()->setPressed(true);
      }
    }
  }

}

void uiSystem::onLevelLoad(const levelLoadEvent & /*p_event*/)
{
  if(!m_init)
  {
    //Get the current scene
    systemManagerInterface * l_system = getSystemManager();
    sceneSystem * l_sceneSystem = l_system->getSystem<sceneSystem>();
    auto l_scene = l_sceneSystem->getCurrentScene().lock();
    auto l_space = l_scene->getSpace("UISystemSpace");

    //If it exists, delete it.
    if(l_space != nullptr)
    {
      l_scene->destroySpace(l_space);
    }
    //Create the UISytemSpace that will hold the uiSystemObject
    l_space = l_scene->createSpace("UISystemSpace");

    //We don't want to serialize this space
    l_space->setShouldSerialize(false);

    //Persist through scene changes
    l_space->dontDeleteOnLoad();
    //Create the uiGameObject that will handle input
    std::shared_ptr<gameObject> l_uiFramework = l_space->instantiateGameObject();
    //Create the input component
    componentHandle<inputComponent> l_input_component = l_uiFramework->addComponent<inputComponent>();
    //Mouse & Keyboard controls
    l_input_component->addButtonMapping("up", Key::Up);
    l_input_component->addButtonMapping("down", Key::Down);
    l_input_component->addButtonMapping("right", Key::Right);
    l_input_component->addButtonMapping("left", Key::Left);
    l_input_component->addButtonMapping("enter", Key::Enter);
    l_input_component->addButtonMapping("lmb", MouseButton::Left);
    l_input_component->addCursorMapping("mouse", Cursor::Default_Mouse);
    //TODO: Gamepad controls
    //l_input_component->addGamepadButtonMapping("up", gpb_dpadUp, 0);
    //l_input_component->addGamepadButtonMapping("down", gpb_dpadDown, 0);
    //l_input_component->addGamepadButtonMapping("right", gpb_dpadRight, 0);
    //l_input_component->addGamepadButtonMapping("left", gpb_dpadLeft, 0);
    //l_input_component->addGamepadButtonMapping("enter", gpb_A, 0);

    //Add the component to the uiGameObject

    l_uiFramework->setName("uiSystemGameObject");

    //Initialize member variables
    m_uiGameObjectName = "uiSystemGameObject";
    m_init = true;
    //m_coolDown = 0.0f;
  }

  cleanListOfButtons();
  updateListOfButtons();
  resetCurrentButton();
}

void uiSystem::onLevelUnload(const levelUnloadEvent & /*p_event*/)
{
  //cleanListOfButtons();
  m_init = false;
}

void uiSystem::addButton(componentHandle<button> p_button)
{
  //Get the button's parent game object id
  std::shared_ptr<gameObject> l_buttonObject = p_button->getGameObject().lock();
  objID l_newButtonID = 0;

  if(l_buttonObject != nullptr)
  {
    l_newButtonID = l_buttonObject->getID();

    //If this is the first button added then set it to the currently selected button
    if (m_listOfButtons.size() == 0)
    {
      //TODO: When this code is able to be merged in (MovingBetweenScenes) has the code
      /*if(p_button->isSelectable())
      {

      }*/
      m_currentSelectedButton = l_newButtonID;
    }
    //Add it to the list
    m_listOfButtons.push_back(l_newButtonID);
  }
}

void uiSystem::setWrapping(bool p_wrapping)
{
	m_wrapping = p_wrapping;
}

void uiSystem::setCurrentSelectedButton(componentHandle<button> p_button)
{
  if(p_button)
  {
    //Make sure the button is selectable
    if(p_button->isSelectable())
    {
      //Get parent gameObject
      std::shared_ptr<gameObject> l_buttonObject = p_button->getGameObject().lock();

      if(l_buttonObject != nullptr)
      {
        objID l_buttonID = p_button->getGameObject().lock()->getID();
        //Get current selected button
        std::shared_ptr<gameObject> l_currentButton = findButton(m_currentSelectedButton);

        if(l_currentButton != nullptr)
        {
          //Set the current button selected to false
          l_currentButton->getComponent<button>()->setSelected(false);
          //Current id is new button id
          m_currentSelectedButton = l_buttonID;
          //Set it to being selected
          p_button->setSelected(true);
        }
      }
    }
  }
}

void uiSystem::setCurrentSelectedButton(const objID p_buttonID)
{
  auto l_gameObject = findButton(p_buttonID);

  if (l_gameObject != nullptr)
  {
    auto l_button = l_gameObject->getComponent<button>();

    if (l_button)
    {
      if (l_button->isSelectable())
      {
        if (l_button->isActive())
        {
          l_button->setSelected(true);
        }
        else
        {
          return;
        }
      }
      else
      {
        return;
      }
    }
  }

  l_gameObject = findButton(m_currentSelectedButton);

  if(l_gameObject != nullptr)
  {
    auto l_button = l_gameObject->getComponent<button>();

    if(l_button)
    {
      l_button->setSelected(false);
    }
  }

	m_currentSelectedButton = p_buttonID;
}

void uiSystem::updateListOfButtons()
{
  //Get the system manager
  systemManagerInterface * l_system = getSystemManager();
  //Get all game objects with button components
  GameObjectFiltering::componentTypeSet l_set;
  l_set.setType(button::getType());
  auto l_list = l_system->getGameObjectGatherer()->getList(l_set);
  //For each button object
  for(auto l_gameObject : l_list)
  {
    //Get its id
    objID l_gameObjectID = l_gameObject->getID();
    //This button hasn't been added yet.
    if(std::find(m_listOfButtons.begin(), m_listOfButtons.end(), l_gameObjectID) == m_listOfButtons.end())
    {
      //If the button is the first button to be added
      if (m_listOfButtons.size() == 0)
      {
        if(l_gameObject->getComponent<button>()->isSelectable())
        {
          m_currentSelectedButton = l_gameObjectID;
        }
      }
      //Add it to the list
      m_listOfButtons.push_back(l_gameObjectID);
    }
    //Set all buttons press state to false
    l_gameObject->getComponent<button>()->setPressed(false);
  }
}

void uiSystem::disableAllButtons()
{
    //Get the system manager
    systemManagerInterface * l_system = getSystemManager();
    //Get all game objects with button components
    GameObjectFiltering::componentTypeSet l_set;
    l_set.setType(button::getType());
    auto l_list = l_system->getGameObjectGatherer()->getList(l_set);
    //For each button object
    for (auto l_gameObject : l_list)
    {
        l_gameObject->getComponent<button>()->setActive(false);
    }
}

void uiSystem::enableAllButtons()
{
    //Get the system manager
    systemManagerInterface * l_system = getSystemManager();
    //Get all game objects with button components
    GameObjectFiltering::componentTypeSet l_set;
    l_set.setType(button::getType());
    auto l_list = l_system->getGameObjectGatherer()->getList(l_set);
    //For each button object
    for (auto l_gameObject : l_list)
    {
        l_gameObject->getComponent<button>()->setActive(true);
    }
}

bool uiSystem::isInsideRectangle(const vector3 & p_center, const vector3 & p_scale, const std::pair<double, double> & p_point)
{
	//No idea if this works

	vector2 l_AB;//				B																	A
	l_AB.x = static_cast<float>((p_center.x + (p_scale.x / 2.0f)) - (p_center.x + (p_scale.x / 2.0f)));
	l_AB.y = static_cast<float>((p_center.y + (p_scale.y / 2.0f)) - (p_center.y - (p_scale.y / 2.0f)));

	vector2 l_BC;//				C																	B
	l_BC.x = static_cast<float>((p_center.x - (p_scale.x / 2.0f)) - (p_center.x + (p_scale.x / 2.0f)));
	l_BC.y = static_cast<float>((p_center.y + (p_scale.y / 2.0f)) - (p_center.y + (p_scale.y / 2.0f)));

	vector2 l_AM;
	l_AM.x = static_cast<float>(p_point.first - (p_center.x + (p_scale.x / 2.0f)));
	l_AM.y = static_cast<float>(p_point.second - (p_center.y - (p_scale.y / 2.0f)));

	vector2 l_BM;
	l_BM.x = static_cast<float>(p_point.first - (p_center.x + (p_scale.x / 2.0f)));
	l_BM.y = static_cast<float>(p_point.second - (p_center.y + (p_scale.y / 2.0f)));

	if((0 <= l_AB.dotP(l_AM) && 0 <= l_BC.dotP(l_BM)))
	{
		if (l_AB.dotP(l_AM) <= l_AB.dotP(l_AB) && l_BC.dotP(l_BM) <= l_BC.dotP(l_BC))
			return true;
	}

	return false;
}

std::shared_ptr<gameObject> uiSystem::findButton(objID p_buttonID)
{
	if(m_currentSelectedButton != 0)
	{
		systemManagerInterface * l_system = getSystemManager();
		sceneSystem * l_sceneSystem = l_system->getSystem<sceneSystem>();
		auto l_scene = l_sceneSystem->getCurrentScene().lock();

    //Try to find the game object
    auto l_gameObject = l_scene->findGameObjectByID(p_buttonID);

    //If null we need to reset the current button
    if(l_gameObject == nullptr)
    {
      cleanListOfButtons();
      resetCurrentButton();

      l_gameObject = l_scene->findGameObjectByID(m_currentSelectedButton);
    }

		return l_gameObject;
	}

  return nullptr;
}

void uiSystem::cleanListOfButtons()
{
  systemManagerInterface * l_system = getSystemManager();
  sceneSystem * l_sceneSystem = l_system->getSystem<sceneSystem>();
  auto l_scene = l_sceneSystem->getCurrentScene().lock();

  for(auto l_gameObjectID : m_listOfButtons)
  {
    //Try to find the game object
    auto l_gameObject = l_scene->findGameObjectByID(l_gameObjectID);

    //If null, remove it from the list
    if(l_gameObject == nullptr)
    {
      auto l_iter = std::find(m_listOfButtons.begin(), m_listOfButtons.end(), l_gameObjectID);

      //Remove it from current buttons.
      if (l_iter != m_listOfButtons.end())
      {
        l_iter = m_listOfButtons.erase(l_iter);
        //Reset current selected button
        if(l_gameObjectID == m_currentSelectedButton)
        {
          m_currentSelectedButton = 0;
        }
      }
    }
  }
}

void uiSystem::resetCurrentButton()
{
  systemManagerInterface * l_system = getSystemManager();
  sceneSystem * l_sceneSystem = l_system->getSystem<sceneSystem>();
  auto l_scene = l_sceneSystem->getCurrentScene().lock();

  for (auto l_gameObjectID : m_listOfButtons)
  {
    //Try to find the game object
    auto l_gameObject = l_scene->findGameObjectByID(l_gameObjectID);

    if(l_gameObject != nullptr)
    {
      auto l_button = l_gameObject->getComponent<button>();

      if (l_button)
      {
        if(l_button->isSelectable())
        {
          if (l_button->isActive())
          {
            m_currentSelectedButton = l_gameObjectID;
            l_button->setSelected(true);
            break;
          }
        }
      }
    }
  }
}

