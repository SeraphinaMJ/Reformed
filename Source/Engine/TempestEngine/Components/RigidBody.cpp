/*!***************************************************************************************
\file       RigidBody.cpp
\author     Cody Cannell
\date       8/9/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Defines the rigid body of the object
*****************************************************************************************/
#include "RigidBody.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "../SceneManagement/Transform.hpp"
#include <Vector3Serialization.hpp>

rigidBody::rigidBody(typeRT& p_type)
{
	updateFromTypeRT(p_type);
}
#include <iostream>
rigidBody::rigidBody()
{
  
}

rigidBody::~rigidBody()
{
  int l_group = 0, l_mask = 0;
  if (m_handle)
  {
    bulletWrapper::removeFromSimulation(m_handle, l_group, l_mask);
  }
}

typeRT rigidBody::toTypeRT() const
{
    typeRT l_result;
    l_result.setTypeName(getType().name());
    l_result.setVariableName(getType().name());

	std::vector<typeRT> l_members;

    typeRT l_collisionShape("collisionShape", static_cast<int>(m_collisionShape));
    l_members.push_back(l_collisionShape);

    typeRT l_mass("mass", m_mass);
    l_members.push_back(l_mass);

    typeRT l_localInertia = ::toTypeRT(m_localInertia);
    l_localInertia.setVariableName("localInertia");
    l_members.push_back(l_localInertia);

    typeRT l_handle("handle", m_handle);
    l_members.push_back(l_handle);

    typeRT l_localGravity = ::toTypeRT(m_localGravity);
    l_localGravity.setVariableName("localGravity");
    l_members.push_back(l_localGravity);

    typeRT l_useGlobalGravity("useGlobalGravity", m_useGlobalGravity);
    l_members.push_back(l_useGlobalGravity);

    typeRT l_restitution("restitution",m_restitution);
    l_members.push_back(l_restitution);

    typeRT l_friction("friction", m_friction);
    l_members.push_back(l_friction);

    typeRT l_damping("damping", m_damping);
    l_members.push_back(l_damping);

    typeRT l_parent("parent", m_parent);
    l_members.push_back(l_parent);

    //TODO: Conversion error from collisionLayer to int?
    l_members.push_back(typeRT("collisionLayerSelf", m_self));
    l_members.push_back(typeRT("collisionLayerWith", m_collidesWith));

	for(auto i_member: l_members)
	{
    l_result.insertMember(i_member);
	}

    return l_result;
}

void rigidBody::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

  //Use setters

	if(l_members.find("collisionShape") != l_members.end())
	{
    unsigned int l_data = p_type.member("collisionShape").getUInt();

    if(l_data >= static_cast<unsigned int>(collisionShape::enm_circle) && l_data < static_cast<unsigned int>(collisionShape::enm_count))
      setCollisionShape(static_cast<collisionShape>(l_data));
	  //m_collisionShape = static_cast<collisionShape>(p_type.member("collisionShape").getUInt());
	}
	if (l_members.find("mass") != l_members.end())
	{
    setMass(p_type.member("mass").getFloat());
		//m_mass = p_type.member("mass").getFloat();
	}
	if (l_members.find("localInertia") != l_members.end())
	{
    //No setter for this at the moment
		m_localInertia = ::toVector3(p_type.member("localInertia"));
	}
	if (l_members.find("handle") != l_members.end())
	{
    //Neither of these should ever be called in theory. Disabling for now. Contact Ryan if questioning

    //setHandle(p_type.member("mass").getFloat());
		//m_handle = p_type.member("handle").getInt();
	}
  if(l_members.find("localGravity") != l_members.end())
  {
    setLocalGravity(::toVector3(p_type.member("localGravity")));
  }
  if (l_members.find("useGlobalGravity") != l_members.end())
  {
    useGlobalGravity(p_type.member("useGlobalGravity").getBool());
  }
  if (l_members.find("restitution") != l_members.end())
  {
    setRestitution(p_type.member("restitution").getFloat());
  }
  if (l_members.find("friction") != l_members.end())
  {
    setFriction(p_type.member("friction").getFloat());
  }
  if (l_members.find("damping") != l_members.end())
  {
    setDamping(p_type.member("damping").getFloat());
  }
  if (l_members.find("parent") != l_members.end())
  {
    //setParent(p_type.member("parent").getObjID());
  }
  //TODO: Conversion error from collisionLayer to int?

  m_self = 0;
  m_collidesWith = 0;

  if (l_members.find("collisionLayerSelf") != l_members.end())
  {
    m_self = (p_type.member("collisionLayerSelf").getUInt());
  }

  if (l_members.find("collisionLayerWith") != l_members.end())
  {
    m_collidesWith = (p_type.member("collisionLayerWith").getUInt());
  }

  setCollisionLayer(m_self, m_collidesWith);
}

vector3 rigidBody::getVelocity()
{
  btVector3 l_result = bulletWrapper::getBTVelocity(m_handle);

  vector3 l_velocity(l_result.getX(), l_result.getY(), l_result.getZ());

  return l_velocity;
}

void rigidBody::setVelocity(vector3 p_velocity)
{
  bulletWrapper::setBTVelocity(m_handle, btVector3(p_velocity.x, p_velocity.y, p_velocity.z));
}

