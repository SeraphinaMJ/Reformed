/*!***************************************************************************************
\file       BulletWrapper.hpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#pragma once

//=========================================================//
//                   BulletWrapper                         //
//=========================================================//

#include <vector>

#include <windows.h>

#include "../TempestEngine/CollisionLayer.hpp"

#ifdef _WIN32

//#define  USE_BRUTEFORCE_RAYBROADPHASE


#pragma warning(push)
#pragma warning(disable : 5033)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)

#endif

#include "btBulletDynamicsCommon.h"

#ifdef _WIN32

#pragma warning(pop)

#endif


#include "DLLControl.hpp"
//#include "../../Engine/TempestEngine/SystemManagement/SystemManagerInterface.hpp"

class systemManagerInterface;
//enum collisionLayer;

    class bulletWrapper /*: public systemBase */
{
public:
  ~bulletWrapper();


		static void  createSimulator(const btVector3 & p_globalGravity, systemManagerInterface * p_systemManager);
    static btCollisionObject * addPhysicsObject(float p_mass, btVector3 p_scale, btVector4 p_position, btQuaternion p_rotation, btVector3 p_localInertia, const btVector3 & p_localGravity, float p_restitiution, float p_friction, float p_damping, const int p_shape, const int p_collisionLayerSelf, const int p_collisionLayerCollidesWith);
    static void destroySimulator();
    static btVector4 updatePhysicsObject(btCollisionObject * p_handle, btQuaternion & p_rotation);
    static void updatePhysicsWorld();
    static void addCentralImpulseToObject(btVector3 p_force, btCollisionObject * p_handle);
    static void addCentralForceToObject(btVector3 p_force, btCollisionObject * p_handle);
    static btCollisionShape* createCollisionShape(const int p_shape, btVector3 scale);

    static btVector3 getBTVelocity(btCollisionObject * p_handle);
    static bool getBTUsingGravity(btCollisionObject * p_handle);
    static btVector3 getBTGlobalGravity();
    static void setBTGlobalGravity(btVector3 p_gravity);
    static btVector3 getBTLocalGravity(btCollisionObject * p_handle);
    

    static bool checkCollision(btCollisionObject * p_handle, const btVector3 & p_start, btVector3 & p_end, btVector3 & p_normal, int p_collidesWith, btCollisionObject * & p_collidedHandle);

    static void updateTransform(btCollisionObject * p_handle, const btVector3 & p_transform, const btQuaternion & p_rotation, const btVector3 & p_scale);

    static void setBTMass(btCollisionObject * p_handle, float p_mass);
    static void setBTFriction(btCollisionObject * p_handle, float p_friction);
    static void setBTRestitution(btCollisionObject * p_handle, float p_restitution);
    static void setBTDamping(btCollisionObject * p_handle, float p_damping);
    static void setBTVelocity(btCollisionObject * p_handle, const btVector3 & p_velocity);
    static void setBTGravity(btCollisionObject * p_handle, const btVector3 & p_gravity);
    static void setBTCollisionLayer(btCollisionObject * p_handle, int p_collisionSelf, int p_collisionWith);

    static void setActivationState(btCollisionObject * p_handle, int p_state);

    static void removeFromSimulation(btCollisionObject* p_handle, int & p_previousGroup, int & p_previousMask);
    static void addToSimulation(btCollisionObject* p_handle, int p_previousGroup, int p_previousMask);

    static void setTimeStep(float p_dt);

    static void deleteObjectFromSimulation(btCollisionObject* p_handle);

    static systemManagerInterface * getBTSystemManager();

    static void setIsRunning(bool p_running);
    static bool isRunning() { return m_isRunning; }

	static bool castRayIntoScene(btVector3& p_start, btVector3& p_end, btVector3& p_normal, btCollisionObject * & p_collidedHandle, collisionLayer p_collides_with);

private:

    static btDefaultCollisionConfiguration * m_collisionConfiguration;
    static btCollisionDispatcher * m_dispatcher;
    static btDbvtBroadphase * m_broadphase;
    static btSequentialImpulseConstraintSolver * m_solver;
    static btDiscreteDynamicsWorld * m_dynamicsWorld;
    static btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
    static std::vector<btRigidBody*> m_bodies;

    static float m_time_step;
    static bool m_isRunning;
    static systemManagerInterface* m_systemManager;
};
