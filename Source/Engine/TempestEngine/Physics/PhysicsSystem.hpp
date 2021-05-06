/*!***************************************************************************************
\file       PhysicsSystem.hpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../SystemBase.hpp"
//#include "../../../Physics.Bullet/BulletWrapper.hpp"
#include "../Components/RigidBody.hpp"
#include <Vector3.hpp>

//=========================================================//
//=========================================================//
//class vector3;

class physicsSystem final : public systemBase
{
///////=======================================================================////////
///////   Public                                                              ////////
///////=======================================================================////////
public:
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("physicsSystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

        ////==========================================================================////
        ////      Pure Virtual                                                         ////
        ////==========================================================================////

        /*!*******************************************************************************
        \brief Engine system call to initalize
        *********************************************************************************/
        void onInitialize() override;

        void onStartFrame() override;

        /*!*******************************************************************************
        \brief Engine system call to update
        *********************************************************************************/
        void onUpdate() override;


        void onEndFrame() override;

        /*!*******************************************************************************
        \brief Engine system call to shutdown
        *********************************************************************************/
        void onShutdown() override;

		    void onLevelLoad();
		    void onLevelUnload();

		    void onLevelLoad(const levelLoadEvent & /*p_event*/) override;
		    void onLevelUnload(const levelUnloadEvent & /*p_event*/) override;

        /*!*******************************************************************************
        \brief Global gravity is what all objects use as the world's gravity, UNLESS the object
               has modified their local gravity.
        *********************************************************************************/
        void setGlobalGravity(const vector3 & p_globalGravity);

        /*!*******************************************************************************
        \brief Global gravity is what all objects use as the world's gravity, UNLESS the object
               has modified their local gravity.
        *********************************************************************************/
        vector3 getGlobalGravity();

  /*!***************************************************************************************
        \brief Returns the objID of the first object the ray cast hits.
        \param p_direction Direction of the ray cast.
        \param p_collidesWith The collision layers to perform the ray cast on.
        \param p_gameObject The gameObject to perform the ray cast from.
        \return The objID of the object if it collided or 0
        *****************************************************************************************/
        objID getObjectRayCast(const vector3 & p_direction, collisionLayer p_collidesWith, std::shared_ptr<gameObject> p_gameObject);

        std::vector<std::shared_ptr<gameObject> > getObjectsWithinRadius(const vector3 & p_point, const float & radius);

        void addPhysicsObjectToWorld(std::shared_ptr<gameObject> p_gameObject);

        void setSimulationTimeStep(float p_dt);

        void processCallback(btPersistentManifold* const & p_manifold, unsigned short p_type);

        void initializeBullet();

        void addAllObjectsToBullet();

        void updateOurObjectsFromBullet();

        void overwriteSpecificObject(objID p_object_id);

		    bool castRay(float p_winPosX, float p_winPosY, float p_mousePosX, float p_mousePosY, collisionLayer p_collides_with, vector3& p_point, vector3& p_normal, objID & p_object_id);

		    vector3 convertToVector3(btVector3 p_btVec3);

		    btVector3 convertToBtVector3(vector3 p_vec3);

        objID getObjectIDFromBTCollisionObject(btCollisionObject * p_collidedHandle);

        void setEngineRunning(bool p_running) { m_engine_running = p_running; }

private:

    //bulletWrapper * m_bullet;
    vector3 m_globalGravity;
    bool m_engine_running = true;
};
