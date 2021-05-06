/*!***************************************************************************************
\file       BulletWrapper.cpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

//=========================================================//
//                   BulletWrapper                         //
//=========================================================//
#include "BulletWrapper.hpp"
#include <Logger.hpp>
#include <algorithm>
#include "../../Engine/TempestEngine/Physics/PhysicsSystem.hpp"

btDefaultCollisionConfiguration* bulletWrapper::m_collisionConfiguration = nullptr;
btCollisionDispatcher* bulletWrapper::m_dispatcher;
btDbvtBroadphase* bulletWrapper::m_broadphase;
btSequentialImpulseConstraintSolver* bulletWrapper::m_solver;
btDiscreteDynamicsWorld* bulletWrapper::m_dynamicsWorld;
btAlignedObjectArray<btCollisionShape*> bulletWrapper::m_collisionShapes;
std::vector<btRigidBody*> bulletWrapper::m_bodies;

float bulletWrapper::m_time_step;
bool bulletWrapper::m_isRunning;
systemManagerInterface * bulletWrapper::m_systemManager;

// TODO
/*
static bool collisionFlag = false;
static int count = 0;
*/

btCollisionShape* bulletWrapper::createCollisionShape(const int p_shape, btVector3 p_scale)
{
    switch (p_shape)
    {
    case 0 /*collisionShape::enm_circle*/:
        //Parameter is radius, potentially need to change this from just using X
        return new btSphereShape(p_scale.getX());
        break;
    case 1 /*collisionShape::enm_square*/:
        return new btBoxShape(btVector3(btScalar(1) * p_scale.getX(), btScalar(1)* p_scale.getY(), btScalar(1)* p_scale.getZ()));
        break;
    case 2 /*collisionShape::enm_plane*/:
        return new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
        break;
    }

    return nullptr;
}

bool collisionCallback(btManifoldPoint& collitionPoint, const btCollisionObjectWrapper * obj1, int id1, int index1, const btCollisionObjectWrapper * obj2, int id2, int index2)
{
    UNREFERENCED_PARAMETER(collitionPoint);
    UNREFERENCED_PARAMETER(obj1);
    UNREFERENCED_PARAMETER(id1);
    UNREFERENCED_PARAMETER(index1);
    UNREFERENCED_PARAMETER(obj2);
    UNREFERENCED_PARAMETER(id2);
    UNREFERENCED_PARAMETER(index2);

  /*  collisionFlag = true;
    count++;
    */

   //logger("logger").debug() << "________________Collision Happened________________";
    return true;
}

void collisionStartedCallback(btPersistentManifold* const & p_manifold)
{
  //contact point, object collided with, normal

  systemManagerInterface* l_system_manager = bulletWrapper::getBTSystemManager();
  //0 for CollisionStarted
  l_system_manager->getSystem<physicsSystem>()->processCallback(p_manifold, 0);

}

void collisionEndedCallback(btPersistentManifold* const & p_manifold)
{
  systemManagerInterface* l_system_manager = bulletWrapper::getBTSystemManager();
  //1 for CollisionEnded
  l_system_manager->getSystem<physicsSystem>()->processCallback(p_manifold, 1);
}

bulletWrapper::~bulletWrapper()
{
  //Only want to do this once (memory pool and stack allocation lives in here
  delete m_collisionConfiguration;
  m_collisionConfiguration = nullptr;
}

void bulletWrapper::createSimulator(const btVector3 & p_globalGravity, systemManagerInterface * p_systemManager)
{
    m_systemManager = p_systemManager;
    //gContactAddedCallback = collisionCallback;
    gContactStartedCallback = collisionStartedCallback;
    gContactEndedCallback = collisionEndedCallback;

    if(m_collisionConfiguration == nullptr)
    {
      m_collisionConfiguration = new btDefaultCollisionConfiguration();
    }

    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(p_globalGravity);
    m_time_step = (1.0f / 60.0f);
    //m_dynamicsWorld->getDispatchInfo().m_useContinuous = true;
    m_isRunning = true;
}

