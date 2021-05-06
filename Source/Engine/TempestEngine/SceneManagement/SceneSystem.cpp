/*!***************************************************************************************
\file       SceneSystem.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the SceneSystem class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "SceneSystem.hpp"

//========1st Party Includes============================================================//
#include "../GameObjectCollector.hpp"
#include "../PersistenceSystem.hpp"
#include "../Serialization.h"
#include "Scene.hpp"
#include "Transform.hpp"
#include "../Components/InputComponent.hpp"
#include "../GameScripts/InputTestScript.hpp"
#include "../PersistenceRequest.hpp"
#include "../SystemManagement/SystemManagerInterface.hpp"

//========3rd Party Includes============================================================//
// #include <corecrt_io.h> // TODO: Is this one needed?
#include <memory>
#include <filesystem>
#include <vector>
#include <stdexcept>
#include "../Components/Renderer.hpp"
#include "../Components/Camera.hpp"
#include "../Components/Skybox.hpp"
#include "../Components/Light.hpp"
#include <algorithm>
#include "EventBus.hpp"

////////==========================================================================////////
////////  Class(sceneSystem)                                                      ////////
////////==========================================================================////////

    ///////============================================================================///////
    ///////   Public                                                                   ///////
    ///////============================================================================///////

        //////==============================================================================//////
        //////    Non-Static                                                                //////
        //////==============================================================================//////

        /////================================================================================/////
        /////     Functions                                                                  /////
        /////================================================================================/////

        ////==================================================================================////
        ////      Non-Virtual                                                                 ////
        ////==================================================================================////

        //======================================================================================//
        //        Helper                                                                        //
        //======================================================================================//


void sceneSystem::reset()
{
    std::shared_ptr<scene> l_new_scene = createEmptyScene();

    if (m_current_scene != nullptr)
    {
        auto l_spaceList = m_current_scene->getSpaces();
        for (auto i_spaces = l_spaceList.begin(); i_spaces != l_spaceList.end(); )
        {
            if (!(*i_spaces)->getDeleteOnLoad())
            {
                l_new_scene->moveSpace(*i_spaces);
                i_spaces = l_spaceList.erase(i_spaces);
            }
            else
                ++i_spaces;
        }
    }
    setmCurrentScene(l_new_scene);
    auto l_space = m_current_scene->createSpace();

    std::shared_ptr<gameObject> l_camera_object = l_space->instantiateGameObject();
    l_camera_object->setName("Main Camera");
    l_camera_object->getComponent<transform>()->setScale(vector3(1, 1, 1));
    l_camera_object->getComponent<transform>()->setRotation(vector3(-0.15f, 0, 0));
    l_camera_object->getComponent<transform>()->setPosition(vector3(0.0f, 10.0f, 150.0f));
    l_camera_object->addComponent<skybox>();
    l_camera_object->addComponent<camera>();

    std::shared_ptr<gameObject> l_light_object = l_space->instantiateGameObject();
    l_light_object->setName("Default Light");
    l_light_object->getComponent<transform>()->setScale(vector3(1, 1, 1));
    l_light_object->getComponent<transform>()->setRotation(vector3(0.0f, 0, 0));
    l_light_object->getComponent<transform>()->setPosition(vector3(0.0f, 10.0f, 0.0f));
    auto l_light_comp = l_light_object->addComponent<light>();
    l_light_comp->setLightType(lighttype::enm_point);
    l_light_comp->setDiffuseColor(color(1.0f, 1.0f, 1.0f));
    componentHandle<light> l_light = l_light_object->getComponent<light>();
    l_light->setIntensity(1.5f);

    //l_camera_object->addComponent(new renderer(primitiveType::enm_cube, programType::enm_forward));

    //l_camera_object->addComponent(new renderer(primitiveType::enm_cube, programType::enm_forward));


    componentHandle<inputComponent> l_input_component = l_camera_object->addComponent<inputComponent>();
    l_input_component->addButtonMapping("forward", 'W');
    l_input_component->addButtonMapping("backward", 'S');
    l_input_component->addButtonMapping("left", 'A');
    l_input_component->addButtonMapping("right", 'D');
    l_input_component->addButtonMapping("up", ' ');
    l_input_component->addButtonMapping("down", '~');

    l_input_component->addButtonMapping("lmb", MouseButton::Left);
    l_input_component->addButtonMapping("rmb", MouseButton::Right);
    l_input_component->addCursorMapping("mouse", Cursor::Default_Mouse);
    //l_input_component->addButtonMapping("capture", 'C');
    l_input_component->addButtonMapping("playSfx", 'P');
}

size_t sceneSystem::getSpaceCount() const
{
    return getCurrentScene().lock()->getSpaces().size();
}

size_t sceneSystem::getGameObjectCount() const
{
    size_t l_count = 0;
    auto & l_spaces = getCurrentScene().lock()->getSpaces();
    for (auto & l_space : l_spaces)
    {
        l_count += l_space->getObjectCount();
    }
    return l_count;
}

void sceneSystem::onInitialize()
{
    resetmCurrentScene();
    reset();
}

void sceneSystem::onPreShutdown()
{
    resetmCurrentScene();
}

void sceneSystem::onShutdown()
{
    m_loaded_scenes.clear();
    m_needs_reset = false;
}

void sceneSystem::onUpdate()
{
    //logger("sceneSystem").info() << "SceneSystem tick";
    std::vector<objID> objectsToDelete;

    auto spaces = this->m_current_scene->getSpaces();

    // for each space find objects marked for deletion
    for (auto space : spaces)
    {
        auto spaceObjects = space->getGameObjectList();

        for (auto object : spaceObjects)
        {
            if (object->markedForDeletion())
            {
                objectsToDelete.push_back(object->getID());
            }
        }
    }

    // finally delete objects
    for (objID objectToDelete : objectsToDelete)
    {
        this->m_current_scene->destroyGameObjectByID(objectToDelete);
    }
}

void sceneSystem::setmCurrentScene(std::shared_ptr<scene> p_new_scene)
{
    EVENT_BUS.fire(new levelUnloadEvent(m_current_scene), nullptr, true);
    m_current_scene = p_new_scene;
    EVENT_BUS.fire(new levelLoadEvent(m_current_scene), nullptr, true);
}

void sceneSystem::resetmCurrentScene()
{
    EVENT_BUS.fire(new levelUnloadEvent(m_current_scene), this, true);
    m_current_scene.reset();
}

std::shared_ptr<scene> sceneSystem::createEmptyScene()
{
    std::shared_ptr<scene> l_new_scene = std::shared_ptr<scene>(new scene(m_go_collector));
    m_loaded_scenes.push_back(l_new_scene);
    return l_new_scene;
}

std::shared_ptr<scene> sceneSystem::getScene(std::string p_name)
{
    for (auto i_scenes : m_loaded_scenes)
    {
        if (i_scenes->getName() == p_name)
        {
            return i_scenes;
        }
    }
    return nullptr;
}

sceneSystem::sceneSystem(gameObjectCollector * p_collector)
    : m_go_collector{ p_collector }, m_needs_reset(false)
{ }

std::weak_ptr<scene> sceneSystem::getCurrentScene() const
{
    return m_current_scene;
}

std::shared_ptr<scene> sceneSystem::loadScene(std::string p_location, std::string p_name)
{
    auto new_scene = createEmptyScene();
    auto l_request = persistenceRequest<scene>(Direction::Read, "scenes/", p_name + ".cee", new_scene);
    l_request.execute(p_location);
    auto test = l_request.getData();
    return l_request.getData();
}

std::shared_ptr<scene> sceneSystem::loadScene(std::string p_name)
{
    return loadScene(getSystemManager()->getConfigurationFunction()().data_path.generic_string(), p_name);
}

void sceneSystem::setCurrentScene(std::shared_ptr<scene> p_new_scene)
{
    // move any spaces that are marked for donotdelete
    std::vector<std::shared_ptr<space>> l_spaces_to_move;
    for (auto i_space : m_current_scene->getSpaces())
    {
        if (!i_space->getDeleteOnLoad())
        {
            l_spaces_to_move.push_back(i_space);
        }
    }
    for (auto i_space : l_spaces_to_move)
    {
        if (p_new_scene->getSpace(i_space->getName()) == nullptr)
        {
            p_new_scene->moveSpace(i_space);
        }
    }

    setmCurrentScene(p_new_scene);
}

void sceneSystem::setCurrentScene(std::string p_name)
{
    // find it by name
    for (auto i_scene : m_loaded_scenes)
    {
        if (i_scene->getName() == p_name)
        {
            // call the proper setCurrentScene method
            setCurrentScene(i_scene);
            return;
        }
    }
    //You tried to set a scene that wasn't loaded!!!
    throw std::runtime_error("You tried to set a scene that wasn't loaded!!!");
}

gameObjectCollector* sceneSystem::getGameObjectCollector() const
{
    return m_go_collector;
}

std::weak_ptr<gameObject> sceneSystem::getGameObjectByID(objID id) const
{
    return this->m_current_scene->findGameObjectByID(id);
}

void sceneSystem::setGameObjectCollector(gameObjectCollector* p_go_collector)
{
    m_go_collector = p_go_collector;
    for (auto & l_scene_iterator : m_loaded_scenes)
    {
        l_scene_iterator->setGameObjectCollector(m_go_collector);
    }
}

void sceneSystem::unloadScene(std::shared_ptr<scene> p_scene)
{
    //Make sure it exists
    for (auto i_scene = m_loaded_scenes.begin(); i_scene != m_loaded_scenes.end(); ++i_scene)
    {
        //Found the scene
        if (*i_scene == p_scene)
        {
            m_loaded_scenes.erase(i_scene);
            break;
        }
    }
}

void sceneSystem::requestResetCurrentScene()
{
    m_needs_reset = true;
}

void sceneSystem::resetCurrentScene()
{
    auto l_systemManager = getSystemManager();
    //load menu, unload current level, load current level, unload menu
    //Shutup it works

    //load menu and set as current scene, then unload old scene
    auto currentScene = l_systemManager->getSystem<sceneSystem>()->getCurrentScene();
    std::string l_current_scene_name = currentScene.lock()->getName();
    
    auto l_scene = l_systemManager->getSystem<sceneSystem>()->createEmptyScene();
    if (l_scene == nullptr)
    {
        l_scene = getSystemManager()->getSystem<sceneSystem>()->loadScene("title_screen");
    }
    getSystemManager()->getSystem<sceneSystem>()->setCurrentScene(l_scene);
    getSystemManager()->getSystem<sceneSystem>()->unloadScene(currentScene.lock());

    //switch back to current scene, after reloading it
    currentScene = l_systemManager->getSystem<sceneSystem>()->getCurrentScene();
    l_scene = l_systemManager->getSystem<sceneSystem>()->getScene(l_current_scene_name);
    if (l_scene == nullptr)
    {
        l_scene = getSystemManager()->getSystem<sceneSystem>()->loadScene(l_current_scene_name);
    }
    getSystemManager()->getSystem<sceneSystem>()->setCurrentScene(l_scene);
    getSystemManager()->getSystem<sceneSystem>()->unloadScene(currentScene.lock());
}

bool sceneSystem::getCurrentSceneNeedsReset()
{
    if(m_needs_reset)
    {
        m_needs_reset = false;
        return true;
    }

    return false;
}
