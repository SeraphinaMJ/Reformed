/*!***************************************************************************************
\file       Health.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Health component for npc or player.
*****************************************************************************************/
#pragma once
#include "Component.hpp"
#include "QueryableInterface.hpp"

class Health : public componentCRTP<Health>, public queryableInterface
{
    int health;

    float m_pain_timer;
    float m_max_pain_time;
public:
    Health();

    void Update(float dt);

    void DecreaseHealth(int amount);
    void IncreaseHealth(int amount);

    int GetHealth() const;
    void SetHealth(int amount);

    bool IsDead() const;

    // instantly kill actor
    void Kill();

    // Default component stuff
    static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Health", true, true, true); }

    /*!***************************************************************************************
        \brief  Gets component type
        \return The component type
        *****************************************************************************************/
    static componentType const getType() { static componentType const type = componentType::forConcrete<Health>(); return type; }

    /*!*******************************************************************************
    \brief  Retrieves the type of component this instance is. SceneSystem requirement.
    \return componentType - The type of this component.
    *********************************************************************************/
    virtual componentType const type() const override;

    /*!*******************************************************************************
    \brief  Converts this into a type
    \return Type equivalent.
    *********************************************************************************/
    virtual typeRT toTypeRT() const override;

    virtual void updateFromTypeRT(typeRT & p_type) override;

    /*!***************************************************************************************
    \brief  Retrieves the type this class, for use with the queryable interface.
    \return string - The type of class this represents.
    *****************************************************************************************/
    std::string getQueryableType() override;

    /*!***************************************************************************************
    \brief  Retrieves the ID of this object's parent, given the parent is valid
    \return string - The ID of the object.
    *****************************************************************************************/
    std::string getQueryableID() override;

    /*!***************************************************************************************
    \brief  Retrieves a property of the object by name
    \param p_name the name of the property(data)
    \return string - Returns the desired data
    *****************************************************************************************/
    std::string getQueryableProperty(std::string p_name) override;

    /*!***************************************************************************************
    \brief  Retrieves a property of the object by name
    \param p_name the name of the property(data)
    \return string - Returns the desired data
    *****************************************************************************************/
    std::vector<std::string> getQueryablePropertyNames() override;

};