btCollisionObject * bulletWrapper::addPhysicsObject(float p_mass, btVector3 p_scale, btVector4 p_position, btQuaternion p_rotation, btVector3 p_localInertia, const btVector3 & p_localGravity, float p_restitiution, float p_friction, float p_damping, const int p_shape, const int p_collisionLayerSelf, const int p_collisionLayerCollidesWith)
{
    if(!m_isRunning)
    {
      return nullptr;
    }

    // only make one of each type of shape. posibly send in dimentions
    btCollisionShape* l_boxShape = createCollisionShape(p_shape, p_scale);

	l_boxShape->setMargin(0.1f);

    btScalar l_mass = btScalar(p_mass);

    btVector3 l_localInertia = p_localInertia;

    if (l_mass)
        l_boxShape->calculateLocalInertia(l_mass, l_localInertia);

    btDefaultMotionState* l_motionstate = new btDefaultMotionState(btTransform(
        p_rotation,
        btVector3(p_position.getX(), p_position.getY(), p_position.getZ())
    ));


    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        l_mass, // mass, in kg. 0 -> Static object, will never move.
        l_motionstate,
        l_boxShape, // collision shape of body
        l_localInertia // local inertia
    );

    rigidBodyCI.m_restitution = p_restitiution;
    rigidBodyCI.m_friction = p_friction;
    rigidBodyCI.m_linearDamping = p_damping;
    rigidBodyCI.m_linearSleepingThreshold = 4.9f;
    rigidBodyCI.m_angularSleepingThreshold = 10.0f;


    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

    //Set flags for collision callback
    //rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    //https://gamedev.stackexchange.com/questions/11961/how-can-i-enable-ccd-in-bullet-physics
    //No idea what this first value should be (scale of the game)
    rigidBody->setCcdMotionThreshold(50.0f);
    rigidBody->setCcdSweptSphereRadius(0.2f);

    //m_dynamicsWorld->addRigidBody(rigidBody);
    m_dynamicsWorld->addRigidBody(rigidBody, p_collisionLayerSelf, p_collisionLayerCollidesWith);

    //Have to do this after https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=5038
    rigidBody->setGravity(p_localGravity);

    m_bodies.push_back(rigidBody);

    return rigidBody;
}

void bulletWrapper::updatePhysicsWorld()
{
  if (!m_isRunning)
  {
    return;
  }

  m_dynamicsWorld->stepSimulation(m_time_step);

  //Do collision detection here
  //int l_numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
  ////Have to loop over all collisions, no bullet functions exist for onEnter and onEnd
  //https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=3997
  //https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11507
  //for(int i = 0; i < l_numManifolds; ++i)
  //{
  //
  //}
}

void bulletWrapper::addCentralImpulseToObject(btVector3 p_force, btCollisionObject * p_handle)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
    if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
    {
        btCollisionObject * obj = p_handle;
        btRigidBody * body = btRigidBody::upcast(obj);

        body->forceActivationState(DISABLE_DEACTIVATION);

        body->applyCentralImpulse(p_force);
    }

}

void bulletWrapper::addCentralForceToObject(btVector3 p_force, btCollisionObject * p_handle)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    body->forceActivationState(DISABLE_DEACTIVATION);

    body->applyCentralForce(p_force);
  }

}

btVector4 bulletWrapper::updatePhysicsObject(btCollisionObject * p_handle, btQuaternion & p_rotation)
{
  if (!m_isRunning)
  {
    return btVector4();
  }

    btTransform trans;
    //logger l_logger("Physics");
    auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();

    // check to see if handle is in world
    if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
    {
        btCollisionObject * obj = p_handle;
        btRigidBody * body = btRigidBody::upcast(obj);

        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }
    }

    p_rotation = trans.getRotation();

    return btVector4(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()),
        1);
}

btVector3 bulletWrapper::getBTVelocity(btCollisionObject * p_handle)
{
  if (!m_isRunning)
  {
    return btVector3();
  }

  btVector3 l_result;
  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    l_result = body->getLinearVelocity();
  }

  return l_result;
}

void bulletWrapper::setBTVelocity(btCollisionObject * p_handle, const btVector3 & p_velocity)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

	  body->forceActivationState(DISABLE_DEACTIVATION);
    body->setLinearVelocity(p_velocity);
  }
}

