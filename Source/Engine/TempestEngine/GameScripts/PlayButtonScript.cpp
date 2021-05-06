/*!***************************************************************************************
\file       PlayButtonScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "PlayButtonScript.hpp"
#include "EventSubscribe.hpp"
#include "Application.hpp"
#include "RequestConfirmation.hpp"
#include "UISprite.hpp"
#include "Transform.hpp"
#include "ScoreTracker.hpp"


playButtonScript::playButtonScript()
{
    
}

playButtonScript::playButtonScript(typeRT p_data)
{
	updateFromTypeRT(p_data);
}

typeRT playButtonScript::toTypeRT() const
{
	// Generate the base script data
	typeRT l_result = scriptCPP::toTypeRT();

	// add data to the l_scriptData object here.
	l_result.insertMember(typeRT("sceneName", m_scene_name));

    l_result.insertMember(typeRT("needs confirmation", m_needs_confirmation));

	return l_result;
}

void playButtonScript::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("sceneName") != l_members.end())
	{
		m_scene_name = p_type.member("sceneName").getString();
	}
    if (l_members.find("needs confirmation") != l_members.end())
    {
        m_needs_confirmation = p_type.member("needs confirmation").getBool();
    }
}

void playButtonScript::HandleEvent(ButtonPressedEvent& ThisEvent)
{
    auto l_object = getGameObject().lock();
    objID l_id = l_object->getID();

    if(l_id == ThisEvent.object)
    {
        auto l_systemManager = getSystemManager();
        //If the play button is pressed, and we are already in the desired scene, then don't switch to it.
        if (l_systemManager->getSystem<sceneSystem>()->getCurrentScene().lock()->getName() != m_scene_name)
        {
            auto l_scene = l_systemManager->getSystem<sceneSystem>()->getScene(m_scene_name);
            if (l_scene == nullptr)
            {
                // load the scene
                l_scene = getSystemManager()->getSystem<sceneSystem>()->loadScene(m_scene_name);
            }
            getSystemManager()->getSystem<sceneSystem>()->setCurrentScene(l_scene);
        }

        //resume game, in case we are paused
        application::requestResume();
    }
}

void playButtonScript::onStart()
{
    m_confirmation_passed = false;
    m_in_confirmation_process = false;
}

void playButtonScript::setSceneName(std::string name)
{
	m_scene_name = name;
}

void playButtonScript::setNeedsConfirmation(bool p_bool)
{
    m_needs_confirmation = p_bool;
}

void playButtonScript::onUpdate()
{
	//if(m_button && m_button->isPressed())
    auto l_button = getComponent<button>();
    if (l_button && l_button->isPressed())
	{
        if (m_needs_confirmation && !m_in_confirmation_process)
        {
            //Need to throw up texture asking Are you sure
            m_in_confirmation_process = true;

            auto l_obj = getGameObject();
            if (!l_obj.expired())
            {
                std::shared_ptr<gameObject> l_locked_and_loaded = l_obj.lock();

                std::shared_ptr<gameObject> l_confirmation_child = l_locked_and_loaded->getSpace()->instantiateGameObject();
                l_confirmation_child->setName("ConfirmationChild");

                if(l_confirmation_child)
                {
                    l_confirmation_child->addComponent<requestConfirmation>();
                    auto l_sprite = l_confirmation_child->addComponent<uiSprite>();
                    l_confirmation_child->getComponent<transform>()->setScale(vector3(1010, 679, 1));

                    if(l_sprite)
                    {
                        l_sprite->setTextureName("AreYouSureYouWantToQuit.png");
                    }
                }
            }

            std::shared_ptr<gameObject> l_score_tracker = getMySpace()->getGameObjectByName("ScoreTracker");
            if(l_score_tracker)
            {
                auto l_score_tracker_script = l_score_tracker->getComponent<scoreTracker>();

                if(l_score_tracker_script)
                {
                    l_score_tracker_script->setShowKills(false);
                }
            }
        }
        else if(!m_needs_confirmation)
        {
            m_confirmation_passed = true;
        }
	}

    if(m_in_confirmation_process)
    {
        auto l_obj = getGameObject();
        if (!l_obj.expired())
        {
            std::shared_ptr<gameObject> l_locked_and_loaded = l_obj.lock();

            std::shared_ptr<gameObject> l_child = l_locked_and_loaded->getSpace()->getGameObjectByName("ConfirmationChild");

            if (l_child)
            {
                auto l_confirmation_script = l_child->getComponent<requestConfirmation>();

                if (l_confirmation_script)
                {
                    if (l_confirmation_script->getStatus())
                    {
                        if (l_confirmation_script->getConfirmed())
                        {
                            m_confirmation_passed = true;
                        }
                        else
                        {
                            m_confirmation_passed = false;

                            m_in_confirmation_process = false;

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

    if (m_confirmation_passed)
    {
        m_confirmation_passed = false;

        auto l_systemManager = getSystemManager();
        std::string currentLevelName = l_systemManager->getSystem<sceneSystem>()->getCurrentScene().lock()->getName();
        //If the play button is pressed, and we are already in the desired scene, then don't switch to it.
        if (currentLevelName != m_scene_name)
        {
            auto currentScene = l_systemManager->getSystem<sceneSystem>()->getCurrentScene();
            auto l_scene = l_systemManager->getSystem<sceneSystem>()->getScene(m_scene_name);
            if (l_scene == nullptr)
            {
                l_scene = getSystemManager()->getSystem<sceneSystem>()->loadScene(m_scene_name);
            }
            getSystemManager()->getSystem<sceneSystem>()->setCurrentScene(l_scene);
            //Need to reset the scene thats already loaded for safety
			if(l_scene->getName() != "title_screen")
			{
				l_systemManager->getSystem<sceneSystem>()->resetCurrentScene();
			}
            //getSystemManager()->getSystem<sceneSystem>()->unloadScene(currentScene.lock());
        }
        else
        {
            //Reset the scene that is already loaded
            l_systemManager->getSystem<sceneSystem>()->requestResetCurrentScene();
		}

        //resume game, in case we are paused
        application::requestResume();
	}
}
