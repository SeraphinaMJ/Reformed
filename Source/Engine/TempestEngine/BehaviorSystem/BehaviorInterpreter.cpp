/*!***************************************************************************************
\file       BehaviorInterpreter.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class can interpret between JSON and typeRT for all the behaviors,
            including the specialized behaviors.
*****************************************************************************************/
#include "BehaviorInterpreter.hpp"

#include "BehaviorSpecialData.hpp"
#include "Nodes/Behaviors/AllBehaviorsInclude.hpp"

#include <iostream>

#include "Logger.hpp"

//const SpecialData EnemySpotted::specialData{ {SpecialData::Entry{ DATA_TYPE::, "", -1.0f}} };

static const std::map<std::string, BehaviorType> behTypes
{
    //# leaves
    {"AttackClosestEnemy", BehaviorType::LEAF},        // 
    {"DefaultLeaf", BehaviorType::LEAF},               // 
    {"JumpUp", BehaviorType::LEAF},                    // 
    {"Repeater", BehaviorType::LEAF},                  // 
    {"ReturnToLeader", BehaviorType::LEAF},            // 
    {"GoToEnemy", BehaviorType::LEAF},
    {"JoinSquad", BehaviorType::LEAF},
    {"PickEnemyToFight", BehaviorType::LEAF},

    //#Decorators
    {"EnemyWithinRange", BehaviorType::DECORATOR},     // 
    {"EnemySpotted", BehaviorType::DECORATOR},         // done
    {"Inverter", BehaviorType::DECORATOR},             // 
    {"IsDead", BehaviorType::DECORATOR},               // 
    {"IsLowHealth", BehaviorType::DECORATOR},          // 
    {"Timer", BehaviorType::DECORATOR},                // 
    {"ReturnTrue", BehaviorType::DECORATOR},           // 
    {"RepeatUntilN", BehaviorType::DECORATOR},         // 
    {"RepeatUntilFailure", BehaviorType::DECORATOR},   // 
    {"RepeatUntilSuccess", BehaviorType::DECORATOR},   // 
    {"IsSquadEngaged", BehaviorType::DECORATOR},
    {"HasSquad", BehaviorType::DECORATOR},

    //#Composites
    {"Selector", BehaviorType::COMPOSITE},             // 
    {"Sequencer", BehaviorType::COMPOSITE}             // 

};

// add appropriate default data based on node type
typeRT getNonRenderData(const char* beh_name)
{
    const BehaviorType& type = behTypes.at(beh_name);

    switch (type)
    {
    case BehaviorType::LEAF:
        return Leaf::leafOnRender();
    case BehaviorType::DECORATOR:
        return Decorator::decoratorOnRender();
    case BehaviorType::COMPOSITE:
        return Composite::compositeOnRender();
    default:
        throw std::runtime_error("Behavior has no basic type");
    }

}

// table that links a class type to general information about its specialized data
std::unordered_map<std::string, SpecialData> specialDataTable;

////////////////////////////
// CONVERSIONS SECTION
////////////////////////////
static std::map<DATA_TYPE, std::function<void(rapidjson::Document&,
    typeRT&, const char*)>> toJSON
{

  {DATA_TYPE::BOOL, [](rapidjson::Document& j, typeRT& render_data, const char* name)
  {

    j[name].Set<float>(render_data.member(name).getBool());
  }},

  {DATA_TYPE::INT, [](rapidjson::Document& j, typeRT& render_data, const char* name)
  {

    j[name].Set<int>(render_data.member(name).getInt());
  }},

  {DATA_TYPE::UNSIGNED_INT, [](rapidjson::Document& j, typeRT& render_data, const char* name)
  {

    j[name].Set<unsigned int>(render_data.member(name).getUInt());
  }},

  {DATA_TYPE::FLOAT, [](rapidjson::Document& j, typeRT& render_data, const char* name)
  {

    j[name].Set<float>(render_data.member(name).getFloat());
  }},

  {DATA_TYPE::DOUBLE, [](rapidjson::Document& j, typeRT& render_data, const char* name)
  {

    j[name].Set<double>(render_data.member(name).getDouble());
  }}
};

static std::map<DATA_TYPE, std::function<void(rapidjson::Document&,
    typeRT&, const char*)>> addToJson
{
  {DATA_TYPE::BOOL, [](rapidjson::Document& doc, typeRT& render_data, const char* name)
  {
    doc.AddMember(rapidjson::StringRef(name), render_data.getBool(), doc.GetAllocator());
  }
  },

  {DATA_TYPE::INT, [](rapidjson::Document& doc, typeRT& render_data, const char* name)
    {
      doc.AddMember(rapidjson::StringRef(name), render_data.getInt(), doc.GetAllocator());
    }
  },

  {DATA_TYPE::UNSIGNED_INT, [](rapidjson::Document& doc, typeRT& render_data, const char* name)
    {
      doc.AddMember(rapidjson::StringRef(name), render_data.getUInt(), doc.GetAllocator());
    }
  },

  {DATA_TYPE::FLOAT, [](rapidjson::Document& doc, typeRT& render_data, const char* name)
    {
      doc.AddMember(rapidjson::StringRef(name), render_data.getFloat(), doc.GetAllocator());
    }
  },

  {DATA_TYPE::DOUBLE, [](rapidjson::Document& doc, typeRT& render_data, const char* name)
    {
      doc.AddMember(rapidjson::StringRef(name), render_data.getDouble(), doc.GetAllocator());
    }
  }
};

