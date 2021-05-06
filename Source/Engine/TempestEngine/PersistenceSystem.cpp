/*!***************************************************************************************
\file       PersistenceSystem.cpp
\author     Michael-Paul Moore
\date       8/2/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Functionality for persistenceSystem.
*****************************************************************************************/

//========Self Include==================================================================//
#include "PersistenceSystem.hpp"

//========1st Party Includes============================================================//
#include "Serialization.h"
#include "Reflection.hpp"

//========3rd Party Includes============================================================//
#include <fstream>
#include <rapidjson.h>
#include <document.h>
#include <istreamwrapper.h>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include "Logger.hpp"

////////==========================================================================////////
////////  Struct / Class                                                          ////////
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
		////      Overridden                                                                  ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void persistenceSystem::onHandleMessage(message& /*p_message*/)
		{
			/*auto code = static_cast<messageCodes>(p_message.m_message_code);
			switch(code)
			{
			case(save_game_object): { static_cast<persistenceRequest<gameObject>*>(p_message.m_data)->execute(getSystemManager()->getConfigurationFunction()().data_path);}break;
			case(load_game_object): { static_cast<persistenceRequest<gameObject>*>(p_message.m_data)->execute(getSystemManager()->getConfigurationFunction()().data_path);}break;
			case(save_space): {static_cast<persistenceRequest<space>*>(p_message.m_data)->execute(getSystemManager()->getConfigurationFunction()().data_path);}break;
			case(load_space): {static_cast<persistenceRequest<space>*>(p_message.m_data)->execute(getSystemManager()->getConfigurationFunction()().data_path);}break;
			case(save_scene): {static_cast<persistenceRequest<scene>*>(p_message.m_data)->execute(getSystemManager()->getConfigurationFunction()().data_path);}break;
			case(load_scene): {static_cast<persistenceRequest<scene>*>(p_message.m_data)->execute(getSystemManager()->getConfigurationFunction()().data_path);}break;
			}*/
		}

		void persistenceSystem::onUpdate()
		{
			//GameObjectFiltering::componentTypeSet l_set;
			//l_set.setType(persistenceRequestBase::getType());
			//auto l_list = getSystemManager()->getGameObjectGatherer()->getList(l_set);

			//for(const auto& game_obj : l_list)
			//{
			//	const auto & l_requests = game_obj->getComponents<persistenceRequestBase>();

			//	for(const auto& i_request : l_requests)
			//	{
			//		if (i_request->m_converted == false)
			//		{
			//			//i_request->execute(getSystemManager()->getConfigurationFunction()().data_path);
   //                     //i_request->m_converted = true;
			//		}
			//	}

			//}
		}

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Constructor                                                                   //
		//======================================================================================//

		persistenceSystem::persistenceSystem()
		{
			m_json_searializer = new jsonSerialization;
		}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		std::string persistenceSystem::objectToStream(void * p_object,
		                                const std::function<std::string(void*)>& p_write_function)
		{
			return p_write_function(p_object);
		}

		const std::string &persistenceSystem::getDataLocation() const
		{
			return m_data_location;
		}

		void persistenceSystem::writeToFile(const std::string p_data, const std::string p_folder, const std::string p_file_name)
		{

			// Make sure directory exist.
			std::filesystem::create_directories(p_folder);

			// Get file name.
			const std::string &file_name = p_folder + p_file_name;

			// Create file.
			std::ofstream out_stream(file_name);

			logger("persistence").debug() << "BEfore checking anything: " << file_name << std::endl;

      if(out_stream.is_open())
      {
        logger("persistence").debug() << "FileName from writeToFile: " << file_name << std::endl;

        out_stream << p_data;
        out_stream.close();
      }
		}

		typeRT persistenceSystem::readFromFile(const std::filesystem::path p_folder,
											   const std::filesystem::path p_file_name)
		{
			std::filesystem::path l_path = p_folder / p_file_name;
			std::ifstream if_stream(l_path);

			if(if_stream)
			{
				std::string file_as_string((std::istreambuf_iterator<char>(if_stream)),
											std::istreambuf_iterator<char>());
				return jsonSerialization::streamToType(file_as_string);
			}

			logger("persistence").error() << "Failed to open file at: " << l_path << std::endl;

			throw std::filesystem::filesystem_error("Could not open file", l_path,
													std::make_error_code(std::errc::no_such_file_or_directory));
		}

//======================================================================================//
		//        Destructor                                                                    //
		//======================================================================================//


		persistenceSystem::~persistenceSystem()
		{
			delete m_json_searializer;
		}
