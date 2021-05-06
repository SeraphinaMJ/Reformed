/*!***************************************************************************************
\file       Scene.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the Scene class member functions
*****************************************************************************************/

//========Self Include==================================================================//
#include "Scene.hpp"

//========1st Party Includes============================================================//
#include "../Serialization.h"
#include "../PersistenceRequest.hpp"
#include "../External/CameraManipulator.hpp"

//========3rd Party Includes============================================================//
#include <memory>
#include <sstream>

////////==========================================================================////////
////////  Class(scene)                                                            ////////
////////==========================================================================////////

	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		std::shared_ptr<scene> scene::read(persistenceRequest<scene>* p_persist_request)
		{
			typeRT l_data = p_persist_request->readDataFromFile();
			auto l_self = p_persist_request->getData();
			l_self->m_id = l_data.member("id").getObjID();
			l_self->m_name = l_data.getVariableName();

			for(const auto& i_spaces: l_data.member("spaces").array())
			{
				auto new_space = l_self->createSpace();
				std::string space_id = objIDtoString(i_spaces.member("id").getObjID());
				persistenceRequest<space> l_request(Direction::Read, "spaces/", space_id + ".spa", new_space);
				l_request.execute(p_persist_request->getRelativePath());
			}
			return l_self;
		}

		void scene::write(persistenceRequest<scene>* p_persist_request)
		{
			typeRT rt_scene = p_persist_request->getData()->toTypeRT();
			const std::string l_data = jsonSerialization::typeToStream(rt_scene);
			p_persist_request->writeDataToFile(l_data);

			//Write all spaces
			for (const auto& i_space : p_persist_request->getData()->getSpaces())
			{
        if(i_space->getShouldSerialize())
        {
          persistenceRequest<space>(Direction::Write, "spaces/", objIDtoString(i_space->getID()) + ".spa", i_space).execute(p_persist_request->getRelativePath());
        }
			}
		}
		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Operators                                                                     //
		//======================================================================================//


		typeRT scene::toTypeRT() const
		{
			typeRT result;
			result.setTypeName("scene");
			result.setVariableName(m_name);

			std::vector<typeRT> p_spaces_as_type_run_time;

			for (const auto& i_space : spaces)
				{
          if(i_space->getShouldSerialize())
          {
            typeRT current_spaceRT(i_space->getName());
            current_spaceRT.setTypeName("space");
            current_spaceRT.insertMember({ typeRT("id", i_space->getID()) });
            p_spaces_as_type_run_time.push_back(current_spaceRT);
          }
				}

			typeRT spacesRT(p_spaces_as_type_run_time);
			spacesRT.setTypeName("spaces");
			spacesRT.setVariableName("spaces");

			result.insertMember(spacesRT);

			result.insertMember(typeRT("id", getID()));


			return result;
		}

std::shared_ptr<space> scene::createSpace()
{
    std::stringstream l_name;
    l_name << "Unnamed_space_" << spaces.size();
    std::shared_ptr<space> result = std::shared_ptr<space>(new space(this, l_name.str()));
    spaces.push_back(result); // add a default space to the back of the list
    return result;  // return the newly created space
}

std::shared_ptr<space> scene::createSpace(const std::string& p_name)
{
	std::shared_ptr<space> result = std::shared_ptr<space>(new space(this, p_name));
	spaces.push_back(result); // add a default space to the back of the list
	return result;  // return the newly created space
}

void scene::moveSpace(const std::shared_ptr<space> & p_space)
{
	p_space->getScene()->destroySpace(p_space);
	spaces.push_back(p_space);
    p_space->setScene(this);
}

void scene::destroySpace(std::shared_ptr<space> const & p_old_space)
{
    spaces.erase(std::remove(spaces.begin(), spaces.end(), p_old_space), spaces.end());
}

const std::vector<std::shared_ptr<space>> & scene::getSpaces() const
{
	return spaces;
}

std::shared_ptr<space> scene::getSpace(std::string p_name) const
{
	for(const auto& i_space : spaces)
		if (i_space->getName() == p_name)
			return i_space;
	return {};
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

scene::scene(gameObjectCollector* p_new_collector, std::string p_name)
    : m_id{getNextID()}, m_name{p_name}, go_collector{p_new_collector},
	editor_camera_space(this, "editor_camera_space")
		{
			editor_camera_object = editor_camera_space.instantiateGameObject();
			editor_camera_object->addComponent<editorCamera>();
		}

scene::~scene()
{
	spaces.clear();
}

void scene::setGameObjectCollector(gameObjectCollector* p_new_collector)
{
    go_collector = p_new_collector;
}

gameObjectCollector* scene::getGameObjectCollector() const
{
    return go_collector;
}

std::shared_ptr<space> scene::getSpaceByIndex(size_t p_index) const
{
    return spaces[p_index];
}

std::shared_ptr<gameObject> scene::findGameObject(std::string p_name) const
{
	// check each space
	for(auto l_space : spaces)
	{
		// check each object
		for(auto l_object : l_space->getGameObjectList())
		{
			if (l_object->getName() == p_name)
				return l_object;
		}
	}
	return nullptr;
}

std::shared_ptr<gameObject> scene::findGameObjectByID(objID p_id) const
{
	// check each space
	for (auto l_space : spaces)
	{
		auto l_gameObject = l_space->getGameObjectByID(p_id);
			if (l_gameObject != nullptr)
				return l_gameObject;
	}

	return nullptr;
}

void scene::destroyGameObject(std::string p_name)
{
	std::shared_ptr<gameObject> l_object = findGameObject(p_name);
	if(l_object != nullptr)
	{
		l_object->getSpace()->destroyGameObject(l_object);
	}
}

void scene::destroyGameObjectByID(objID p_ID)
{
	std::shared_ptr<gameObject> l_object = findGameObjectByID(p_ID);
	l_object->getSpace()->destroyGameObject(l_object);
}

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>

