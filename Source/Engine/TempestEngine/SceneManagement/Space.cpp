/*!***************************************************************************************
\file       Space.hpp
\author     Aaron Damyen
\date       7/2/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implementation of the space class member functions
*****************************************************************************************/

//========Self Include==================================================================//
#include "Space.hpp"

//========1st Party Includes============================================================//
#include "GameObject.hpp"
#include "Transform.hpp"
#include "../DLLControl.hpp"
#include "../Serialization.h"
#include "../PersistenceRequest.hpp"
//========3rd Party Includes============================================================//
#include <mutex>
#include <filesystem>

std::shared_ptr<gameObject> space::instantiateGameObject()
{
	std::shared_ptr<gameObject> l_go = std::shared_ptr<gameObject>(new gameObject(this));
	l_go->addComponent<transform>();

	object_list_mutex.lock();
	objects.push_back(l_go);
	object_list_mutex.unlock();
	return l_go;
}

std::shared_ptr<gameObject> space::instantiateGameObject(std::string p_name)
{
	std::shared_ptr<gameObject> l_object = instantiateGameObject();
	l_object->setName(p_name);
	return l_object;
}

std::shared_ptr<gameObject> space::instantiateGameObject(typeRT p_data)
{
	std::shared_ptr<gameObject> l_object = instantiateGameObject();
	synchronizeGameObject(l_object, p_data);
	return l_object;
}

bool space::destroyGameObject(std::shared_ptr<gameObject> const & p_game_object)
{
	if (p_game_object->getSpace() == this)
	{
    //Remove my parent
    p_game_object->detatchFromParent();

    //Remove any children I have
    auto l_children = p_game_object->getChildren();

    for(auto l_child : l_children)
    {
      destroyGameObject(l_child);
    }

		//object_list_mutex.lock();
		objects.erase(std::remove(objects.begin(), objects.end(), p_game_object), objects.end());
		//object_list_mutex.unlock();

    // mark object for deletion at cleanup time
    p_game_object->markForDeletion();
		return true;
	}
	return false;
}

//bool space::deleteGameObjectFromDisk(std::shared_ptr<gameObject> const & p_game_object)
//{
//    // get id of object
//    objID id = p_game_object->getID();
//
//    // delete it from memory
//    destroyGameObject(p_game_object);
//
//    // file name is object name
//    std::string objectName = objIDtoString(id);
//
//    // then delete it from disk
//    //std::filesystem::remove()
//    return false;
//}

bool space::moveGameObject(std::shared_ptr<gameObject> & p_game_object, space * p_destination_space)
{
	if (p_game_object->getSpace() == this && p_destination_space != this)
	{
		p_game_object->detatchFromParent();

		destroyGameObject(p_game_object);
		//object_list_mutex.lock();
		p_destination_space->objects.push_back(p_game_object);
    p_game_object->containing_space = p_destination_space;
		//object_list_mutex.unlock();


		return true;
	}
	return false;
}

void space::dontDeleteOnLoad()
{
	m_shouldDeleteOnLoad = false;
}

void space::deleteOnLoad()
{
  m_shouldDeleteOnLoad = true;
}

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

space::space(scene * p_scene, std::string p_name) : m_id{getNextID()}, m_shouldDeleteOnLoad{ true }, enabled { true },
	name{ p_name }, containing_scene{ p_scene }
{ }

space::space(typeRT& p_type_rt) : m_id{ p_type_rt.member("id").getObjID()}, m_shouldDeleteOnLoad{ true }, enabled{ true },
name{p_type_rt.getVariableName()}, containing_scene{nullptr}
{
}

space::~space()
{
	object_list_mutex.lock();
	objects.clear();
	object_list_mutex.unlock();
}

size_t space::getObjectCount() const
{
	return objects.size();
}

bool space::isEnabled() const
{
	return enabled;
}

void space::setEnabled(bool p_is_enabled)
{
	enabled = p_is_enabled;
}

