/*!***************************************************************************************
\file       Command.hpp
\author     Charles Boudousquie
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the command abstract class.
*****************************************************************************************/
#pragma once
#include <functional>

#include "Reflection.hpp"
#include <Color4.hpp>


namespace Editor
{
  class EditorObject;
  class engineController;

  template <typename T>
  using func_t = void(*)(typeRT & p_data, T p_data_type); // pointer to function with 2 arg and void return

    //class typeRT;
    /*!***************************************************************************************
    \par class: Command
    \brief   This is the abstract interface for which a change to a gameobject(s) is represented.
    *****************************************************************************************/
    class Command
    {
    public:
        virtual ~Command() = default;

        /*!***************************************************************************************
        \brief Initializes command with enginecontroller and object id.
        \param controller - gives access to engine, useful for object creation/deletion
        \param p_editor_object - the pointer to the EditorObject being modified with this command
        *****************************************************************************************/
      virtual void Init(engineController* controller, EditorObject * p_editor_object);

        /*!***************************************************************************************
        \brief Undos a change done to the object
        *****************************************************************************************/
        virtual void undo() = 0;

        /*!***************************************************************************************
        \brief Redos a change done to the object
        *****************************************************************************************/
        virtual void redo() = 0;

        /*!***************************************************************************************
        \brief Records after state.
        \param newState - current state of object to be recorded
        *****************************************************************************************/
        virtual void Record();

        /*!***************************************************************************************
        \brief Executes a command function.
        *****************************************************************************************/
        virtual void execute();

        //Object Data Change Commands
        static void setBool(typeRT & p_data, bool p_bool);
        static void setChar(typeRT & p_data, char p_char);
        static void setInt(typeRT & p_data, int p_int);
        static void setUInt(typeRT & p_data, unsigned int p_unsigned_int);
        static void setFloat(typeRT & p_data, float p_float);
        static void setDouble(typeRT & p_data, double p_double);
        static void setString(typeRT & p_data, std::string p_string);
        static void setULong(typeRT & p_data, unsigned long p_unsigned_long);
        static void setVector3(typeRT & p_data, vector3 p_items);
        static void setVector4(typeRT & p_data, vector4 p_items);
        static void setQuaternion(typeRT & p_data, quaternion p_items);
        static void setVariableName(typeRT & p_data, std::string p_string);
        static void setColorRange(typeRT & p_data, std::vector<color4> p_items);
        static void setAngleRange(typeRT & p_data, std::vector<float> p_items);
		static void setEmitterShape(typeRT & p_data, std::vector<int> p_items);

    protected:
        std::string originalName;
        std::string newName;
        engineController * eController;
        EditorObject * m_editor_object;
        typeRT beforeState;
        typeRT afterState;
        unsigned long id; // hopefully objID always stays a unsigned long, it better

    };


};