bool bulletWrapper::getBTUsingGravity(btCollisionObject * p_handle)
{
  if (!m_isRunning)
  {
    return false;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  btVector3 l_result;
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    l_result = body->getGravity();

    if (l_result.getY() != 0 || l_result.getX() != 0 || l_result.getZ() != 0)
      return true;
  }

  return false;
}

btVector3 bulletWrapper::getBTGlobalGravity()
{
  if (!m_isRunning)
  {
    return btVector3();
  }

  return m_dynamicsWorld->getGravity();
}

void bulletWrapper::setBTGlobalGravity(btVector3 p_gravity)
{
  if (!m_isRunning)
  {
    return;
  }

  m_dynamicsWorld->setGravity(p_gravity);
}

btVector3 bulletWrapper::getBTLocalGravity(btCollisionObject * p_handle)
{
  if (!m_isRunning)
  {
    return btVector3();
  }

  btVector3 l_result;
  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    l_result = body->getGravity();
  }

  return l_result;
}

void bulletWrapper::setBTGravity(btCollisionObject * p_handle, const btVector3 & p_gravity)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    //Remove the rigid body to the dynamics world
    //removeFromSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 0);

    body->setGravity(p_gravity);

    //Add the rigid body to the dynamics world
    //addToSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 1);
  }
}

void bulletWrapper::setBTCollisionLayer(btCollisionObject * p_handle, int p_collisionSelf, int p_collisionWith)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody* body = btRigidBody::upcast(obj);

    //Remove from simulation
    int l_prev, l_old;
    removeFromSimulation(p_handle, l_prev, l_old);
    //Add back to world with new layers
    m_dynamicsWorld->addRigidBody(body, p_collisionSelf, p_collisionWith);
  }
}

void bulletWrapper::setActivationState(btCollisionObject * p_handle, int p_state)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody* body = btRigidBody::upcast(obj);

    //https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11047

    switch (p_state)
    {
    case 0:
      {
      body->setActivationState(DISABLE_SIMULATION);
      body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
      break;
      }
    case 1:
      {
      body->setActivationState(ACTIVE_TAG);
      body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
      break;
      }
    default :
      {
      break;
      }
    }
  }
}

void bulletWrapper::removeFromSimulation(btCollisionObject* p_handle, int & p_previousGroup, int & p_previousMask)
{
  if (!m_isRunning)
  {
    return;
  }

  if(p_handle != nullptr && m_isRunning)
  {
    btRigidBody* body = btRigidBody::upcast(p_handle);
    p_previousGroup = p_handle->getBroadphaseHandle()->m_collisionFilterGroup;
    p_previousMask = p_handle->getBroadphaseHandle()->m_collisionFilterMask;
    m_dynamicsWorld->removeRigidBody(body);
  }
}

void bulletWrapper::addToSimulation(btCollisionObject* p_handle, int p_previousGroup, int p_previousMask)
{
  if (!m_isRunning)
  {
    return;
  }

  if(p_handle != nullptr && m_isRunning)
  {
    btRigidBody* body = btRigidBody::upcast(p_handle);
    m_dynamicsWorld->addRigidBody(body);
    body->getBroadphaseHandle()->m_collisionFilterGroup = p_previousGroup;
    body->getBroadphaseHandle()->m_collisionFilterMask = p_previousMask;
  }
}

void bulletWrapper::setTimeStep(float p_dt)
{
  m_time_step = p_dt;
}

void bulletWrapper::deleteObjectFromSimulation(btCollisionObject* p_handle)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();

  if(l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    m_dynamicsWorld->removeCollisionObject(p_handle);
  }
}

systemManagerInterface* bulletWrapper::getBTSystemManager()
{
  if (!m_isRunning)
  {
    return nullptr;
  }

  return m_systemManager;
}

void bulletWrapper::setIsRunning(bool p_running)
{
  m_isRunning = p_running;
}