static std::map<DATA_TYPE, std::function<void(const rapidjson::Value&, typeRT&)>> JSONtoTypeRT
{
  {DATA_TYPE::BOOL, [](const rapidjson::Value& doc, typeRT& render_data)
  {
    render_data.setBool(doc.GetBool());
  }},

  {DATA_TYPE::INT, [](const rapidjson::Value& doc, typeRT& render_data)
  {
    render_data.setInt(doc.GetInt());
  }},

  {DATA_TYPE::UNSIGNED_INT, [](const rapidjson::Value& doc, typeRT& render_data)
  {
    render_data.setUInt(doc.GetUint());
  }},

  {DATA_TYPE::FLOAT, [](const rapidjson::Value& doc, typeRT& render_data)
  {
    render_data.setFloat(doc.GetFloat());
  }},

  {DATA_TYPE::DOUBLE, [](const rapidjson::Value& doc, typeRT& render_data)
  {
    render_data.setDouble(doc.GetDouble());
  }}
};

std::map<DATA_TYPE, std::function<void(std::any&, typeRT&)>> typeRTSetters
{
    {DATA_TYPE::BOOL, [](std::any& value, typeRT& data) { data.setBool(std::any_cast<bool>(value)); }},
    {DATA_TYPE::INT, [](std::any& value, typeRT& data) { data.setInt(std::any_cast<int>(value)); }},
    {DATA_TYPE::UNSIGNED_INT, [](std::any& value, typeRT& data) { data.setUInt(std::any_cast<unsigned int>(value)); }},
    {DATA_TYPE::FLOAT, [](std::any& value, typeRT& data) { data.setFloat(std::any_cast<float>(value)); }},
    {DATA_TYPE::DOUBLE, [](std::any& value, typeRT& data) { data.setDouble(std::any_cast<double>(value)); }}
};
////////////////////////////
// END OF CONVERSIONS SECTION
////////////////////////////


BehaviorInterpreter::BehaviorInterpreter()
{
    // leaves
    specialDataTable["AttackClosestEnemy"] = AttackClosestEnemy::specialData;
    specialDataTable["GoToEnemy"] = GoToEnemy::specialData;

    // decorators
    specialDataTable["EnemyWithinRange"] = EnemyWithinRange::specialData;
    //specialDataTable["EnemySpotted"] = EnemySpotted::specialData;
    specialDataTable["IsLowHealth"] = IsLowHealth::specialData;
    specialDataTable["Timer"] = Timer::specialData;
    specialDataTable["RepeatUntilN"] = RepeatUntilN::specialData;
}

bool BehaviorInterpreter::IsSpecialized(const std::string& name)
{
    return specialDataTable.find(name) != specialDataTable.end();
}

void BehaviorInterpreter::FillJSON(typeRT& render_data, std::string behaviorName,
    rapidjson::Document& doc)
{
    assert(render_data.getVariableName() == "Node Render Data");

    std::vector<SpecialData::Entry> dataList = specialDataTable[behaviorName].entries;  // GetList of special data based on behaviorName;

    // for each special member that the behavior has
    for (auto& entry : dataList)
    {

        // if json has member
        if (doc.HasMember(entry.name))
        {
            // insert value of entry into the json
            toJSON[entry.type](doc, render_data.member(entry.name), entry.name);
        }
        else
        {
            addToJson[entry.type](doc, render_data.member(entry.name), entry.name);
        }
    }
}

void BehaviorInterpreter::FillTypeRT(typeRT& render_data, std::string behaviorName, const rapidjson::Value &JSON)
{
    // get default typert
    render_data = GetDefaultTypeRT(behaviorName);
    typeRT& special_data = render_data.member("Node Render Data");

    std::vector<SpecialData::Entry> dataList = specialDataTable[behaviorName].entries;// GetList of special data based on behaviorName;

    for (auto entry : dataList)
    {
        // if member was written down in file then set it
        if (JSON.HasMember(entry.name))
        {
            JSONtoTypeRT[entry.type](JSON[entry.name], special_data.member(entry.name));
        }
        else
        {
            // otherwise just keep default
            logger("BehaviorInterpreter").error() << "missing variable in json file" << std::endl;
        }
    }

}


typeRT BehaviorInterpreter::GetDefaultTypeRT(std::string behaviorName)
{

    //first step : create default_render_data based on 1 of 3 nodes types
    typeRT renderData = getNonRenderData(behaviorName.c_str());


    // if there is special data to hand over
    if (BehaviorInterpreter::GetInstance().IsSpecialized(behaviorName))
    {
        //    second step :
        auto specialdata = specialDataTable[behaviorName];
        //for each specialized member that behavior has
        for (auto& entry : specialdata.entries)
        {
            //    create said sub_member.
            typeRT sub_member(entry.name);
            //    set appropriate value for sub_member.
            typeRTSetters[entry.type](entry.data, sub_member);
            //    insert sub_member into default_render_data
            renderData.member("Node Render Data").insertMember(sub_member);
        }

    }

    return renderData;
}

