#include "PhysicsInterface.hpp"
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

btDefaultCollisionConfiguration * physicsInterface::m_collisionConfiguration;
btCollisionDispatcher * physicsInterface::m_dispatcher;
btDbvtBroadphase * physicsInterface::m_broadphase;
btSequentialImpulseConstraintSolver * physicsInterface::m_solver;
btDiscreteDynamicsWorld * physicsInterface::m_dynamicsWorld;

void physicsInterface::createSimulator()
{

    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver * sol = new btSequentialImpulseConstraintSolver;
    m_solver = sol;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void physicsInterface::addPhysicsObject()
{
    btCollisionShape* l_groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    (void)l_groundShape;

   /* btDefaultMotionState* l_motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(orientations[i].x, orientations[i].y, orientations[i].z, orientations[i].w),
        btVector3(positions[i].x, positions[i].y, positions[i].z)
    ));*/

    //btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
    //    0,                  // mass, in kg. 0 -> Static object, will never move.
    //    motionstate,
    //    boxCollisionShape,  // collision shape of body
    //    btVector3(0, 0, 0)    // local inertia
    //);

   // btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);

   // m_dynamicsWorld->addRigidBody(rigidBody);

}