typeRT space::toTypeRT() const
{

	typeRT result;
	result.setTypeName("space");
	result.setVariableName(name);

	std::vector<typeRT> p_gameobjects_as_type_run_time;

	for (const auto& i_gameObjects : getTopLevelGameObjectList())
	{
		typeRT current_gameObjectRT(i_gameObjects->getName());
		current_gameObjectRT.setTypeName("gameObject");
		current_gameObjectRT.insertMember(typeRT("id", i_gameObjects->getID()));
		p_gameobjects_as_type_run_time.push_back(current_gameObjectRT);
	}

	typeRT gameObjectsRT(p_gameobjects_as_type_run_time);
	gameObjectsRT.setTypeName("gameObjects");
	gameObjectsRT.setVariableName("gameObjects");


	result.insertMember(typeRT("id", getID()));
	result.insertMember(gameObjectsRT);

  result.insertMember(typeRT("shouldDeleteOnLoad", m_shouldDeleteOnLoad));


	return result;

}

std::string space::getName() const
{
	return name;
}

void space::setName(std::string p_new_name)
{
	name = p_new_name;
}

scene * space::getScene() const
{
	return containing_scene;
}

void space::setScene(scene* p_scene)
{
	containing_scene = p_scene;
}

/*void space::moveGameObject(space* p_new_space, std::shared_ptr<gameObject> p_gameObject)
{
	auto someshit = std::remove(objects.begin(), objects.end(), p_gameObject);
	p_new_space->objects.push_back(p_gameObject);
	p_gameObject->containing_space = p_new_space;
}*/

std::vector<std::shared_ptr<gameObject>> const space::getGameObjectList() const
{
	std::lock_guard<std::mutex> lock_(object_list_mutex);
	return objects;
}

std::shared_ptr<gameObject> space::getGameObjectByID(objID p_objID)
{
	for(auto l_gameObject : objects)
	{
		if (l_gameObject->getID() == p_objID)
			return l_gameObject;
	}

	return nullptr;
}

std::shared_ptr<gameObject> space::getGameObjectByName(const std::string& p_name)
{
	for (auto l_gameObject : objects)
	{
		if (l_gameObject->getName() == p_name)
			return l_gameObject;
	}

	return nullptr;
}

/*TEMPESTENGINE_API*/ std::vector<std::shared_ptr<gameObject>> const space::getTopLevelGameObjectList() const
{
	std::lock_guard<std::mutex> lock_(object_list_mutex);
	std::vector<std::shared_ptr<gameObject>> results;
	for(auto item : objects)
	{
		if(item->isTopLevelObject())
		{
			results.push_back(item);
		}
	}
	return results;
}

std::shared_ptr<space> space::read(persistenceRequest<space>* p_persist_request)
{
	typeRT l_data = p_persist_request->readDataFromFile();
	auto l_self = p_persist_request->getData();
	l_self->m_id = l_data.member("id").getObjID();
	l_self->name = l_data.getVariableName();

  if(l_data.members().find("shouldDeleteOnLoad") != l_data.members().end())
  {
    l_self->m_shouldDeleteOnLoad = l_data.member("shouldDeleteOnLoad").getBool();
  }

	for (auto i_gameObjects: l_data.member("gameObjects").array())
	{
		std::shared_ptr<gameObject> new_object = l_self->instantiateGameObject();
		std::string gameObject_id = objIDtoString(i_gameObjects.member("id").getObjID());
		persistenceRequest<gameObject> l_request(Direction::Read, "gameObjects/", gameObject_id + ".toy", new_object);
		l_request.execute(p_persist_request->getRelativePath());
	}
	return l_self;
}

void space::write(persistenceRequest<space>* p_persist_request)
{
	//Grab space pointer from the persist request.
	std::shared_ptr<space> l_space = p_persist_request->getData();
	typeRT rt_space = l_space->toTypeRT();
	const std::string l_data = jsonSerialization::typeToStream(rt_space);
	p_persist_request->writeDataToFile(l_data);

	//Loop over all of the game objects in the current space and create write requests for each.
	for (const auto & game_object : l_space->getTopLevelGameObjectList())
	{
		const std::string &file_name = objIDtoString(game_object->getID()) + ".toy";
		persistenceRequest<gameObject>(Direction::Write, "gameObjects/", file_name, game_object).execute(p_persist_request->getRelativePath());
	}
}