bool bulletWrapper::castRayIntoScene(btVector3& p_start, btVector3& p_end, btVector3& p_normal, btCollisionObject * & p_collidedHandle, collisionLayer p_collides_with)
{
	btCollisionWorld::ClosestRayResultCallback l_rayCallBack(p_start, p_end);

	int layer = enm_world; //| enm_world | enm_player | enm_enemy | enm_trigger;
  l_rayCallBack.m_collisionFilterGroup = p_collides_with;
	//l_rayCallBack.m_collisionFilterMask = p_collides_with /*| enm_player | enm_enemy | enm_trigger*/;
  l_rayCallBack.m_collisionFilterMask = p_collides_with;


	m_dynamicsWorld->rayTest(p_start, p_end, l_rayCallBack);

	if (l_rayCallBack.hasHit())
	{
		//Store results
		p_end = l_rayCallBack.m_hitPointWorld;
		p_normal = l_rayCallBack.m_hitNormalWorld;

    btCollisionObject * l_object = const_cast<btCollisionObject*>(l_rayCallBack.m_collisionObject);
    p_collidedHandle = l_object;

		return true;
	}

	return false;
}

bool bulletWrapper::checkCollision(btCollisionObject * p_handle, const btVector3 & p_start, btVector3 & p_end, btVector3 & p_normal, int p_collidesWith, btCollisionObject * & p_collidedHandle)
{
	if (!m_isRunning)
	{
		return false;
	}
	auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
	// check to see if handle is in world
	if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
	{
		btCollisionObject * obj = p_handle;
		btCollisionWorld::ClosestRayResultCallback l_rayCallBack(p_start, p_end);
		l_rayCallBack.m_collisionFilterGroup = obj->getBroadphaseHandle()->m_collisionFilterGroup;
		l_rayCallBack.m_collisionFilterMask = p_collidesWith;
		m_dynamicsWorld->rayTest(p_start, p_end, l_rayCallBack);
		if (l_rayCallBack.hasHit())
		{
			//Store results
			p_end = l_rayCallBack.m_hitPointWorld;
			p_normal = l_rayCallBack.m_hitNormalWorld;
			btRigidBody * l_body = const_cast<btRigidBody*>(btRigidBody::upcast(l_rayCallBack.m_collisionObject));
			btCollisionObject * l_object = const_cast<btCollisionObject*>(l_rayCallBack.m_collisionObject);
			//Find the collided object
			if (l_objectArray.findLinearSearch(l_object) != l_objectArray.size())
			{
				p_collidedHandle = l_object;
				return true;
			}
		}
	}
	//For ray casting with no rigidbody
	else if (p_handle == nullptr)
	{
		btCollisionWorld::ClosestRayResultCallback l_rayCallBack(p_start, p_end);
		l_rayCallBack.m_collisionFilterGroup = BIT(1);
		l_rayCallBack.m_collisionFilterMask = p_collidesWith;
		m_dynamicsWorld->rayTest(p_start, p_end, l_rayCallBack);
		if (l_rayCallBack.hasHit())
		{
			//Store results
			p_end = l_rayCallBack.m_hitPointWorld;
			p_normal = l_rayCallBack.m_hitNormalWorld;
			btRigidBody * l_body = const_cast<btRigidBody*>(btRigidBody::upcast(l_rayCallBack.m_collisionObject));
			btCollisionObject * l_object = const_cast<btCollisionObject*>(l_rayCallBack.m_collisionObject);
			//Find the collided object
			//if (l_objectArray.findLinearSearch(l_object) != l_objectArray.size())
			//{
				p_collidedHandle = l_object;
				return true;
			//}
		}
	}
	return false;
}

