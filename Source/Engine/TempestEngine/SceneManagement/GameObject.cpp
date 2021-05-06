/*!***************************************************************************************
\file       GameObject.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the GameObject class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "GameObject.hpp"

//========1st Party Includes============================================================//
#include "Transform.hpp"
#include "../Serialization.h"

//========3rd Party Includes============================================================//
#include <algorithm>
#include <filesystem>
#include <ComponentFactory.hpp>

////////==========================================================================////////
////////  Class(gameObject)                                                       ////////
////////==========================================================================////////

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
std::shared_ptr<gameObject> gameObject::read(persistenceRequest<gameObject>* p_persist_request)
{
	typeRT l_data = p_persist_request->readDataFromFile();
	auto l_self = p_persist_request->getData();

	// read id and name
	l_self->m_id = l_data.member("id").getObjID();
	l_self->m_name = l_data.getVariableName();

	// deserialize components
	componentFactory::deserializeComponents(l_self, l_data);

	// deserialize child game objects
	for(auto i_children : l_data.member("childList").array())
	{
		auto new_gameObject = p_persist_request->getData()->getSpace()->instantiateGameObject();
		std::string file_name = objIDtoString(i_children.getObjID());
		persistenceRequest<gameObject> l_child_request(Direction::Read, p_persist_request->getFolderName(), file_name + ".toy", new_gameObject);
		l_child_request.execute(p_persist_request->getRelativePath());
		p_persist_request->getData()->addChild(l_child_request.getData());
	}

	return l_self;

}
void gameObject::write(persistenceRequest<gameObject>* p_persist_request)
{
	for(auto i_children: children)
	{
		persistenceRequest<gameObject> l_child_request(Direction::Write, p_persist_request->getFolderName(),objIDtoString(i_children->getID()) + ".toy" , i_children);
		l_child_request.execute(p_persist_request->getRelativePath());
	}

	typeRT rt_gameObject = toTypeRT(p_persist_request->getData());
	p_persist_request->writeDataToFile(jsonSerialization::typeToStream(rt_gameObject));
}
////==========================================================================////
////      Virtual                                                             ////
////==========================================================================////

//==============================================================================//
//        Operators                                                             //
//==============================================================================//

std::string gameObject::getName() const
{
	return m_name;
}

void gameObject::setName(std::string p_new_name)
{
	m_name = p_new_name;
}


void gameObject::applyFromTypeRT(typeRT & p_type)
{
	// get components from the new typert
	auto l_changed_components = p_type.member("components").array();

	for(auto l_iter = l_changed_components.begin(); l_iter != l_changed_components.end(); ++l_iter)
	{
		//Find the component by componentType->has a string in it.
		auto l_component = component_list.getComponentByString(l_iter->getTypeName());
		//If it is attached to this gameObject, change it
		if(l_component)
		{
			(l_component)->updateFromTypeRT(*l_iter);
		}
		else
		{
				componentFactory::addComponentByType(shared_from_this(), l_iter->getTypeName());
				(component_list.getComponentByString(l_iter->getTypeName()))->updateFromTypeRT(*l_iter);
		}
	}
}

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//==============================================================================//
//        Helper                                                                //
//==============================================================================//

void gameObject::addChild(std::shared_ptr<gameObject> p_new_child)
{
	componentHandle<transform> l_child_trans = p_new_child->getComponent<transform>();
	componentHandle<transform> l_parent_trans = this->getComponent<transform>();

	// get the child global transform so we can restore it after parenting
	matrix4x4 l_cgm = l_child_trans->getGlobalMatrix();
	//vector3 l_child_global_trans = l_child_trans->getGlobalPosition();
	//quaternion l_child_global_rot = l_child_trans->getGlobalRotation();
	//vector3 l_child_global_scale = l_child_trans->getGlobalScale();

	// move child into same space as parent, if not already there
	p_new_child->getSpace()->moveGameObject(p_new_child, getSpace());

	// make child
	children.emplace_back(p_new_child);
	p_new_child->parent = shared_from_this();
	l_child_trans->markDirty();

	// The math:
	//   We want the child-global-matrix (CGM) to remains constant
	//   The parent-global-matrix (PGM) is constant
	//   We need to calculate the proper child-local-matrix (CLM) that makes the CGM remain constant
	//
	//   So, normally the child-global-matrix is calculated as: CGM = CLM * PGM
	//   Thus, CGM * PGM^-1 = CLM

	// update transform of the child for new global reference
	matrix4x4 l_clm = l_cgm * l_parent_trans->getGlobalMatrix().inverse();
	l_child_trans->fromLocalMatrix(l_clm);
	//vector4 l_child_local_trans_val = l_parent_trans->getGlobalMatrix().inverse() * vector4(l_child_global_trans, 1); // PGM^-1 * CGP = CLP
	//l_child_trans->setPosition( vector3(l_child_local_trans_val.x, l_child_local_trans_val.y, l_child_local_trans_val.z) );
	//l_child_trans->setRotation(l_child_global_rot * l_parent_trans->getGlobalRotation().Conjugated()); // CGR * PGR^-1 = CLR
	//l_child_trans->setScale(l_child_global_scale / l_parent_trans->getGlobalScale()); // CGS * PGS^-1 = CLS
}

void gameObject::removeChild(std::shared_ptr<gameObject> p_old_child)
{
	auto l_location = std::find(children.begin(), children.end(), p_old_child);
	if (l_location != children.end())
	{
//auto l_parent_transform = getComponent<transform>();
//auto l_child_transform = (*l_location)->getComponent<transform>();

//Get them before we start updating them
//vector3 l_childPosition = l_parent_transform->getGlobalPosition() + l_child_transform->getPosition();
//quaternion l_childRotation = l_parent_transform->getGlobalRotation() + l_child_transform->getRotation();
//vector3 l_childScale = l_parent_transform->getGlobalScale() + l_child_transform->getScale();

		// clean up the child's parent pointer
		(*l_location)->parent.reset();

// update child globals
//l_child_transform->setPosition(l_childPosition);
//l_child_transform->setRotation(l_childRotation);
//l_child_transform->setScale(l_childScale);

		// remove the child from the children list
		children.erase(l_location);
	}
}

std::vector<baseComponentHandle> gameObject::getAllComponents()
{
	return component_list.getAllComponents();
}

std::vector<baseComponentHandle> gameObject::getAllComponents() const
{
	return component_list.getAllComponents();
}

bool gameObject::hasParent() const
{
	if (parent.lock() == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::weak_ptr<gameObject> gameObject::getParent() const
{
	return parent;
}

void gameObject::detatchFromParent()
{
	if(!parent.expired())
	{
		parent.lock()->removeChild(shared_from_this());
	}
}

bool gameObject::isTopLevelObject() const
{
	return getParent().expired();
}

std::vector<std::shared_ptr<gameObject>> gameObject::getChildren()
{
	return children;
}

space* gameObject::getSpace()
{
	return containing_space;
}

GameObjectFiltering::componentTypeSet gameObject::getComponentTypeSet() const
{
	return component_list.getComponentTypeSet();
}

void gameObject::RemoveAllComponents()
{
	// setup the list of components to remove
	auto compList = this->component_list.getAllComponents();
	for(int i = 0, size = compList.size(); i < size; ++i)
	{
		// remove the one transform component from our list of components to remove.
		if((compList[i])->type() == transform::getType())
		{
			std::swap(compList[i], compList[size - 1]);
			compList.pop_back();
			break;
		}
	}
	
	// iterate all components, cleaning up and removing only non-transform ones
	for (int i = 0, size = compList.size(); i < size; ++i)
	{
		// clean out the component memory
		DEALLOCATE(getSpace()->getScene()->getGameObjectCollector()->getComponentRepository(),
					compList[i]);

		this->component_list.removeComponent(compList[i]);
	}
}

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//



gameObject::~gameObject()
{
	children.clear();
	parent.reset();
}

std::shared_ptr<gameObject> gameObject::cloneObject()
{
	return getSpace()->instantiateGameObject(toTypeRT(shared_from_this()));
}

bool gameObject::markedForDeletion() const
{
    return shouldDelete;
}

void gameObject::markForDeletion()
{
    shouldDelete = true;
}

gameObject::gameObject(space * p_space) : shouldDelete(false), m_id{getNextID() }, m_name{ "Unnamed_game_object" }, containing_space { p_space }
{
	component_list.setParentGameObject(this);
	parent.reset();
}

