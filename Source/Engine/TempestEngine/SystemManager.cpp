/*!***************************************************************************************
\file       SystemManager.cpp
\author     Michael-Paul Moore
\date       6/29/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This contains the implementation of system manager.
*****************************************************************************************/

//========Self Include==================================================================//
#include "SystemManager.hpp"

//========1st Party Includes============================================================//
#include "Engine.hpp"
#include "SystemBundle.hpp"

#include "Message.hpp"
#include "EventBus.hpp"

#include <cassert>

////////==========================================================================////////
////////  Class (systemManager)                                                   ////////
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
		//        Constructor                                                                   //
		//======================================================================================//

		systemManager::systemManager() : m_go_gatherer(nullptr), m_jobManager()
		{
		}

		void systemManager::initialize(std::function<configuration()> p_conf_function)
		{
			m_conf_function = p_conf_function;
		}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void systemManager::addSystemBundles(const std::vector<systemBundle*>& system_bundles)
		{
			for (const auto & system_bundle : system_bundles) // add bundles to bundle_master
				m_sys_bndl_mast[system_bundle->name()] = std::shared_ptr<systemBundle>(system_bundle); // insert bundle

			for (const auto & system_bundle : system_bundles) // add systems to system_master
				m_system_master.insert(system_bundle->systemMap().begin(), system_bundle->systemMap().end()); // insert map inside bundle
		}

		void systemManager::initializeSystemBundles(const std::vector<std::string> & system_bundle_names)
		{
			for (const auto & system_name : system_bundle_names)
			{
				assert(m_sys_bndl_mast.find(system_name) != m_sys_bndl_mast.end());
				m_sys_bndl_mast[system_name]->task().sendInitialize(this);
				m_jobManager.giveTask(&m_sys_bndl_mast[system_name]->task());
			}
		}

		void systemManager::update()
		{
			m_jobManager.update();
		}

		void systemManager::waitForBundleInitialization(std::vector<std::string> const& system_bundle_names)
		{
			for (const auto & system_name : system_bundle_names)
				m_sys_bndl_mast[system_name]->task().waitForInitialization();
		}

		void systemManager::activateSystemBundles(const std::vector<std::string>& system_bundle_names)
		{
			for (const auto system_name : system_bundle_names)
				activateSystemBundle(system_name);
		}

		void systemManager::setGameObjectGatherer(gameObjectGatherer * gatherer)
		{
		    m_go_gatherer = gatherer;
		}

		gameObjectGatherer * systemManager::getGameObjectGatherer() const
		{
			return m_go_gatherer;
		}

		systemBase * systemManager::getSystem(const std::string & system_to_get_name)
		{
			if (m_system_master.find(system_to_get_name) != m_system_master.end())
				return m_system_master[system_to_get_name].get();
			return nullptr;
		}

		bool systemManager::isSystemActive(systemBase* p_system_ptr)
		{
			for(auto l_bundle : m_active_system_bundles)
			{
				for(auto l_system_entry : l_bundle->systemMap())
				{
					if (l_system_entry.second == p_system_ptr)
						return true;
				}
			}
			return false;
		}

		void systemManager::sendTask(task & task)
		{
			m_jobManager.giveTask(&task);
		}

		void systemManager::shutdown()
		{
			sendShutdownAllSystemBundles();
			//activateAllSystemBundles();
			destroyAllSystems();
		}

		configurationFunctionPtr systemManager::getConfigurationFunction() const
		{
			return m_conf_function;
		}

		std::vector<std::string> systemManager::getAllBundleNames() const
		{
			std::vector<std::string> results;
			for(auto l_bundle_pair = m_sys_bndl_mast.begin(); l_bundle_pair != m_sys_bndl_mast.end(); l_bundle_pair++)
			{
				results.push_back(l_bundle_pair->first);
			}
			return results;
		}

		std::vector<std::string> systemManager::getActiveBundleNames() const
		{
			std::vector<std::string> results;
			for(auto l_bundle = m_active_system_bundles.begin(); l_bundle != m_active_system_bundles.end(); l_bundle++)
			{
				results.push_back((*l_bundle)->name());
			}
			return results;
		}

		void systemManager::enableBundle(const std::string & p_bundle_name)
		{
			systemBundle * l_bundle = getBundle(p_bundle_name);
			if(l_bundle != nullptr && !alreadyActive(l_bundle))
			{
				activateSystemBundle(p_bundle_name);
			}
		}

		void systemManager::disableBundle(const std::string & p_bundle_name)
		{
			systemBundle * l_bundle = getBundle(p_bundle_name);
			if (l_bundle != nullptr && alreadyActive(l_bundle))
			{
				shutdownSystemBundle(p_bundle_name, l_bundle);
			}
		}

