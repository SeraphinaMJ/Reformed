/*!***************************************************************************************
\file       RigidBody.hpp
\author     Ryan Booth
\date       8/9/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Defines the rigid body of the object
*****************************************************************************************/
#pragma once

#include "../CollisionLayer.hpp"
#include "../../../Physics.Bullet/BulletWrapper.hpp"
#include <Component.hpp>
#include <ComponentTypeDetails.hpp>
#include <Vector3.hpp>



enum class collisionShape
{
    enm_circle = 0,
    enm_square = 1,
    enm_plane  = 2,
    enm_count
};


class rigidBody : public componentCRTP<rigidBody>
{
public:
	/*!***************************************************************************************
	\brief  Converts type to rigidbody.
	\param p_type - type to convert
	*****************************************************************************************/
	rigidBody(typeRT& p_type);

	/*!***************************************************************************************
	\brief  default constructor.
	*****************************************************************************************/
	rigidBody();

  ~rigidBody();

    /*!***************************************************************************************
        \brief  Converts this into a type
        \return Type equivalent.
        *****************************************************************************************/
    typeRT toTypeRT() const override;
    /*!***************************************************************************************
    \brief  Gets component type
    \return The component type
    *****************************************************************************************/
    componentType const type() const override { return getType(); }

    /*!***************************************************************************************
    \brief  Gets component type
    \return The component type
    *****************************************************************************************/
    static componentType const getType() { static componentType const type = componentType::forConcrete<rigidBody>(); return type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Rigid Body", true, true, false); }
    
		void updateFromTypeRT(typeRT& p_type) override;

  /*!***************************************************************************************
    \brief Gets the mass of the object
    \return The mass of the object
    *****************************************************************************************/
    float getMass() { return m_mass; }

  /*!***************************************************************************************
    \brief Currently 3 collision shapes (circle, square, plane). Collision shape is a "bounding box".
    \return The collision shape.
    *****************************************************************************************/
    collisionShape getCollisionShape() { return m_collisionShape; }

  /*!***************************************************************************************
    \brief Gets the current local inertia of the object.
    \return The local inertia this object has.
    *****************************************************************************************/
    vector3 getLocalInertia() { return m_localInertia;  }

  /*!***************************************************************************************
    \brief "Restitution is like 'bounciness'. Higher restitution makes a body bounce more, no 
            restitution gives no added bouncing." https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=8100
    \return The restitution of the object.
    *****************************************************************************************/
    float getRestitution() { return m_restitution; }

  /*!***************************************************************************************
    \brief "Friction is essentially damping, but it only applies when two bodies are touching
            instead of all the time like with linear and angular damping. I am not sure how 
            different values on two different bodies interact." https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=8100
    \return The friction of the object.
    *****************************************************************************************/
    float getFriction() { return m_friction; }

  /*!***************************************************************************************
    \brief "In bullet at least, linear damping affects how the body moves through the world in
            any given direction. Having linear damping at zero (the default) means objects will
            keep moving until friction slows them down. At higher values, they would slow down
            even if they don't touch anything. Angular damping is similar, but applies to angular
            motion (ie. rotation)." https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=8100
    \return The damping of the object.
    *****************************************************************************************/
    float getDamping() { return m_damping; }

  /*!***************************************************************************************
    \brief Gets the velocity of the object.
    \return The current velocity of the object.
    *****************************************************************************************/
    vector3 getVelocity();

  /*!***************************************************************************************
    \brief The Local Gravity of an object overrides the constant Global Gravity. Allows for objects
           to have different values of gravity in the same world.
    \return Gets the local gravity of the object.
    *****************************************************************************************/
    vector3 getLocalGravity() { return m_localGravity; }

  /*!***************************************************************************************
    \brief If an object is using Global Gravity then the acceleration on an object due to gravity
           is the 'worlds' gravity. If not using Global Gravity, then the acceleration is the 
           object's value of Local Gravity. 
    \return If the object is using Global Gravity or not.
    *****************************************************************************************/
    bool isUsingGlobalGravity() { return m_useGlobalGravity; }

  /*!***************************************************************************************
    \brief Gets the object ID of this object's parent.
    \return The object ID of the parent.
    *****************************************************************************************/
    objID getParent();

  /*!***************************************************************************************
    \brief The 
    \return A pair of collision layers. The first value is the object's layer, and the second
            value is all of the layers it collides with. By default objects collide with their
            own layer as well.
    *****************************************************************************************/
    std::pair<unsigned int, unsigned int> getCollisionLayer() { return std::pair< unsigned int, unsigned int>(m_self, m_collidesWith); }

  /*!***************************************************************************************
    \brief Gets the handle to the object that bullet uses to track objects.
    \return The bullet handle.
    *****************************************************************************************/
    btCollisionObject * getHandle() { return m_handle; }

  /*!***************************************************************************************
    \brief Sets an object's mass.
    \param p_mass The value to set the object's mass to.
    *****************************************************************************************/
  void setMass(float p_mass);

  /*!***************************************************************************************
    \brief "Restitution is like 'bounciness'. Higher restitution makes a body bounce more, no 
            restitution gives no added bouncing." https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=8100
           
            "With a value of 0.0, it doesn�t bounce at all. The sphere will stick to the floor on the first touch.
            With a value between 0 and 1, the object bounces, but with each bounce loses part of its energy. 
            The sphere will bounce several times, each time lower than the previous bounce until finally it stops.
            With a value of more than 1, the object gains energy with each bounce. 
            This is not very realistic, or at least I can�t think of a real-life object that behaves this way. 
            Your sphere will bounce higher than the point from which it was dropped, then it will bounce even higher and so on, until it bounces right into space."
            https://www.raywenderlich.com/2606-bullet-physics-tutorial-getting-started
    \param p_restitution The value of restitution. 
    *****************************************************************************************/
  void setRestitution(float p_restitution);

  /*!***************************************************************************************
    \brief "Friction is essentially damping, but it only applies when two bodies are touching
            instead of all the time like with linear and angular damping. I am not sure how 
            different values on two different bodies interact." https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=8100
    \param p_friction The value of friction. Range: [0.0f - 1.0f]
    *****************************************************************************************/
  void setFriction(float p_friction);

  /*!***************************************************************************************
    \brief "In bullet at least, linear damping affects how the body moves through the world in
            any given direction. Having linear damping at zero (the default) means objects will
            keep moving until friction slows them down. At higher values, they would slow down
            even if they don't touch anything. Angular damping is similar, but applies to angular
            motion (ie. rotation)." https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=8100
    \param p_damping The value of damping. [0.0f, 1.0f) Not recommended over 1.0f for now.
    *****************************************************************************************/
  void setDamping(float p_damping);

  /*!***************************************************************************************
    \brief Currently 3 collision shapes (circle, square, plane). Collision shape is a "bounding box".
    \param p_collisionShape The shape of the "bounding box". (i.e. circle, square, plane)
    *****************************************************************************************/
    void setCollisionShape(collisionShape p_collisionShape) { m_collisionShape = p_collisionShape; }

  /*!***************************************************************************************
    \brief Sets the object's handle determined by bullet. Should only ever be called once per
           object in theory. Bad things might happen if this is not the case.
    \param p_handle The handle retrieved from bullet.
    *****************************************************************************************/
    void setHandle(btCollisionObject * p_handle) { m_handle = p_handle; }


  /*!***************************************************************************************
    \brief Sets the objects velocity. If used it will temporarily disable gravity's acceleration 
           on an object until it ramps back up.
           https://stackoverflow.com/questions/15180602/bullet-physics-engine-why-does-gravity-not-accelerate
    \param p_velocity The desired velocity.
    *****************************************************************************************/
    void setVelocity(vector3 p_velocity);

  /*!***************************************************************************************
    \brief If true, the object uses the Global Gravity constant. If false, the object uses it's
           Local Gravity value. 
    \param p_useGlobalGravity True or false.
    *****************************************************************************************/
  void useGlobalGravity(bool p_useGlobalGravity);

  /*!***************************************************************************************
    \brief Local Gravity overrides Global Gravity if set to. NOTE: Must call useGlobalGravity()
           with false if you want to use this value, after this function!
    \param p_LocalGravity The value that will override Global Gravity.
    *****************************************************************************************/
    void setLocalGravity(vector3 p_LocalGravity) { m_localGravity = p_LocalGravity; }

  /*!***************************************************************************************
    \brief Sets the collision layer of an object. By default objects collide with their
            own layer as well.
    \param p_self The layer you are on.
    \param p_collidesWith The layers you want to collide with. (can be || together)
    *****************************************************************************************/
    void setCollisionLayer(unsigned int p_self, unsigned int p_collidesWith);

  /*!***************************************************************************************
    \brief Set's the object's parent. Any forces applied to the parent object also get applied
           to the children of that parent.
    \param p_parentObject The object's parent ID.
    *****************************************************************************************/
    void setParent(objID p_parentObject);

  /*!***************************************************************************************
    \brief Removes the object ID of the parent.
    *****************************************************************************************/
    void removeParent() { m_parent = 0; }

  /*!***************************************************************************************
    \brief An object is grounded if its' feet are resting on another object. Ray Casting determination.
           (UNTESTED)
    \return True if "grounded". False if not "grounded".
    *****************************************************************************************/
    bool isGrounded();

  /*!***************************************************************************************
    \brief Performs a ray cast using bullet. (UNTESTED)
    \param p_start The point to start the ray cast from.
    \param p_end The point to end the ray cast.
    \param p_collidesWith
    \param p_handle The btHandle of the collided object if there is one.
    \return True if the line intersected another object, otherwise false.
    *****************************************************************************************/
    bool rayCast(vector3 p_start, vector3 p_end, int p_collidesWith, btCollisionObject * & p_handle);

  /*!***************************************************************************************
    \brief Applies a force to an objects center and adds to its acceleration. 
           Useful for outside forces (like wind).
           If confused see Cody.
    \param p_force The force to be applied to the object.
    *****************************************************************************************/
    void applyForce(const vector3 & p_force);

  /*!***************************************************************************************
    \brief An impulse bypasses acceleration and adds immediately to the object's velocity.
           Useful for instant reaction (for player control).
           If confused see Cody.
    \param p_force The force of the impulse to be applied to the object.
    *****************************************************************************************/
    void applyImpulse(const vector3 & p_force);

  /*!***************************************************************************************
    \brief For physics System to call.
    \param p_start Start of the ray cast.
    \param p_end End of the ray cast.
    \param p_collidesWith Collision layers to perform the ray cast against.
    \param p_handle The btHandle of the object it collided with.
    \return The objID of the first object hit or 0.
    *****************************************************************************************/
    objID getObjectRayCast(const vector3 & p_start, const vector3 & p_end, int p_collidesWith, btCollisionObject * & p_handle);

  /*!***************************************************************************************
    \brief Disables an object in bullet. Dangerous to use. Potential fix for lerping objects.
    \param p_state 0 for disable, 1 for enable.
    *****************************************************************************************/
    void setActivationState(int p_state);

    void updateBulletTransform();

    void setNewRigidBodyFlag(bool p_flag) { m_new_rigid_body = p_flag; }
    bool getNewRigidBodyFlag() const { return m_new_rigid_body; }

private:

    collisionShape m_collisionShape = collisionShape::enm_square;

    unsigned int m_self = collisionLayer::enm_world;
    unsigned int m_collidesWith = collisionLayer::enm_nothing;
    
    vector3 m_localInertia = vector3();
    vector3 m_localGravity = vector3();
    bool m_useGlobalGravity = true;
	  //should change to density to update during runtime in the future.
    float m_mass = 0.0f;
    float m_restitution = 0.0f;
    float m_friction = 0.0f;
    float m_damping = 0.0f;
    btCollisionObject * m_handle = nullptr;

    bool m_new_rigid_body = false;

    //Parent object for being picked up / etc.
    objID m_parent = 0;
};
