/*!***************************************************************************************
\file       RequestConfirmation.cpp
\author     Ryan Booth
\date       12/07/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "RequestConfirmation.hpp"
#include "InputComponent.hpp"
#include "UISystem.hpp"

requestConfirmation::requestConfirmation()
{

}

requestConfirmation::requestConfirmation(typeRT p_data)
{
    updateFromTypeRT(p_data);
}

typeRT requestConfirmation::toTypeRT() const
{
    typeRT toReturn;
    toReturn.setTypeName(getType().name());
    toReturn.setVariableName(getType().name());
    return toReturn;
}

void requestConfirmation::updateFromTypeRT(typeRT& p_type)
{

}

void requestConfirmation::onStart()
{
    m_status = false;

    auto l_obj = getGameObject();

    if (!l_obj.expired())
    {
        auto l_input = l_obj.lock()->addComponent<inputComponent>();

        if (l_input != nullptr)
        {
            l_input->addButtonMapping("Confirm", Key::Y);
            l_input->addButtonMapping("Deny", Key::N);
        }
    }

    auto l_ui_system = getSystem<uiSystem>();

    if(l_ui_system)
    {
        l_ui_system->disableAllButtons();
    }
}

void requestConfirmation::onUpdate()
{
    auto l_obj = getGameObject();

    if (!l_obj.expired())
    {
        auto l_input = l_obj.lock()->getComponent<inputComponent>();

        if (l_input != nullptr)
        {
            if (l_input->getButtonState("Confirm"))
            {
                m_status = true;
                m_confirmed = true;
            }
            if (l_input->getButtonState("Deny"))
            {
                m_status = true;
                m_confirmed = false;
            }

            if(m_status)
            {
                auto l_ui_system = getSystem<uiSystem>();

                if (l_ui_system)
                {
                    l_ui_system->enableAllButtons();
                }
            }
        }
    }
}
