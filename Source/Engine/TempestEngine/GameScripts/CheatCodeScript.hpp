/*************************************************************************************
\file       CheatCodeScript.hpp
\author     Ryan Booth
\date       12 / 6 / 2019
\copyright  All content © 2018 - 2019 DigiPen(USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include "../Components/ScriptCPP.hpp"

class cheatCodeScript : public scriptCPP
{
public:
    cheatCodeScript();
    cheatCodeScript(typeRT p_data);

    componentType const type() const override { return cheatCodeScript::getType(); }
    static componentType const getType() { static componentType const s_type = componentType::forInherited<cheatCodeScript, scriptCPP>(); return s_type; }
    static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "cheatCodeScript", true, true, false); }

    typeRT toTypeRT() const override;
    void updateFromTypeRT(typeRT& p_type) override;

protected:
    void onStart() override;
    void onUpdate() override;

private:
    void killPlayer();
    void showMemoryDebug();
};