void bulletWrapper::updateTransform(btCollisionObject * p_handle, const btVector3 & p_transform, const btQuaternion & p_rotation, const btVector3 & /*p_scale*/)
{
  if (!m_isRunning)
  {
    return;
  }

  //Potential : https://gamedev.stackexchange.com/questions/45203/bullet-physic-transform-body-after-adding

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();

  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    btTransform l_transform;

    if (body && body->getMotionState())
    {
      //body->forceActivationState(DISABLE_DEACTIVATION);
      //setActivationState(p_handle, 0);

      l_transform.setOrigin(p_transform);
      l_transform.setRotation(p_rotation);

      body->getMotionState()->setWorldTransform(l_transform);
      //Also need this as of November 22nd, 2019
      body->setWorldTransform(l_transform);

      //TODO: Set new scale (BREAKS BULLET for some reason), solution for this is taking it out of sim
      //obj->getCollisionShape()->setLocalScaling(p_scale);

      //setActivationState(p_handle, 1);
    }
    else
    {
      //setActivationState(p_handle, 0);

      l_transform = obj->getWorldTransform();

      l_transform.setOrigin(p_transform);
      l_transform.setRotation(p_rotation);

      obj->setWorldTransform(l_transform);

      //TODO: Set new scale (BREAKS BULLET for some reason)
      //obj->getCollisionShape()->setLocalScaling(p_scale);

      //setActivationState(p_handle, 1);
    }
  }
}

void bulletWrapper::setBTMass(btCollisionObject * p_handle, float p_mass)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    int l_group = obj->getBroadphaseHandle()->m_collisionFilterGroup;
    int l_mask = obj->getBroadphaseHandle()->m_collisionFilterMask;

    (void)l_group;
    (void)l_mask;

    //https://studiofreya.com/game-maker/bullet-physics/bullet-physics-how-to-change-body-mass/
    //Most likely will trash handles
    //TODO: Set Mass

    int l_previousGroup, l_previousMask;

    //Remove the rigid body to the dynamics world
    removeFromSimulation(p_handle, l_previousGroup, l_previousMask);

    btVector3 inertia;
    body->getCollisionShape()->calculateLocalInertia(p_mass, inertia);
    body->setMassProps(p_mass, inertia);

    //Add the rigid body to the dynamics world
    addToSimulation(p_handle, l_previousGroup, l_previousMask);
  }
}

void bulletWrapper::setBTFriction(btCollisionObject * p_handle, float p_friction)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    //Remove the rigid body to the dynamics world
    //removeFromSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 0);

    //Set Friction
    body->setFriction(p_friction);

    //Add the rigid body to the dynamics world
    //addToSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 1);
  }
}

void bulletWrapper::setBTRestitution(btCollisionObject * p_handle, float p_restitution)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    //Remove the rigid body to the dynamics world
    //removeFromSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 0);

    //Set restitution
    body->setRestitution(p_restitution);

    //Add the rigid body to the dynamics world
    //addToSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 1);
  }
}

void bulletWrapper::setBTDamping(btCollisionObject * p_handle, float p_damping)
{
  if (!m_isRunning)
  {
    return;
  }

  auto l_objectArray = m_dynamicsWorld->getCollisionObjectArray();
  // check to see if handle is in world
  if (l_objectArray.findLinearSearch(p_handle) != l_objectArray.size())
  {
    btCollisionObject * obj = p_handle;
    btRigidBody * body = btRigidBody::upcast(obj);

    //Remove the rigid body to the dynamics world
    //removeFromSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 0);

    //Set damping
    body->setDamping(p_damping, body->getAngularDamping());

    //Add the rigid body to the dynamics world
    //addToSimulation(p_handle, l_previousGroup, l_previousMask);
    setActivationState(p_handle, 1);
  }
}


void bulletWrapper::destroySimulator()
{
  //remove the rigidbodies from the dynamics world and delete them
  int i;
  for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
  {
    btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
    btRigidBody* body = btRigidBody::upcast(obj);
    if (body && body->getMotionState())
    {
      delete body->getMotionState();
    }
    m_dynamicsWorld->removeCollisionObject(obj);
    delete obj;
  }

  //delete collision shapes
  for (int j = 0; j < m_collisionShapes.size(); j++)
  {
    btCollisionShape* shape = m_collisionShapes[j];
    delete shape;
  }

    delete m_dynamicsWorld;
    //delete m_collisionConfiguration; //Deleted in destructor
    delete m_dispatcher;
    delete m_broadphase;
    delete m_solver;

    m_collisionShapes.clear();
    m_bodies.clear();
    m_isRunning = false;
    m_systemManager = nullptr;
}

//=========================================================//
//=========================================================//
