/*!***************************************************************************************
\file       BehaviorInterpreter.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class can interpret between JSON and typeRT for all the behaviors,
            including the specialized behaviors.
*****************************************************************************************/
#pragma once
#include <any>
#include <string>
#include <Reflection.hpp>

#include <rapidjson.h>
#include "document.h"


class BehaviorInterpreter
{
    BehaviorInterpreter();
public:
    static BehaviorInterpreter& GetInstance()
    {
        static BehaviorInterpreter instance;
        return instance;
    }

    bool IsSpecialized(const std::string& name);

    static void FillJSON(typeRT& render_data, std::string behaviorName, rapidjson::Document& doc);

    static void FillTypeRT(typeRT& data, std::string behaviorName, const rapidjson::Value &JSON);

    static typeRT GetDefaultTypeRT(std::string behaviorName);


};



