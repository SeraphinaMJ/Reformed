/*!***************************************************************************************
\file       menuControllerScript.hpp
\author     Ryan Booth
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"

class menuControllerScript : public scriptCPP
{
public:
  menuControllerScript() = default;
  menuControllerScript(typeRT p_data);
  componentType const type() const override { return menuControllerScript::getType(); }
  static componentType const getType() { static componentType const s_type = componentType::forInherited<menuControllerScript, scriptCPP>(); return s_type; }
  static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "menuControllerScript", true, true, false); }

  void onUpdate() override;

private:
};