void systemManager::suspendBundle(const std::string& p_bundle_name)
{
  systemBundle * l_bundle = getBundle(p_bundle_name);
  if (l_bundle != nullptr && alreadyActive(l_bundle))
  {
    suspendSystemBundle(p_bundle_name);
  }
}

void systemManager::sendMessage(std::string p_sender, std::string p_destination, unsigned p_message_code, void* p_data)
{
	auto* m = new message();
	m->m_sender = p_sender;
	m->m_destination = p_destination;
	m->m_message_code = p_message_code;
	m->m_data = p_data;

	EVENT_BUS.fire(m);
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

		void systemManager::activateSystemBundle(const std::string &system_name)
		{
			m_sys_bndl_mast[system_name]->task().sendRun();
			m_jobManager.giveTask(&m_sys_bndl_mast[system_name]->task());
			m_active_system_bundles.push_back(m_sys_bndl_mast[system_name].get());
		}

		void systemManager::shutdownSystemBundle(const std::string & p_name, systemBundle * p_bundle)
		{
			p_bundle->task().sendShutdown();

			m_system_master.erase(p_name); // temporary solution, TODO: Make a list of available systems
		}

		void systemManager::suspendSystemBundle(const std::string& p_name)
		{
			if (m_sys_bndl_mast.find(p_name) != m_sys_bndl_mast.end())
			{
				auto l_bundle = m_sys_bndl_mast[p_name];
				l_bundle->task().sendSuspend();
				m_active_system_bundles.erase(
					std::find(m_active_system_bundles.begin(),
						      m_active_system_bundles.end(),
						      l_bundle.get()));
			}
		}

		systemBundle* systemManager::getBundle(const std::string& p_name)
		{
			if (m_sys_bndl_mast.find(p_name) != m_sys_bndl_mast.end())
				return m_sys_bndl_mast[p_name].get();
			return nullptr;
		}

		void systemManager::sendShutdownAllSystemBundles()
		{
			for (const auto& bundle : m_sys_bndl_mast)
			{
				shutdownSystemBundle(bundle.first, bundle.second.get());
			}
		}

		void systemManager::activateAllSystemBundles()
		{
			for (const auto & system_bundle : m_sys_bndl_mast)
				if (!alreadyActive(system_bundle.second.get()))
					activateSystemBundle(system_bundle.first);
		}

		bool systemManager::alreadyActive(const systemBundle * const p_system_bundle) const
		{
			for (const auto & system_bundle : m_active_system_bundles)
				if (p_system_bundle == system_bundle)
					return true;
			return false;
		}

		void systemManager::destroyAllSystems()
		{
			while (!m_sys_bndl_mast.empty())
			{
				for (auto sys_bndl_mast_it = m_sys_bndl_mast.begin();
					sys_bndl_mast_it != m_sys_bndl_mast.end(); ++sys_bndl_mast_it)
				{
					if (sys_bndl_mast_it->second->task().isRemoved()) // check if task is finished
					{
						m_sys_bndl_mast.erase(sys_bndl_mast_it);
						break; // since we changed the container structure
					}

					/* Cleaning Up System Manager Run another update since it has not been removed yet. May *
					* be done to much but it is at least not done to little. */
					update();
				}
			}

			m_active_system_bundles.clear();
		}
