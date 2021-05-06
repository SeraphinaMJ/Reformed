/*!***************************************************************************************
\file       Serialization.cpp
\author     Michael-Paul Moore
\date       7/13/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This contains the functionality for the serialization system.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Serialization.h"

//========1st Party Includes============================================================//
#include "Reflection.hpp"

//========3rd Party Includes============================================================//
#include "prettywriter.h"
#include "stringbuffer.h"

////////==========================================================================////////
////////  Class(serializationSystem)                                              ////////
////////==========================================================================////////

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		//std::string jsonSerialization::gameObjecToStream(void* p_game_object)
		//{
		//	const gameObject& l_game_object = *static_cast<gameObject*>(p_game_object);

		//	rapidjson::StringBuffer buffer;
		//	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		//	rapidjson::Document document;
		//	typeRT game_object_as_type = l_game_object;
		//	type_to_document(game_object_as_type, document, document.GetAllocator());
		//	document.Accept(writer);

		//	return buffer.GetString();
		//}

		std::string jsonSerialization::typeToStream(typeRT & p_type)
		{
			rapidjson::Document document;
			type_to_document(p_type, document, document.GetAllocator());

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);
			return buffer.GetString();
		}

typeRT jsonSerialization::streamToType(const std::string & p_type)
{
	rapidjson::Document document;
	document.Parse(p_type.c_str());
	return document_to_type(document);
}

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void jsonSerialization::type_to_document(typeRT & p_type, rapidjson::Document& result,
		                                         rapidjson::Document::AllocatorType& allocator)
		{
			result.SetObject();

			rapidjson::Document type_name;
			type_name.SetString(p_type.getTypeName().c_str(), allocator);
			result.AddMember("type_name", type_name, allocator);

			rapidjson::Document variable_name;
			variable_name.SetString(p_type.getVariableName().c_str(), allocator);
			result.AddMember("variable_name", variable_name, allocator);

			// decide if this is a base type or not
			if (p_type.isBase())
			{
				rapidjson::Document data;

				if (p_type.getTypeName() == "bool")
				{
					data.SetBool(p_type.getBool());
					result.AddMember("data", data, allocator);
				}
				else if (p_type.getTypeName() == "char" || p_type.getTypeName() == "int" || p_type.getTypeName() == "unsigned int")
				{
					data.SetInt(p_type.getInt());
					result.AddMember("data", data, allocator);
				}
				else if (p_type.getTypeName() == "float")
				{
					data.SetFloat(p_type.getFloat());
					result.AddMember("data", data, allocator);
				}
				else if (p_type.getTypeName() == "double")
				{
					data.SetDouble(p_type.getDouble());
					result.AddMember("data", data, allocator);
				}
				else if (p_type.getTypeName() == "string")
				{
					data.SetString(p_type.getString().c_str(), allocator);
					result.AddMember("data", data, allocator);
				}
				else if (p_type.getTypeName() == "unsigned long")
				{
					data.SetUint(p_type.getObjID());
					result.AddMember("data", data, allocator);
				}
			}
			else
			{
				rapidjson::Value members(rapidjson::kArrayType);

				for (auto type_member : p_type.members())
				{
					rapidjson::Document member;
					type_to_document(type_member.second, member, allocator);
					members.PushBack(member, allocator);
				}

				result.AddMember("members", members, allocator);

				rapidjson::Value array(rapidjson::kArrayType);

				for (auto type_array : p_type.array())
				{
					rapidjson::Document item;
					type_to_document(type_array, item, allocator);
					array.PushBack(item, allocator);
				}

				result.AddMember("array", array, allocator);
			}
		}

		typeRT jsonSerialization::document_to_type(const rapidjson::Value & input)
		{
			std::unique_ptr<typeRT> result;

			// check if base
			if(input.HasMember("data") && input.HasMember("members"))
			{
				assert("JSON Document For Type Incorrect. Cannot Have members & data");
				return {};
			}
			if (!input.HasMember("data") && !input.HasMember("members"))
			{
				assert("JSON Document For Type Incorrect. Cannot have no members and no data");
				return {};
			}
			else if(input.HasMember("data"))
			{
				if (input["data"].IsUint64())
					result.reset(new typeRT(input["variable_name"].GetString(), input["data"].GetUint64()));
				else if (input["data"].IsFloat())
					result.reset(new typeRT(input["variable_name"].GetString(),input["data"].GetFloat()));
				else if (input["data"].IsDouble()) // rapidJSON thinks a float is also a double...
					result.reset(new typeRT(input["variable_name"].GetString(),input["data"].GetDouble()));
				else if (input["data"].IsInt())
					result.reset(new typeRT(input["variable_name"].GetString(),static_cast<objID>(input["data"].GetInt())));
				else if (input["data"].IsString())
				{
					std::string str(input["data"].GetString());
					result.reset(new typeRT(input["variable_name"].GetString(),str));
				}
				else if(input["data"].IsBool())
					result.reset(new typeRT(input["variable_name"].GetString(), input["data"].GetBool()));
				else { assert(false && "Unknown type"); }
			}
			else if (input.HasMember("array") && input["array"].GetArray().Size())
			{
				std::vector<typeRT> types;
				for (const auto & i_type : input["array"].GetArray())
					types.push_back(document_to_type(i_type));
				result.reset(new typeRT(types));
			}
			else
			{
				result.reset(new typeRT);

				result->setTypeName(input["type_name"].GetString());
				std::vector<typeRT> members;

				for (auto& member : input["members"].GetArray())
					members.push_back(document_to_type(member));
				for(auto i_member : members)
				result->insertMember(i_member);
			}

			result->setVariableName(input["variable_name"].GetString());

			return *result;
		}
