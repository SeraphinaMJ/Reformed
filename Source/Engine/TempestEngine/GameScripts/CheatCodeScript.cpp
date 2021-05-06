/*************************************************************************************
\file       CheatCodeScript.cpp
\author     Ryan Booth
\date       12 / 6 / 2019
\copyright  All content © 2018 - 2019 DigiPen(USA) Corporation, all rights reserved.
\par
\brief      
*****************************************************************************************/

#include "CheatCodeScript.hpp"
#include "Player.hpp"
#include "Health.hpp"
#include "InputComponent.hpp"
#include <IComponentRepository.hpp>
#include <Debug.hpp>
#include "GraphicsSystem.hpp" //for debug line toggling
#include "Engine.hpp"

cheatCodeScript::cheatCodeScript()
{

}

cheatCodeScript::cheatCodeScript(typeRT p_data)
{

}

typeRT cheatCodeScript::toTypeRT() const
{
    typeRT toReturn;
    toReturn.setTypeName(getType().name());
    toReturn.setVariableName(getType().name());
    return toReturn;
}

void cheatCodeScript::updateFromTypeRT(typeRT& p_type)
{

}

void cheatCodeScript::onStart()
{
    auto l_obj = getGameObject();

    if(!l_obj.expired())
    {
        auto l_input = l_obj.lock()->addComponent<inputComponent>();
        if(l_input != nullptr)
        {
            l_input->addButtonMapping("cheats_enabled", Key::Control_Left);
            l_input->addButtonMapping("kill", Key::N);
            l_input->addButtonMapping("memoryDebug", Key::M);
			      l_input->addButtonMapping("showDebugLines", Key::Comma);
			      l_input->addButtonMapping("hideDebugLines", Key::Period);
        }
    }
}

void cheatCodeScript::onUpdate()
{
    auto l_obj = getGameObject();

    if(!l_obj.expired())
    {
        auto l_input = l_obj.lock()->getComponent<inputComponent>();

        if(l_input != nullptr)
        {
            //Put cheats under this. All cheats under this will be LEFT-CONTROL + the key
            if (l_input->getButtonState("cheats_enabled"))
            {
                if (l_input->getButtonState("kill"))
                {
                    killPlayer();
                }
            }
            if (l_input->getButtonState("memoryDebug"))
            {
              showMemoryDebug();
            }
			  if (l_input->getButtonState("showDebugLines"))
			  {
				  graphicsSystem * gs = getSystemManager()->getSystem<graphicsSystem>();
				  if (gs)
					  gs->setDebugLineDraw(true);
			  }
			  if (l_input->getButtonState("hideDebugLines"))
			  {
				  graphicsSystem * gs = getSystemManager()->getSystem<graphicsSystem>();
				  if (gs)
					  gs->setDebugLineDraw(false);
			  }
      }
  }
}

void cheatCodeScript::killPlayer()
{
    GameObjectFiltering::componentTypeSet l_pattern;
    l_pattern.setType(player::getType());

    auto l_list = this->getSystemManager()->getGameObjectGatherer()->getList(l_pattern);

    if(l_list.size())
    {
        auto l_player = l_list[0];

        if (l_player)
        {
            auto l_health = l_player->getComponent<Health>();

            if (l_health)
            {
                l_health->Kill();
            }
        }
    }
}

void cheatCodeScript::showMemoryDebug()
{
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
  auto* l_repository = 
    this->getMySpace()->getScene()->getGameObjectCollector()->getComponentRepository();
  std::vector<icomponentRepository::memoryDebugData> debugData;
  l_repository->getMemoryDebugData(debugData);


  for (unsigned i = 0; i < debugData.size(); ++i)
  {
    /*
        std::string name;
    unsigned allocationCount;
    unsigned memoryUsed;
    */
    std::string debugText = debugData[i].name +
      std::string("   Alloc count: ") + std::to_string(debugData[i].allocationCount) +
      std::string("   Memory used: ") + std::to_string(debugData[i].memoryUsed);
    debug::displayText(debugText, "Neuton-Regular", -500, 225 - 50 * i, 0.5f, color::red);
  }

#endif
}
