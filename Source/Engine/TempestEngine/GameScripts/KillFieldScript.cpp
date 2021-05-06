/*!***************************************************************************************
\file       KillFieldScript.cpp
\author     Alex Kudebeh
\date       12/7/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Kill things that drop into the abyss
*****************************************************************************************/
#include "KillFieldScript.hpp"
#include "Transform.hpp"
#include "Health.hpp"

killFieldScript::killFieldScript(typeRT p_data)
{

}

void killFieldScript::onStart()
{

}

void killFieldScript::onUpdate()
{
	std::weak_ptr<gameObject> myObj = getGameObject();

	if (myObj.expired())
	{
		return;
	}

	componentHandle<transform> trans = getComponent<transform>();
	if (trans && trans->getGlobalPosition().y < -100.0f)
	{
		componentHandle<Health> myHealth = myObj.lock()->getComponent<Health>();
		if (myHealth)
			myHealth->DecreaseHealth(1000);
	}
}

void killFieldScript::onShutdown()
{

}
