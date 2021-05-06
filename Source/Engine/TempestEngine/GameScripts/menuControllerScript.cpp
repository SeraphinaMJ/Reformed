/*!***************************************************************************************
\file       menuControllerScript.cpp
\author     Ryan Booth
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "menuControllerScript.hpp"
#include "../Components/InputComponent.hpp"
#include "../Components/Button.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"

 menuControllerScript::menuControllerScript(typeRT p_data) : scriptCPP(p_data)
{

}

void menuControllerScript::onUpdate()
{
  auto l_gameObject = getGameObject().lock();
  auto l_input = l_gameObject->getComponent<inputComponent>();

  if (l_input)
  {
    if (l_input->getButtonState("start"))
    {
      auto l_gameSpace = getSystemManager()->getSystem<sceneSystem>()->getCurrentScene().lock()->getSpace("GameUISpace");

      if (l_gameSpace != nullptr)
      {
        auto l_listButtons = l_gameSpace->getGameObjectList();

        for (auto& buttonObject : l_listButtons)
        {
          buttonObject->getComponent<button>()->setActive(false);
        }
      }

      auto l_menuSpace = getSystemManager()->getSystem<sceneSystem>()->getCurrentScene().lock()->getSpace("MenuSpace");

      if (l_menuSpace != nullptr)
      {
        auto l_listButtons = l_menuSpace->getGameObjectList();

        for (auto& buttonObject : l_listButtons)
        {
          buttonObject->getComponent<button>()->setActive(true);
        }
      }
    }
  }

}