void rigidBody::useGlobalGravity(bool p_useGlobalGravity)
{
  //Turning on Global Gravity
  if (!m_useGlobalGravity && p_useGlobalGravity && (m_handle != nullptr))
  {
    bulletWrapper::setBTGravity(m_handle, bulletWrapper::getBTGlobalGravity());
  }
  //Turning off Global Gravity, turning on Local Gravity
  else if(m_useGlobalGravity && !p_useGlobalGravity && (m_handle != nullptr))
  {
    bulletWrapper::setBTGravity(m_handle, btVector3(m_localGravity.x, m_localGravity.y, m_localGravity.z));
  }

  m_useGlobalGravity = p_useGlobalGravity;
}

void rigidBody::setCollisionLayer(unsigned int p_self, unsigned int p_collidesWith)
{
  m_self = p_self;
  m_collidesWith = p_collidesWith;

  if(m_handle != nullptr)
  {
    bulletWrapper::setBTCollisionLayer(m_handle, m_self, m_collidesWith);
  }
}

objID rigidBody::getParent()
{
  return m_parent;
}

void rigidBody::setMass(float p_mass)
{
  if(m_handle != nullptr)
  {
    bulletWrapper::setBTMass(m_handle, p_mass);
  }

  m_mass = p_mass;
}

void rigidBody::setRestitution(float p_restitution)
{
  if(m_handle != nullptr)
  {
    bulletWrapper::setBTRestitution(m_handle, p_restitution);
  }

  m_restitution = p_restitution;
}

void rigidBody::setFriction(float p_friction)
{
  if(m_handle != nullptr)
  {
    bulletWrapper::setBTFriction(m_handle, p_friction);
  }

  m_friction = p_friction;
}

void rigidBody::setDamping(float p_damping)
{
  if (m_handle != nullptr)
  {
    bulletWrapper::setBTDamping(m_handle, p_damping);
  }

  m_damping = p_damping;
}

void rigidBody::setParent(objID p_parentObject)
{
  if(getGameObject().lock() != nullptr)
  {
    //Don't allow parenting to yourself
    if (p_parentObject != getGameObject().lock()->getID())
    {
      m_parent = p_parentObject;
    }
  }
  else
  {
    m_parent = p_parentObject;
  }
}

bool rigidBody::isGrounded()
{
  auto l_gameObject = getGameObject().lock();
  vector3 l_position = l_gameObject->getComponent<transform>()->getGlobalPosition();
  vector3 l_feet = l_position;
  l_feet.y = l_position.y - (l_gameObject->getComponent<transform>()->getScale().y);
  btCollisionObject * l_handle = nullptr;
  int l_collidesWith = static_cast<collisionLayer>(collisionLayer::enm_world /*| collisionLayer::enm_enemy*/);

  return rayCast(l_position, l_feet, l_collidesWith, l_handle);
}

bool rigidBody::rayCast(vector3 p_start, vector3 p_end, int p_collidesWith, btCollisionObject * & p_handle)
{
  btVector3 l_normal;
  btVector3 l_end = btVector3(p_end.x, p_end.y, p_end.z);
  return bulletWrapper::checkCollision(m_handle, btVector3(p_start.x, p_start.y, p_start.z), l_end, l_normal, p_collidesWith, p_handle);
}

void rigidBody::applyImpulse(const vector3 & p_force)
{
  bulletWrapper::addCentralImpulseToObject(btVector3(p_force.x, p_force.y, p_force.z), m_handle);

  //Parenting in here?
  //TODO: FORCES WITH DIFFERENT MASSES WILL PROBS NOT WORK
  auto l_gameObject = getGameObject().lock();

  auto l_children = l_gameObject->getChildren();

  if (l_children.size() != 0)
  {
	  for(size_t i = 0; i < l_children.size(); ++i)
	  {
		  componentHandle<rigidBody> l_childBody = l_children[i]->getComponent<rigidBody>();
			//Move it also
		  if(l_childBody)
		  {
	        l_childBody->applyImpulse(p_force);
		  }
	  }
  }
}

objID rigidBody::getObjectRayCast(const vector3 & p_start, const vector3 & p_end, int p_collidesWith, btCollisionObject * & p_handle)
{
  return rayCast(p_start, p_end, p_collidesWith, p_handle);
}

void rigidBody::setActivationState(int p_state)
{
  bulletWrapper::setActivationState(m_handle, p_state);
}

void rigidBody::updateBulletTransform()
{
  auto l_gameObject = getGameObject().lock();
  auto l_transform = l_gameObject->getComponent<transform>();

  vector3 l_tempestPosition = l_transform->getGlobalPosition();
  btVector3 l_position(l_tempestPosition.x, l_tempestPosition.y, l_tempestPosition.z);

  quaternion l_tempestRotation = l_transform->getGlobalRotation();
  btQuaternion l_rotation(l_tempestRotation.x, l_tempestRotation.y, l_tempestRotation.z, l_tempestRotation.w);

  vector3 l_tempestScale = l_transform->getGlobalScale();
  btVector3 l_scale(l_tempestScale.x, l_tempestScale.y, l_tempestScale.z);

  bulletWrapper::updateTransform(m_handle, l_position, l_rotation, l_scale);
}

void rigidBody::applyForce(const vector3& p_force)
{
  bulletWrapper::addCentralForceToObject(btVector3(p_force.x, p_force.y, p_force.z), m_handle);

  //Parenting in here?
  //TODO: FORCES WITH DIFFERENT MASSES WILL PROBS NOT WORK
  auto l_gameObject = getGameObject().lock();

  auto l_children = l_gameObject->getChildren();

  if (l_children.size() != 0)
  {
    for (size_t i = 0; i < l_children.size(); ++i)
    {
      componentHandle<rigidBody> l_childBody = l_children[i]->getComponent<rigidBody>();
      //Move it also
      if (l_childBody)
      {
        l_childBody->applyForce(p_force);
      }
    }
  }
}
