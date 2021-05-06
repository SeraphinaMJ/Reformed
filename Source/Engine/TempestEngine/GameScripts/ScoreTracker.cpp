/*!***************************************************************************************
\file       ScoreTracker.cpp
\author     Alex Kudebeh
\date       12/3/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/

//======== Self Include ================================================================//
#include "ScoreTracker.hpp"
//======== 1st Party Includes ==========================================================//
#include "UISprite.hpp"
#include "Button.hpp"
#include "Transform.hpp"
#include "PlayButtonScript.hpp"
#include "ExitGameButton.hpp"
#include "Debug.hpp"
#include "Player.hpp"
#include "EventSubscribe.hpp"
#include "InputComponent.hpp"
#include "InputFramework.hpp"
#include "PikminPlayerControllerScript.hpp"
#include "TargetingCircleScript.hpp"
//======== 3rd Party Includes ==========================================================//
#include <sstream>
#include <iomanip> //setw, setfill
#include "Application.hpp"

//======== Types =======================================================================//
//======== Defines =====================================================================//
#define COOLDOWN 0.5f;
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
scoreTracker::scoreTracker()
{
	EH_INITIALIZE();
}

scoreTracker::scoreTracker(typeRT & p_data)
{
	EH_INITIALIZE();
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
void scoreTracker::onStart()
{
    auto l_obj = getGameObject();

    if (!l_obj.expired())
    {
        auto l_input = l_obj.lock()->addComponent<inputComponent>();
        if (l_input != nullptr)
        {
            l_input->addButtonMapping("openPauseMenu", Key::ESC);
        }
    }

    m_timer = 0.0f;
    m_show_kills = false;
}

void scoreTracker::onUpdate()
{
	if (m_gameOver == false)
	{
		GameObjectFiltering::componentTypeSet playerPattern;
		playerPattern.setType(player::getType());
		std::vector<std::shared_ptr<gameObject>> players = getSystemManager()->getGameObjectGatherer()->getList(playerPattern);

        auto l_obj = getGameObject();

        if (!l_obj.expired())
        {
            auto l_input = l_obj.lock()->getComponent<inputComponent>();

            if (l_input != nullptr)
            {
                if(m_timer > 0.0f)
                {
                    m_timer -= dt();
                }
                else
                {
                    if (l_input->getButtonState("openPauseMenu"))
                    {
						GameObjectFiltering::componentTypeSet pikPattern;
						playerPattern.setType(pikminPlayerControllerScript::getType());
						std::vector<std::shared_ptr<gameObject>> players = getSystemManager()->getGameObjectGatherer()->getList(pikPattern);

						componentHandle<pikminPlayerControllerScript> ppcs = players[0]->getComponent<pikminPlayerControllerScript>();

                        if (m_pause_open == false)
                        {
                            showPauseMenu();

							inputFramework::disableMouseCapture();

							if (ppcs)
								ppcs->disableController();

                            m_pause_open = true;

                            m_show_kills = true;

                            //Pause game
                            application::requestPause();

                            m_timer += COOLDOWN;
                        }
                        else
                        {
                            //remove pause menu
                            hidePauseMenu();
                            m_pause_open = false;


							inputFramework::enableMouseCapture();

							if (ppcs)
								ppcs->enableController();

                            //resume game
                            application::requestResume();

                            m_timer += COOLDOWN;
                        }
                    }
                    else
                    {
                        if (players.size() == 0 && m_pause_open == false)
                        {
                            m_show_kills = true;
                            m_gameOver = true;
                            m_pause_open = true;
                            showPauseMenu();
                        }
                    }
                }
            }
        }
	}

    if(m_show_kills)
    {
        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << m_killCount;
        debug::displayText(ss.str(), "Neuton-Regular", -90, 145, 2.0f, color::blue);
        debug::displayText("Enemies Defeated!", "Neuton-Regular", -125, 110, 0.7f, color::blue);
    }
}

void scoreTracker::updateFromTypeRT(typeRT & p_type)
{

}

typeRT scoreTracker::toTypeRT() const
{
	typeRT toReturn;
	toReturn.setTypeName(getType().name());
	toReturn.setVariableName(getType().name());
	return toReturn;
}

//======== Functionality ===============================================================//
void scoreTracker::addKill()
{
	++m_killCount;
}

void scoreTracker::HandleEvent(EnemyDeathEvent& event)
{
	addKill();
}

void scoreTracker::showPauseMenu()
{
	componentHandle<uiSprite> scoreScreen;
	componentHandle<uiSprite> replaySprite;
	componentHandle<uiSprite> menuSprite;
	componentHandle<uiSprite> quitSprite;

    std::weak_ptr<gameObject> l_obj = getGameObject();
    if(!l_obj.expired())
    {
        std::shared_ptr<gameObject> l_locked_and_loaded = l_obj.lock();
        std::shared_ptr<gameObject> scoreScreenObj = l_locked_and_loaded->getSpace()->instantiateGameObject();
        
        if(scoreScreenObj)
        {
            scoreScreenObj->setName("ScoreScreenObject");
        }
        scoreScreen = scoreScreenObj->addComponent<uiSprite>();

        if (m_gameOver)
        {
            scoreScreen->setTextureName("GameOver.png");
        }
        else
        {
            scoreScreen->setTextureName("ScoreScreen.png");
        }

        scoreScreenObj->getComponent<transform>()->setScale(vector3(1010, 679, 0));

        std::shared_ptr<gameObject> replayButtonObject = l_locked_and_loaded->getSpace()->instantiateGameObject();
        replayButtonObject->setName("ReplayButton");
        replaySprite = replayButtonObject->addComponent<uiSprite>();
        replaySprite->setTextureName("ReplayButton.png");
        replayButtonObject->getComponent<transform>()->setPosition(vector3(0, -50, 0));
        replayButtonObject->getComponent<transform>()->setScale(vector3(250, 50, 1));

        componentHandle<button> replayButtonButton = replayButtonObject->addComponent<button>();
        replayButtonButton->setSelectable(true);
        replayButtonButton->setPressed(false);
        replayButtonObject->addComponent<playButtonScript>()->setSceneName("Cool_Game");
        replayButtonObject->getComponent<playButtonScript>()->setNeedsConfirmation(true);

        std::shared_ptr<gameObject> menuButtonObject = l_locked_and_loaded->getSpace()->instantiateGameObject();
        menuButtonObject->setName("MenuButton");
        menuSprite = menuButtonObject->addComponent<uiSprite>();
        menuSprite->setTextureName("MainMenuButton.png");
        menuButtonObject->getComponent<transform>()->setPosition(vector3(0, -200, 0));
        menuButtonObject->getComponent<transform>()->setScale(vector3(250, 50, 1));
        
        componentHandle<button> menuButtonButton = menuButtonObject->addComponent<button>();
        menuButtonButton->setSelectable(true);
        menuButtonObject->addComponent<playButtonScript>()->setSceneName("title_screen");
        menuButtonObject->getComponent<playButtonScript>()->setNeedsConfirmation(true);

        std::shared_ptr<gameObject> quitButtonObject = l_locked_and_loaded->getSpace()->instantiateGameObject();
        quitButtonObject->setName("QuitButton2");
        quitSprite = quitButtonObject->addComponent<uiSprite>();
        quitSprite->setTextureName("QuitButton.png");
        quitButtonObject->getComponent<transform>()->setPosition(vector3(0, -350, 0));
        quitButtonObject->getComponent<transform>()->setScale(vector3(250, 50, 1));

        componentHandle<button> quitButtonButton = quitButtonObject->addComponent<button>();
        quitButtonButton->setSelectable(true);
        quitButtonObject->addComponent<exitGameButtonScript>();

        menuButtonButton->setNeighbor(replayButtonObject, button::DIRECTION_UP, true);
        menuButtonButton->setNeighbor(quitButtonObject, button::DIRECTION_DOWN, true);

        replayButtonButton->setSelected(true);

		auto l_objs = findObjectsByComponent<targetingCircleScript>();
		for (auto l_obj : l_objs)
		{
			l_obj->getComponent<targetingCircleScript>()->pauseMenuShown(true);
		}
    }
}

void scoreTracker::hidePauseMenu()
{
    m_show_kills = false;

    std::weak_ptr<gameObject> l_obj = getGameObject();
    if (!l_obj.expired())
    {
        std::shared_ptr<gameObject> l_locked_and_loaded = l_obj.lock();
        space * l_space = l_locked_and_loaded->getSpace();

        std::shared_ptr<gameObject> l_score_screen = l_space->getGameObjectByName("ScoreScreenObject");
        if(l_score_screen)
        {
            l_score_screen->markForDeletion();
        }
        std::shared_ptr<gameObject> l_replay = l_space->getGameObjectByName("ReplayButton");
        if (l_replay)
        {
            l_replay->markForDeletion();
        }
        std::shared_ptr<gameObject> l_menu = l_space->getGameObjectByName("MenuButton");
        if (l_menu)
        {
            l_menu->markForDeletion();
        }
        std::shared_ptr<gameObject> l_quit = l_space->getGameObjectByName("QuitButton2");
        if (l_quit)
        {
            l_quit->markForDeletion();
        }

		auto l_objs = findObjectsByComponent<targetingCircleScript>();
		for (auto l_obj : l_objs)
		{
			l_obj->getComponent<targetingCircleScript>()->pauseMenuShown(false);
		}
    }
}

//======== Helper Functions ============================================================//

SUBSCRIBE(EnemyDeathEvent, &scoreTracker::HandleEvent, EHCLASS(scoreTracker), );

