/*!**********************************************************************************************************
\file       AnimationSystem.cpp
\author     Henry Brobeck
\date       11/13/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#include "AnimationSystem.hpp"
#include "../GameObjectFiltering/ComponentTypeSet.hpp"
#include "../Components/AnimatorComponent.hpp"
#include "../GameObjectGatherer.hpp"
#include "../SceneManagement/GameObject.hpp"

void animationSystem::onStartFrame()
{
	//list through all animations, update running ones
	GameObjectFiltering::componentTypeSet pattern;
	pattern.setType(animatorComponent::getType());
	auto objects = getSystemManager()->getGameObjectGatherer()->getList(pattern);

	for(auto& iter : objects)
	{
		auto anims = iter->getComponents<animatorComponent>();
		for(auto anim : anims)
		{
			animation* currentAnimation = (anim)->getCurrentAnimation();
			if(currentAnimation)
			{
				currentAnimation->update(getDt());
			}
		}
	}

}
