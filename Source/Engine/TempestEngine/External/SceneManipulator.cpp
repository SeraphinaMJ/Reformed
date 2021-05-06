/*!***************************************************************************************
\file       SceneManipulator.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "SceneManipulator.hpp"
#include "../Engine.hpp"
#include "../SystemManager.hpp"
#include "../SceneManagement/SceneSystem.hpp"
#include "../SceneManagement/Scene.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../Components/Renderer.hpp"
#include "../GameObjectGatherer.hpp"
#include "../Components/Camera.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Button.hpp"
#include "../Components/UISprite.hpp"
#include "../Audio/AudioComponent.hpp"
//#include "../Replay/JumbotronComponent.hpp"
#include "../Components/InputComponent.hpp"
#include <ComponentFactory.hpp>
#include "../SceneManagement/UISystem.hpp"
#include "../Rendering/CameraSystem.hpp"
#include "../DynamicWaypoints/DynamicWaypointGraph.hpp"
#include "PhysicsSystem.hpp"
#include <GraphicsSystem.hpp>
#include <Vector2.hpp>
#include <memory>
#include <Reflection.hpp>
#include <GameObjectSerialization.hpp>

sceneManipulator::sceneManipulator(engine* p_engine, systemManagerInterface * p_system_manager, gameObjectGatherer * p_game_object_gatherer)
	: m_engine{p_engine}, m_system_manager{p_system_manager},
	m_game_object_gatherer{p_game_object_gatherer}
{ }


// written by charles
objID sceneManipulator::getParentID(objID p_object_id)
{
    auto object = findGameObjectByID(p_object_id);

    if (object != nullptr)
    {
        if (object->hasParent() == true)
        {
            return object->getParent().lock()->getID();
        }
    }

    return 0;
}

// written by charles
objID sceneManipulator::getSpaceIDForObject(objID p_object_id)
{
    sceneSystem * scene_sys = m_system_manager->getSystem<sceneSystem>();

    const auto& spaces = scene_sys->m_current_scene->getSpaces();
    for (auto & space : spaces)
    {
        auto object = space->getGameObjectByID(p_object_id);
        if (object != nullptr)
        {
            return space->getID();
        }
    }

    // no space found
    return 0;
}

objID sceneManipulator::getSpaceIDFromName(std::string name)
{
    sceneSystem * scene_sys = m_system_manager->getSystem<sceneSystem>();

    const auto spaces = scene_sys->m_current_scene->getSpaces();
    for (auto space : spaces)
    {
        std::string spaceName = space->getName();
        if (spaceName.compare(name.c_str()) == 0)
        {
            return space->getID();
        }
    }

    return 0;
}

std::vector<objID> sceneManipulator::getSceneIDs()
{
	std::vector<objID> results;
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();
	
	if (l_scene_sys != nullptr)
	{
		for (std::shared_ptr<scene> l_scene : l_scene_sys->m_loaded_scenes)
		{
			results.push_back(l_scene->getID());
		}
	}
	return results;
}

std::string sceneManipulator::getSceneName(objID p_scene_id)
{
    std::string results;
    sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

    if(l_scene_sys != nullptr)
    {
		std::shared_ptr<scene> l_scene = findSceneByID(p_scene_id);
        results = l_scene->getName();
    }
    return results;
}

std::vector<objID> sceneManipulator::getSpaceIDsForScene(objID p_scene_id)
{
    std::vector<objID> results;
    sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

    if(l_scene_sys != nullptr)
    {
		std::shared_ptr<scene> l_scene = findSceneByID(p_scene_id);
        for(std::shared_ptr<space> l_space : l_scene->getSpaces())
        {
            results.push_back(l_space->getID());
        }
    }
    return results;
}

std::string sceneManipulator::getSpaceName(objID p_space_id)
{
    std::string results;
    sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

    if(l_scene_sys != nullptr)
    {
		std::shared_ptr<space> l_space = findSpaceByID(p_space_id);
        results = l_space->getName();
    }
    return results;
}

std::vector<objID> sceneManipulator::getObjectIDsForSpace(objID p_space_id)
{
	std::vector<objID> results;
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		std::shared_ptr<space> l_space = findSpaceByID(p_space_id);
		for (std::shared_ptr<gameObject> l_object : l_space->getGameObjectList())
		{
			results.push_back(l_object->getID());
		}
	}
	return results;
}

std::vector<objID> sceneManipulator::getTopObjectIDsForSpace(objID p_space_id)
{
	std::vector<objID> results;
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		std::shared_ptr<space> l_space = findSpaceByID(p_space_id);
		for(std::shared_ptr<gameObject> l_object : l_space->getGameObjectList())
		{
			if(l_object->getParent().expired())
			{
				results.push_back(l_object->getID());
			}
		}
	}
	return results;
}

std::vector<objID> sceneManipulator::getChildObjectIDsForObject(objID p_object_id)
{
	std::vector<objID> results;
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		std::shared_ptr<gameObject> l_object = findGameObjectByID(p_object_id);
		for(std::shared_ptr<gameObject> l_child : l_object->getChildren())
		{
			results.push_back(l_child->getID());
		}
	}
	return results;
}

std::string sceneManipulator::getObjectName(objID p_object_id)
{
	std::string results;
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		std::shared_ptr<gameObject> l_object = findGameObjectByID(p_object_id);
		results = l_object->getName();
	}
	return results;
}

typeRT sceneManipulator::getTypeRT(objID p_id)
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		// find if it is a scene
		std::shared_ptr<scene> l_scene = findSceneByID(p_id);
		if (l_scene != nullptr)
			return l_scene->toTypeRT();
		//// find if it is a space
		std::shared_ptr<space> l_space = findSpaceByID(p_id);
		if (l_space != nullptr)
			return l_space->toTypeRT();
		// find if it is an object
		std::shared_ptr<gameObject> l_object = findGameObjectByID(p_id);
		if(l_object != nullptr)
			return toTypeRT(l_object);
	}
	return typeRT();
}

void sceneManipulator::applyTypeRT(objID p_id, typeRT & p_type)
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();
	if(l_scene_sys != nullptr)
	{
		// find if it is a scene
		// find if it is a space
    std::shared_ptr<space> l_space = findSpaceByID(p_id);
    if (l_space != nullptr)
    {
      if (p_type.members().find("shouldDeleteOnLoad") != p_type.members().end())
      {
        if(p_type.member("shouldDeleteOnLoad").getBool())
        {
          l_space->deleteOnLoad();
        }
        else
        {
          l_space->dontDeleteOnLoad();
        }
      }
    }
		// find if it is an object
		std::shared_ptr<gameObject> l_object = findGameObjectByID(p_id);
		if (l_object != nullptr)
			l_object->applyFromTypeRT(p_type);
	}
}

std::shared_ptr<scene> sceneManipulator::addEmptyScene()
{
  auto l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  return l_scene_sys->createEmptyScene();
}

std::shared_ptr<space> sceneManipulator::addEmptySpace()
{
  auto l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  auto l_scene = l_scene_sys->getCurrentScene().lock();

  if(l_scene != nullptr)
  {
    return l_scene->createSpace();
  }

  return nullptr;
}

std::shared_ptr<space> sceneManipulator::addEmptySpace(objID p_scene_id)
{
  auto l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  auto l_scene = l_scene_sys->getScene(findSceneByID(p_scene_id)->getName());

  if(l_scene != nullptr)
  {
    return l_scene->createSpace();
  }

  return nullptr;
}

objID sceneManipulator::addEmptySpace(const std::string& p_name)
{
	auto l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	auto l_scene = l_scene_sys->getCurrentScene().lock();
	if(l_scene != nullptr)
	{
		auto space = l_scene->createSpace(p_name);
		return space->getID();
	}
	return {};
}

std::shared_ptr<gameObject> sceneManipulator::addEmptyGameObject()
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();
  physicsSystem * l_physics_system = m_system_manager->getSystem<physicsSystem>();

	if(l_scene_sys != nullptr)
	{
		std::weak_ptr<scene> l_cur_scene_ptr = l_scene_sys->getCurrentScene();
		if(!l_cur_scene_ptr.expired())
		{
			std::shared_ptr<scene> l_cur_scene = l_cur_scene_ptr.lock();
			std::shared_ptr<space> l_cur_space = l_cur_scene->getSpaceByIndex(0);
			std::shared_ptr<gameObject> l_object = l_cur_space->instantiateGameObject();
      //Add a renderer
      auto l_comp = l_object->addComponent<renderer>();
      l_comp->setPrimitiveType(CUBE_FILE);
      l_comp->setProgramType(programType::enm_forward);
      //Add a blank rigidBody
      auto l_rigid_body = l_object->addComponent<rigidBody>();
      l_rigid_body->setCollisionLayer(collisionLayer::enm_trigger, collisionLayer::enm_trigger);

      l_physics_system->addPhysicsObjectToWorld(l_object);
      
      return l_object;
		}
	}

  return nullptr;
}

std::shared_ptr<gameObject> sceneManipulator::addEmptyGameObject(objID p_parent_id)
{
  physicsSystem * l_physics_system = m_system_manager->getSystem<physicsSystem>();
	// see if parent is a space
	std::shared_ptr<space> l_space = findSpaceByID(p_parent_id);
	if(l_space != nullptr)
	{
		// add game object directly to space
    std::shared_ptr<gameObject> l_object = l_space->instantiateGameObject();
    //Add a renderer
    auto l_comp = l_object->addComponent<renderer>();
    l_comp->setPrimitiveType(CUBE_FILE);
    l_comp->setProgramType(programType::enm_forward);
    //Add a blank rigidBody
    auto l_rigid_body = l_object->addComponent<rigidBody>();
    l_rigid_body->setCollisionLayer(collisionLayer::enm_trigger, collisionLayer::enm_trigger);

    l_physics_system->addPhysicsObjectToWorld(l_object);
    
    return l_object;
	}
	else
	{
		// see if parent is an object
		std::shared_ptr<gameObject> l_object = findGameObjectByID(p_parent_id);
		if(l_object != nullptr)
		{
			// add new game object as child
			std::shared_ptr<gameObject> l_new_obj = l_object->getSpace()->instantiateGameObject();
      //Add a renderer
      auto l_comp = l_new_obj->addComponent<renderer>();
      l_comp->setPrimitiveType(CUBE_FILE);
      l_comp->setProgramType(programType::enm_forward);
      //Add a blank rigidBody
      auto l_rigid_body = l_new_obj->addComponent<rigidBody>();
      l_rigid_body->setCollisionLayer(collisionLayer::enm_trigger, collisionLayer::enm_trigger);

      l_physics_system->addPhysicsObjectToWorld(l_new_obj);

			l_object->addChild(l_new_obj);

      return l_new_obj;
		}
	}

  return nullptr;
}

objID sceneManipulator::addEmptyGameObject(objID p_parent_id, const std::string& p_name)
{
  physicsSystem * l_physics_system = m_system_manager->getSystem<physicsSystem>();
	// see if parent is a space
	std::shared_ptr<space> l_space = findSpaceByID(p_parent_id);
	if (l_space != nullptr)
	{
		// add game object directly to space
		auto obj = l_space->instantiateGameObject(p_name);
    //Add a renderer
    auto l_comp = obj->addComponent<renderer>();
    l_comp->setPrimitiveType(CUBE_FILE);
    l_comp->setProgramType(programType::enm_forward);
    //Add a blank rigidBody
    auto l_rigid_body = obj->addComponent<rigidBody>();
    l_rigid_body->setCollisionLayer(collisionLayer::enm_trigger, collisionLayer::enm_trigger);

    l_physics_system->addPhysicsObjectToWorld(obj);

		return obj->getID();
	}
	else
	{
		// see if parent is an object
		std::shared_ptr<gameObject> l_object = findGameObjectByID(p_parent_id);
		if (l_object != nullptr)
		{
			// add new game object as child
			std::shared_ptr<gameObject> l_new_obj = l_object->getSpace()->instantiateGameObject(p_name);
      //Add a renderer
      auto l_comp = l_new_obj->addComponent<renderer>();
      l_comp->setPrimitiveType(CUBE_FILE);
      l_comp->setProgramType(programType::enm_forward);
      //Add a blank rigidBody
      auto l_rigid_body = l_new_obj->addComponent<rigidBody>();
      l_rigid_body->setCollisionLayer(collisionLayer::enm_trigger, collisionLayer::enm_trigger);

      l_physics_system->addPhysicsObjectToWorld(l_new_obj);

			l_object->addChild(l_new_obj);
			return l_new_obj->getID();
		}
	}
	return {};
}

void sceneManipulator::addGameObjectComponent(objID p_object_id, const std::string & p_component_type)
{
	//Find the gameObject that was requested
	std::shared_ptr<gameObject> l_game_object = findGameObjectByID(p_object_id);
	
	if (l_game_object != nullptr)
	{
		componentFactory::addComponentByType(l_game_object, p_component_type);
	}
}

void sceneManipulator::removeGameObjectComponent(objID p_object_id, const std::string & p_component_type)
{
	//Find the gameObject that was requested
	std::shared_ptr<gameObject> l_game_object = findGameObjectByID(p_object_id);

	if (l_game_object != nullptr)
	{
		componentFactory::removeComponentByType(l_game_object, p_component_type);
	}
}


void sceneManipulator::readObjectTransform(objID p_object_id, matrixArray1D& p_transform_matrix)
{
	auto l_object = findGameObjectByID(p_object_id);
    if (l_object)
    {
        auto l_transform = l_object->getComponent<transform>();

        std::memcpy(p_transform_matrix, l_transform->getGlobalMatrix().toFloatPtr(), sizeof(p_transform_matrix));
    }
	
}

void sceneManipulator::writeObjectTransform(objID p_object_id, matrixArray1D const& p_transform_matrix)
{
	auto l_object = findGameObjectByID(p_object_id);
	if (p_transform_matrix)
	{
		//if (!l_object->getParent().expired
		//	l_matrix = l_object->getParent().lock()->getComponent<transform>()->getGlobalMatrix().inverse() * l_global_matrix;
		l_object->getComponent<transform>()->fromGlobalMatrix(matrix4x4{ p_transform_matrix });

		for (auto&& i_child : l_object->getChildren())
		{
			i_child->getComponent<transform>()->markDirty();
		}
	}
}

void sceneManipulator::createEditorCamera()
{
	std::shared_ptr<scene> l_scene = m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock();
	std::shared_ptr<space> l_camera_space = l_scene->getSpace("Editor Camera Space");
	if(l_camera_space == nullptr)
	{
		l_camera_space = l_scene->createSpace("Editor Camera Space");
	}
	std::shared_ptr<gameObject> l_camera_object = l_camera_space->instantiateGameObject("Editor Camera");
	l_camera_object->addComponent<camera>();
}

void sceneManipulator::removeEditorCamera()
{
	m_system_manager->getSystem<sceneSystem>()->getCurrentScene().lock()->destroyGameObject("Editor Camera");
}

void sceneManipulator::tellBulletToOverwriteTransform(objID p_object_id)
{
  physicsSystem * l_physics_system = m_system_manager->getSystem<physicsSystem>();
  l_physics_system->overwriteSpecificObject(p_object_id);
}

void sceneManipulator::setButtonNeighbor(objID p_objectID, objID p_neighborID, int p_neighbor)
{
  std::shared_ptr<gameObject> l_object = findGameObjectByID(p_objectID);

  if(l_object != nullptr)
  {
    std::shared_ptr<gameObject> l_neighbor_object = findGameObjectByID(p_neighborID);

    if(l_neighbor_object != nullptr)
    {
      if(l_neighbor_object != nullptr)
      {
        switch (p_neighbor)
        {
        case 0:
        {
          l_object->getComponent<button>()->setNeighbor(l_neighbor_object, button::DIRECTION_RIGHT, true);
          break;
        }
        case 1:
        {
          l_object->getComponent<button>()->setNeighbor(l_neighbor_object, button::DIRECTION_LEFT, true);
          break;
        }
        case 2:
        {
          l_object->getComponent<button>()->setNeighbor(l_neighbor_object, button::DIRECTION_UP, true);
          break;
        }
        case 3:
        {
          l_object->getComponent<button>()->setNeighbor(l_neighbor_object, button::DIRECTION_DOWN, true);
          break;
        }
        default:
        {
          break;
        }
        }
      }
    }
  }
}

objID sceneManipulator::removeButtonNeighbor(objID p_objectID, int p_neighbor)
{
  std::shared_ptr<gameObject> l_object = findGameObjectByID(p_objectID);

  objID l_removed_neighbor_id = 0;

  if (l_object != nullptr)
  {
    switch (p_neighbor)
    {
    case 4:
    {
      l_removed_neighbor_id = l_object->getComponent<button>()->removeNeighbor(button::DIRECTION_RIGHT);
      break;
    }
    case 5:
    {
      l_removed_neighbor_id = l_object->getComponent<button>()->removeNeighbor(button::DIRECTION_LEFT);
      break;
    }
    case 6:
    {
      l_removed_neighbor_id = l_object->getComponent<button>()->removeNeighbor(button::DIRECTION_UP);
      break;
    }
    case 7:
    {
      l_removed_neighbor_id = l_object->getComponent<button>()->removeNeighbor(button::DIRECTION_DOWN);
      break;
    }
    default:
    {
      break;
    }
    }
  }

  return l_removed_neighbor_id;
}

objID sceneManipulator::getObjectID(std::string p_objectName)
{
  std::shared_ptr<gameObject> l_object = findGameObjectByName(p_objectName);

  if(l_object != nullptr)
  {
    return l_object->getID();
  }

  return 0;
}

void sceneManipulator::setCollisionLayer(objID p_object_id, unsigned p_data, unsigned p_type)
{
  std::shared_ptr<gameObject> l_object = findGameObjectByID(p_object_id);

  if(l_object != nullptr)
  {
    auto l_rigidBody = l_object->getComponent<rigidBody>();

    if(l_rigidBody)
    {
      //collisionLayerSelf
      if(p_type == 0)
      {
        l_rigidBody->setCollisionLayer(p_data, l_rigidBody->getCollisionLayer().second);
      }
      //collisionLayerWith
      else if(p_type == 1)
      {
        l_rigidBody->setCollisionLayer(l_rigidBody->getCollisionLayer().first, p_data);
      }
    }
  }

}

void sceneManipulator::removeGameObject(objID p_objectID)
{
  sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  if (l_scene_sys != nullptr)
  {
    for (std::shared_ptr<scene> l_scene : l_scene_sys->m_loaded_scenes)
    {
      for (std::shared_ptr<space> l_space : l_scene->getSpaces())
      {
        for (std::shared_ptr<gameObject> l_object : l_space->getGameObjectList())
        {
          if (l_object->getID() == p_objectID)
          {
            l_space->destroyGameObject(l_object);
            return;
          }
        }
      }
    }
  }
}

void sceneManipulator::addParentToGameObject(objID p_parent_id, objID p_child_id)
{
  auto l_parentObject = findGameObjectByID(p_parent_id);

  if(l_parentObject != nullptr)
  {
    auto l_childObject = findGameObjectByID(p_child_id);

    if(l_childObject != nullptr)
    {
      l_parentObject->addChild(l_childObject);
    }
  }
}

void sceneManipulator::removeParent(objID p_child_id)
{
  auto l_gameObject = findGameObjectByID(p_child_id);

  if(l_gameObject != nullptr)
  {
    l_gameObject->detatchFromParent();
  }
}

void sceneManipulator::setGameObjectName(objID p_object_id, const std::string & p_name)
{
  auto l_gameObject = findGameObjectByID(p_object_id);

  if(l_gameObject != nullptr)
  {
    l_gameObject->setName(p_name);
  }
}

void sceneManipulator::setSceneName(objID p_scene_id, const std::string& p_name)
{
  auto l_scene = findSceneByID(p_scene_id);

  if(l_scene != nullptr)
  {
    l_scene->setName(p_name);
  }
}

void sceneManipulator::setSpaceName(objID p_space_id, const std::string& p_name)
{
  auto l_space = findSpaceByID(p_space_id);

  if (l_space != nullptr)
  {
    l_space->setName(p_name);
  }
}

void sceneManipulator::deleteSpace(objID p_space_id)
{
  auto l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  auto l_scene = l_scene_sys->getCurrentScene().lock();

  if(l_scene != nullptr)
  {
    auto l_space = findSpaceByID(p_space_id);

    if(l_space != nullptr)
    {
      l_scene->destroySpace(l_space);
    }
  }
}

void sceneManipulator::deleteSpace(const std::string& p_space_name)
{
	auto l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	auto l_scene = l_scene_sys->getCurrentScene().lock();

	l_scene->destroySpace(l_scene->getSpace(p_space_name));
}

void sceneManipulator::setUISpriteMaterial(objID p_sprite_id, const std::string & p_name)
{
  auto l_gameObject = findGameObjectByID(p_sprite_id);

  if(l_gameObject != nullptr)
  {
    auto l_sprite = l_gameObject->getComponent<uiSprite>();

    if(l_sprite)
    {
		l_sprite->setTextureName(p_name);
    }
  }
}

void sceneManipulator::moveObjectToSpace(objID p_object_id, const std::string & p_space)
{
  auto l_gameObject = findGameObjectByID(p_object_id);

  if(l_gameObject != nullptr)
  {
    auto l_space = findSpaceByName(p_space);

    if(l_space != nullptr)
    {
      auto l_old_space = l_gameObject->getSpace();

      if(l_old_space != nullptr)
      {
        l_old_space->moveGameObject(l_gameObject, l_space.get());
      }
    }
  }
}

void sceneManipulator::moveObjectToSpace(objID p_object_id, objID p_space_id)
{
    std::string spaceName = getSpaceName(p_space_id);
    moveObjectToSpace(p_object_id, spaceName);
}

void sceneManipulator::addRenderedGameObject(float p_x, float p_y, float p_z, std::string p_object_name, std::string p_texture)
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if (l_scene_sys != nullptr)
	{
		std::weak_ptr<scene> l_cur_scene_ptr = l_scene_sys->getCurrentScene();
		if (!l_cur_scene_ptr.expired())
		{
			std::shared_ptr<scene> l_cur_scene = l_cur_scene_ptr.lock();
			std::shared_ptr<space> l_cur_space = l_cur_scene->getSpaceByIndex(0);

			auto l_new_game_object = l_cur_space->instantiateGameObject();

			//Set the name
			l_new_game_object->setName(p_object_name);

			//Set the position
			l_new_game_object->getComponent<transform>()->setPosX(p_x);
			l_new_game_object->getComponent<transform>()->setPosY(p_y);
			l_new_game_object->getComponent<transform>()->setPosZ(p_z);

			//Set the texture
      auto l_comp = l_new_game_object->addComponent<renderer>();
      l_comp->setPrimitiveType(CUBE_FILE);
      l_comp->setProgramType(programType::enm_forward);
			l_new_game_object->getComponent<renderer>()->setMaterialName(p_texture);
		}
	}
}

std::shared_ptr<scene> sceneManipulator::findSceneByID(objID p_scene_id)
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		for(std::shared_ptr<scene> l_scene : l_scene_sys->m_loaded_scenes)
		{
			if (l_scene->getID() == p_scene_id)
			{
				return l_scene;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<space> sceneManipulator::findSpaceByID(objID p_space_id)
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		for(std::shared_ptr<scene> l_scene : l_scene_sys->m_loaded_scenes)
		{
			for(std::shared_ptr<space> l_space : l_scene->getSpaces())
			{
				if(l_space->getID() == p_space_id)
				{
					return l_space;
				}
			}
		}
	}
	return nullptr;
}

std::shared_ptr<space> sceneManipulator::findSpaceByName(const std::string& p_space)
{
  sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  if (l_scene_sys != nullptr)
  {
    auto l_scene = l_scene_sys->getCurrentScene().lock();

    if(l_scene != nullptr)
    {
      for (std::shared_ptr<space> l_space : l_scene->getSpaces())
      {
        if(l_space->getName() == p_space)
        {
          return l_space;
        }
      }
    }
  }

  return nullptr;
}

std::shared_ptr<gameObject> sceneManipulator::findGameObjectByName(std::string p_objectName)
{
  sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

  if (l_scene_sys != nullptr)
  {
    for (std::shared_ptr<scene> l_scene : l_scene_sys->m_loaded_scenes)
    {
      for (std::shared_ptr<space> l_space : l_scene->getSpaces())
      {
        for (std::shared_ptr<gameObject> l_object : l_space->getGameObjectList())
        {
          if (p_objectName.compare(l_object->getName()) == 0)
          {
            return l_object;
          }
        }
      }
    }
  }
  return nullptr;
}

std::shared_ptr<gameObject> sceneManipulator::findGameObjectByID(objID p_object_id)
{
	sceneSystem * l_scene_sys = m_system_manager->getSystem<sceneSystem>();

	if(l_scene_sys != nullptr)
	{
		for(std::shared_ptr<scene> l_scene : l_scene_sys->m_loaded_scenes)
		{
			for(std::shared_ptr<space> l_space : l_scene->getSpaces())
			{
				for(std::shared_ptr<gameObject> l_object : l_space->getGameObjectList())
				{
					if(l_object->getID() == p_object_id)
					{
						return l_object;
					}
				}
			}
		}
	}
	return nullptr;
}

void sceneManipulator::dynamicWaypointGraphCreatePath()
{
	dynamicWaypointGraph * dwg = m_system_manager->getSystem<dynamicWaypointGraph>();
	if (dwg != nullptr)
		dwg->createLevelPath();
}

void sceneManipulator::dynamicWaypointGraphCreateSetPaths()
{
	dynamicWaypointGraph * dwg = m_system_manager->getSystem<dynamicWaypointGraph>();
	if (dwg != nullptr)
		dwg->createNodeSetPaths();
}

void sceneManipulator::dynamicWaypointGraphStitchPaths()
{
	dynamicWaypointGraph * dwg = m_system_manager->getSystem<dynamicWaypointGraph>();
	if (dwg != nullptr)
		dwg->stitchPath();
}

std::vector<objID> sceneManipulator::dynamicWaypointGraphClear()
{
	dynamicWaypointGraph * dwg = m_system_manager->getSystem<dynamicWaypointGraph>();
	if (dwg != nullptr)
		return dwg->clear();

	return std::vector<objID>();
}

void sceneManipulator::dynamicWaypointGraphDeleteRandomNodeSet()
{
	dynamicWaypointGraph * dwg = m_system_manager->getSystem<dynamicWaypointGraph>();
	if (dwg != nullptr)
		dwg->deleteRandomNodeSet();
}

void sceneManipulator::dynamicWaypointGraphTestFunction()
{
	dynamicWaypointGraph * dwg = m_system_manager->getSystem<dynamicWaypointGraph>();
	if (dwg != nullptr)
		dwg->testFunction();
}

objID sceneManipulator::getObjectFromClick(float p_mousePosX, float p_mousePosY)
{
	auto * l_physics_sys = m_system_manager->getSystem<physicsSystem>();
	auto * l_graphics_sys = m_system_manager->getSystem<graphicsSystem>();

	if (l_physics_sys == nullptr || l_graphics_sys == nullptr)
		return 0;

	const auto [l_viewOff, l_viewDim] = l_graphics_sys->getViewport().asVectors();

	auto l_adjustedPos = vector2(p_mousePosX, p_mousePosY) - l_viewOff;

	vector3 l_point, l_normal;
	objID l_object;
	if(l_physics_sys->castRay(l_viewDim.x, l_viewDim.y, l_adjustedPos.x, l_adjustedPos.y, collisionLayer(enm_world | enm_player | enm_enemy | enm_trigger), l_point, l_normal, l_object))
	{
		return l_object;
	}

	return 0;
}

void sceneManipulator::setPhysicsSystemTimeStep(float p_time_step)
{
  physicsSystem * l_physics_sys = m_system_manager->getSystem<physicsSystem>();

  if (l_physics_sys != nullptr)
  {
    l_physics_sys->setSimulationTimeStep(p_time_step);
  }
}

void sceneManipulator::setPhysicsEngineRunning(bool p_running)
{
  physicsSystem * l_physics_sys = m_system_manager->getSystem<physicsSystem>();

  if (l_physics_sys != nullptr)
  {
    l_physics_sys->setEngineRunning(p_running);
  }
}

void sceneManipulator::addPhysicsObjectFromEditor(objID p_object)
{
  std::shared_ptr<gameObject> l_object = findGameObjectByID(p_object);

  if(l_object != nullptr)
  {
    auto l_rigid_body = l_object->getComponent<rigidBody>();

    if(l_rigid_body)
    {
      l_rigid_body->setNewRigidBodyFlag(true);
    }
  }
}

void sceneManipulator::setPhysicsIsDirtyFlag(objID p_object)
{
  std::shared_ptr<gameObject> l_object = findGameObjectByID(p_object);

  if (l_object != nullptr)
  {
    auto l_transform = l_object->getComponent<transform>();

    if (l_transform)
    {
      l_transform->setPhysicsIsDirtyFlag(true);
    }
  }
}

void sceneManipulator::setPhysicsGravity(vector3 p_gravity)
{
  physicsSystem * l_physics_sys = m_system_manager->getSystem<physicsSystem>();

  if (l_physics_sys != nullptr)
  {
    l_physics_sys->setGlobalGravity(p_gravity);
  }
}
