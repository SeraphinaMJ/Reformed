/*!***************************************************************************************
\file       Command.cpp
\author     Charles Boudousquie, Ryan Booth
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definitions for the command abstract class.
*****************************************************************************************/

#include "Command.hpp"
#include "UndoRedo.hpp"
#include "EngineController.hpp"
#include "SceneManipulator.hpp"
#include "EditorObject.hpp"
#include "Quaternion.hpp"

#include "EngineRunner.hpp"
#include "Engine.hpp"
#include "Color4Serialization.hpp"

void Editor::Command::Init(engineController* controller, EditorObject* p_editor_object)
{
  if (p_editor_object != nullptr)
  {
    eController = controller;
    m_editor_object = p_editor_object;
    id = p_editor_object->getObjectID();

    // set old state
    beforeState = m_editor_object->getConstData();
    originalName = m_editor_object->getObjectName();
  }
}

void Editor::Command::Record()
{
  afterState = m_editor_object->getConstData();
  newName = m_editor_object->getObjectName();
}

void Editor::Command::execute()
{
  //do nothing
}

void Editor::Command::setBool(typeRT & p_data, bool p_bool)
{
  p_data.setBool(p_bool);
}

void Editor::Command::setChar(typeRT & p_data, char p_char)
{
  p_data.setChar(p_char);
}

void Editor::Command::setInt(typeRT & p_data, int p_int)
{
  p_data.setInt(p_int);
}

void Editor::Command::setUInt(typeRT & p_data, unsigned p_unsigned_int)
{
  p_data.setUInt(p_unsigned_int);
}

void Editor::Command::setFloat(typeRT & p_data, float p_float)
{
  p_data.setFloat(p_float);
}

void Editor::Command::setDouble(typeRT & p_data, double p_double)
{
  p_data.setDouble(p_double);

}

void Editor::Command::setString(typeRT & p_data, std::string p_string)
{
  p_data.setString(p_string);
}

void Editor::Command::setULong(typeRT & p_data, unsigned long p_unsigned_long)
{
  p_data.setULong(p_unsigned_long);
}

void Editor::Command::setVector3(typeRT & p_data, vector3 p_items)
{
  if (p_items[0] != p_data.member("x").getFloat())
  {
    p_data.member("x").setFloat(p_items.x);
  }
  if (p_items[1] != p_data.member("y").getFloat())
  {
    p_data.member("y").setFloat(p_items.y);
  }
  if (p_items[2] != p_data.member("z").getFloat())
  {
    p_data.member("z").setFloat(p_items.z);
  }

}

void Editor::Command::setVector4(typeRT & p_data, vector4 p_items)
{
  if (p_items[0] != p_data.member("x").getFloat())
  {
    p_data.member("x").setFloat(p_items[0]);
  }
  if (p_items[1] != p_data.member("y").getFloat())
  {
    p_data.member("y").setFloat(p_items[1]);
  }
  if (p_items[2] != p_data.member("z").getFloat())
  {
    p_data.member("z").setFloat(p_items[2]);
  }
  if (p_items[3] != p_data.member("w").getFloat())
  {
    p_data.member("w").setFloat(p_items[3]);
  }
}

void Editor::Command::setQuaternion(typeRT & p_data, quaternion p_items)
{
  if (p_items[0] != p_data.member("x").getFloat())
  {
    p_data.member("x").setFloat(p_items[0]);
  }
  if (p_items[1] != p_data.member("y").getFloat())
  {
    p_data.member("y").setFloat(p_items[1]);
  }
  if (p_items[2] != p_data.member("z").getFloat())
  {
    p_data.member("z").setFloat(p_items[2]);
  }
  if (p_items[3] != p_data.member("w").getFloat())
  {
    p_data.member("w").setFloat(p_items[3]);
  }

}


void Editor::Command::setEmitterShape(typeRT & p_data, std::vector<int> p_items)
{
	std::vector<typeRT>& members = p_data.array();

	members[0].setInt(p_items[0]);
}

void Editor::Command::setVariableName(typeRT& p_data, std::string p_string)
{
  p_data.setVariableName(p_string);
}

void Editor::Command::setColorRange(typeRT& p_data, std::vector<color4> p_items)
{
  std::vector<typeRT>& members = p_data.array();

  members[0] = ::toTypeRT(p_items[0]);
  members[1] = ::toTypeRT(p_items[1]);
}

void Editor::Command::setAngleRange(typeRT& p_data, std::vector<float> p_items)
{
  std::vector<typeRT>& members = p_data.array();

  members[0].setFloat(p_items[0]);
  members[1].setFloat(p_items[1]);
}
