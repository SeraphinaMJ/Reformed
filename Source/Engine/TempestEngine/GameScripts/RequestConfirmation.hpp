/*!***************************************************************************************
\file       RequestConfirmation.hpp
\author     Ryan Booth
\date       12/07/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#pragma once
#include "ScriptCPP.hpp"

class requestConfirmation : public scriptCPP
{
public:
    requestConfirmation();
    requestConfirmation(typeRT p_data);

    componentType const type() const override { return requestConfirmation::getType(); }
    static componentType const getType() { static componentType const s_type = componentType::forInherited<requestConfirmation, scriptCPP>(); return s_type; }
    static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "requestConfirmation", true, true, false); }

    typeRT toTypeRT() const override;
    void updateFromTypeRT(typeRT& p_type) override;

    bool getStatus() const { return m_status; }
    bool getConfirmed() const { return m_confirmed; }

protected:
    void onStart() override;
    void onUpdate() override;

private:
    bool m_status = false;
    bool m_confirmed = false;
};
