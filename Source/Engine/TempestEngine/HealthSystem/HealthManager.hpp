/*!***************************************************************************************
\file       HealthManager.hpp
\author     Charles Boudousquie
\date       12/2/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manager that updates health component.
*****************************************************************************************/
#pragma once
#include <SystemBase.hpp>


class HealthManager final : public systemBase
{
    public:
    
    
    /*!*******************************************************************************
    \brief  Retrieves the type of component this instance is. SceneSystem requirement.
    \return componentType - The type of this component.
    *********************************************************************************/
    const std::string& name() const override;

    /*!*******************************************************************************
    \brief  Returns name of this system.
    *********************************************************************************/
    static const std::string& getName()
    {
        static const std::string n("HealthManager"); return n;
    }
    
        /*!***************************************************************************************
        \brief  Updates all trees.
        *****************************************************************************************/
        void onUpdate() override;



};