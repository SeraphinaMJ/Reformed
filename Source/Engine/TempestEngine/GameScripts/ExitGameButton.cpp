/*!***************************************************************************************
\file       ExitGameButton.hpp
\author     Alex Kudebeh
\date       11/26/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Exit game button script
*****************************************************************************************/
#include "ExitGameButton.hpp"
#include "Engine.hpp"
#include "Application.hpp"
#include "ButtonPressedEvent.hpp"
#include <EventSubscribe.hpp>
#include "RequestConfirmation.hpp"
#include "UISprite.hpp"
#include "Transform.hpp"
#include "ScoreTracker.hpp"

void exitGameButtonScript::onStart()
{
	m_button = getComponent<button>();
    m_inside_conformation = false;
}

exitGameButtonScript::exitGameButtonScript()
{
    EH_INITIALIZE();
}

exitGameButtonScript::exitGameButtonScript(typeRT p_data)
{

}

void exitGameButtonScript::onShutdown()
{
	//UNSUBSCRIBE(ButtonPressedEvent, &exitGameButtonScript::HandleEvent, )
}

typeRT exitGameButtonScript::toTypeRT() const
{
	typeRT toReturn;
	toReturn.setTypeName(getType().name());
	toReturn.setVariableName(getType().name());
	return toReturn;
}

void exitGameButtonScript::updateFromTypeRT(typeRT& p_type)
{

}

void exitGameButtonScript::HandleEvent(ButtonPressedEvent& ThisEvent)
{
	std::weak_ptr<gameObject> weakobj = getGameObject();
	if (weakobj.expired())
		return;

    objID l_id = weakobj.lock()->getID();

    if (l_id == ThisEvent.object)
    {
        //Need to throw up texture asking Are you sure
        m_inside_conformation = true;

		space * l_space = weakobj.lock()->getSpace();
		if (l_space)
		{
			std::shared_ptr<gameObject> l_confirmation_child = l_space->instantiateGameObject();
			if (l_confirmation_child)
			{
				l_confirmation_child->setName("ConfirmationChild");
				l_confirmation_child->addComponent<requestConfirmation>();
				auto l_sprite = l_confirmation_child->addComponent<uiSprite>();
				if (l_space->getScene()->getName() == "title_screen")
				{
					l_confirmation_child->getComponent<transform>()->setScale(vector3(100, 50, 1));
				}
				else
				{
					l_confirmation_child->getComponent<transform>()->setScale(vector3(1010, 679, 1));

				}

				if (l_sprite)
				{
					l_sprite->setTextureName("AreYouSureYouWantToQuit.png");
				}

				std::shared_ptr<gameObject> l_score_tracker = getMySpace()->getGameObjectByName("ScoreTracker");
				if (l_score_tracker)
				{
					auto l_score_tracker_script = l_score_tracker->getComponent<scoreTracker>();

					if (l_score_tracker_script)
					{
						l_score_tracker_script->setShowKills(false);
					}
				}
			}
        }

        //application::requestQuit();
    }
}

void exitGameButtonScript::onUpdate()
{
	//if (!m_button && m_button->isPressed())
	//{
	//	//application::requestQuit();
	//}
	//

    bool l_confirmation_passed = false;

    if(m_inside_conformation)
    {
        auto l_obj = getGameObject();
        if (!l_obj.expired())
        {
            std::shared_ptr<gameObject> l_locked_and_loaded = l_obj.lock();

			space * l_space = l_locked_and_loaded->getSpace();

			if (l_space)
			{
				std::shared_ptr<gameObject> l_child = l_space->getGameObjectByName("ConfirmationChild");

				if (l_child)
				{
					auto l_confirmation_script = l_child->getComponent<requestConfirmation>();

					if (l_confirmation_script)
					{
						if (l_confirmation_script->getStatus())
						{
							if (l_confirmation_script->getConfirmed())
							{
								l_confirmation_passed = true;
							}
							else
							{
								l_confirmation_passed = false;

								m_inside_conformation = false;

								l_child->markForDeletion();

								std::shared_ptr<gameObject> l_score_tracker = getMySpace()->getGameObjectByName("ScoreTracker");
								if (l_score_tracker)
								{
									auto l_score_tracker_script = l_score_tracker->getComponent<scoreTracker>();

									if (l_score_tracker_script)
									{
										l_score_tracker_script->setShowKills(true);
									}
								}
							}
						}
					}
				}
			}
        }
    }

    if(l_confirmation_passed)
    {
        application::requestQuit();
    }
}

SUBSCRIBE(ButtonPressedEvent, &exitGameButtonScript::HandleEvent, EHCLASS(exitGameButtonScript), );
