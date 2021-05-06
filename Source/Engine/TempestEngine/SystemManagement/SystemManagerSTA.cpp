/*!**********************************************************************************************************
\file       SystemManagerSTA.cpp
\author     Aaron Damyen
\date       11/13/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#include "SystemManagerSTA.hpp"
#include "../SystemBase.hpp"
#include <cassert>
#include "../Utility/Debug.hpp"
#include <iomanip>
#include "SystemMonitor.hpp"
#include <PreShutdownEvent.hpp>
#include "EventBus.hpp"
#include "Message.hpp"
#include "PhysicsSystem.hpp"

systemManagerSTA::systemManagerSTA()
	: m_go_gatherer{nullptr}
{
}

void systemManagerSTA::addSystemBundles(const std::vector<systemBundle*>& p_system_bundles)
{
	for (const auto & l_system_bundle : p_system_bundles)
		m_system_bundle_master[l_system_bundle->name()] = l_system_bundle;

	for (const auto & l_system_bundle : p_system_bundles)
		m_system_master.insert(l_system_bundle->systemMap().begin(), l_system_bundle->systemMap().end());
}

void systemManagerSTA::initializeSystemBundles(const std::vector<std::string>& p_system_bundle_names)
{
	for(const auto & l_system_name : p_system_bundle_names)
	{
		assert(m_system_bundle_master.find(l_system_name) != m_system_bundle_master.end());
		m_system_bundle_master[l_system_name]->initialize(this);
	}
}

void systemManagerSTA::activateSystemBundles(const std::vector<std::string>& p_system_bundle_names)
{
	for (const auto l_system_name : p_system_bundle_names)
		activateSystemBundle(l_system_name);
}

void systemManagerSTA::waitForBundleInitialization(std::vector<std::string> const & /*p_system_bundle_names*/)
{
	; // do nothing, no need to wait for initialization
}

systemBase * systemManagerSTA::getSystem(const std::string & p_system_name)
{
	if (m_system_master.find(p_system_name) != m_system_master.end())
		return m_system_master[p_system_name];
	return nullptr;
}

configurationFunctionPtr systemManagerSTA::getConfigurationFunction() const
{
	return m_conf_function;
}

bool systemManagerSTA::isSystemActive(systemBase* p_system_ptr)
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

std::vector<std::string> systemManagerSTA::getAllBundleNames() const
{
	std::vector<std::string> l_results;
	for(auto l_bundle_pair : m_system_bundle_master)
	{
		l_results.push_back(l_bundle_pair.first);
	}
	return l_results;
}

std::vector<std::string> systemManagerSTA::getActiveBundleNames() const
{
	std::vector<std::string> l_results;
	for (auto l_bundle : m_active_system_bundles)
	{
		l_results.push_back(l_bundle->name());
	}
	return l_results;
}

std::vector<std::string> systemManagerSTA::getSystemNamesInBundle(const std::string& p_bundle_name)
{
	std::vector<std::string> l_results;
	for(auto l_system_entry : getBundle(p_bundle_name)->systemMap())
	{
		l_results.push_back(l_system_entry.first);
	}
	return l_results;
}

void systemManagerSTA::enableBundle(const std::string & p_bundle_name)
{
	systemBundle * l_bundle = getBundle(p_bundle_name);
	if(l_bundle != nullptr && !alreadyActive(l_bundle))
	{
		activateSystemBundle(p_bundle_name);
	}
}

void systemManagerSTA::suspendBundle(const std::string& p_bundle_name)
{
  systemBundle * l_bundle = getBundle(p_bundle_name);
  if (l_bundle != nullptr && alreadyActive(l_bundle))
  {
    suspendSystemBundle(p_bundle_name);
  }
}

void systemManagerSTA::disableBundle(const std::string & p_bundle_name)
{
  systemBundle * l_bundle = getBundle(p_bundle_name);
  if (l_bundle != nullptr && alreadyActive(l_bundle))
  {
    shutdownSystemBundle(p_bundle_name);
  }
}

void systemManagerSTA::sendTask(task & p_task)
{
	p_task.onUpdate(); // run it??
}

void systemManagerSTA::setGameObjectGatherer(gameObjectGatherer * p_gatherer)
{
	m_go_gatherer = p_gatherer;
}

gameObjectGatherer * systemManagerSTA::getGameObjectGatherer() const
{
	return m_go_gatherer;
}

void systemManagerSTA::initialize(std::function<configuration()> p_conf_function)
{
	m_conf_function = p_conf_function;
	auto* l_bundle = new systemBundle("__Monitoring", { std::make_pair(systemMonitor::getName(), new systemMonitor()) });
	l_bundle->setDesiredFrameTime(1.0f / 60.0f);
	addSystemBundles({ l_bundle });
	initializeSystemBundles({ "__Monitoring" });
	activateSystemBundles({ "__Monitoring" });
}

void systemManagerSTA::update()
{
	// calculate core dt
	auto l_current_time = std::chrono::high_resolution_clock::now();
	float l_dt = (l_current_time - m_last_time).count() / float(1E09);
	m_last_time = l_current_time;

	// loop over all active systems and run their start frames
	const size_t l_bundle_count = m_active_system_bundles.size();
	for(size_t i_index = 0; i_index < l_bundle_count; ++i_index)
	{
		m_active_system_bundles[i_index]->updateDT(l_current_time, l_dt);
		m_active_system_bundles[i_index]->update();
	}


    EVENT_BUS.update(l_dt);
}

void systemManagerSTA::shutdown()
{
	// send pre-shutdown event
	EVENT_BUS.fire(new preShutdownEvent(), nullptr, true);

	// disable all systems
	for(auto l_bundle : m_active_system_bundles)
	{
		for(auto l_system : l_bundle->systemMap())
		{
			l_system.second->shutdown();
		}
	}
	m_active_system_bundles.clear();
}

void systemManagerSTA::pauseEngine()
{
    for(auto l_system_bundle : getActiveBundleNames())
    {
        if(l_system_bundle == "physics Bundle")
        {
            auto l_physics_system = getSystem<physicsSystem>();

            l_physics_system->setSimulationTimeStep(0.0f);
        }
        else if(l_system_bundle == "graphics & display" || "Scripting Bundle")
        {
            //do nothing
        }
        else
        {
            suspendSystemBundle(l_system_bundle);
        }
    }
}

void systemManagerSTA::resumeEngineFromPause()
{
    for (auto l_system_bundle : getAllBundleNames())
    {
        if (l_system_bundle == "physics Bundle")
        {
            auto l_physics_system = getSystem<physicsSystem>();

            l_physics_system->setSimulationTimeStep(1.0f / 60.0f);
        }
        else if (l_system_bundle == "graphics & display")
        {
            //do nothing
        }
        else
        {
            activateSystemBundle(l_system_bundle);
        }
    }
}

void systemManagerSTA::activateSystemBundle(const std::string& p_system_bundle_name)
{
	if(m_system_bundle_master.find(p_system_bundle_name) == m_system_bundle_master.end())
		return;  // don't have a system bundle by that name

	// verify it is not already active
	if (std::find(m_active_system_bundles.begin(), m_active_system_bundles.end(), m_system_bundle_master[p_system_bundle_name]) != m_active_system_bundles.end())
		return;  // already active, so done!

  if(p_system_bundle_name == "physics Bundle")
  {
    auto l_bundle = getBundle(p_system_bundle_name);

    if (l_bundle != nullptr)
    {
      for (auto l_system : l_bundle->systemMap())
      {
        if(l_system.second->getState() == systemBase::systemState::enm_Uninitialized)
          l_system.second->initialize(this);
      }
    }
  }


	// add system bundle to the active list
	m_active_system_bundles.push_back(m_system_bundle_master[p_system_bundle_name]);
}

void systemManagerSTA::suspendSystemBundle(const std::string & p_system_bundle_name)
{
	if(m_system_bundle_master.find(p_system_bundle_name) != m_system_bundle_master.end())
	{
		auto l_bundle = m_system_bundle_master[p_system_bundle_name];
		m_active_system_bundles.erase(
			std::find(m_active_system_bundles.begin(),
				m_active_system_bundles.end(),
				l_bundle)
		);
	}
}

void systemManagerSTA::shutdownSystemBundle(const std::string& p_system_bundle_name)
{
  if (m_system_bundle_master.find(p_system_bundle_name) != m_system_bundle_master.end())
  {
    auto l_bundle = m_system_bundle_master[p_system_bundle_name];
    //Shutdown the system bundle
    for (auto l_system : l_bundle->systemMap())
    {
      l_system.second->shutdown();
    }
    //Remove from active list
    m_active_system_bundles.erase(
      std::find(m_active_system_bundles.begin(),
        m_active_system_bundles.end(),
        l_bundle)
    );
  }
}

systemBundle * systemManagerSTA::getBundle(const std::string & p_system_bundle_name)
{
	if (m_system_bundle_master.find(p_system_bundle_name) != m_system_bundle_master.end())
		return m_system_bundle_master[p_system_bundle_name];
	return nullptr;
}

bool systemManagerSTA::alreadyActive(const systemBundle * p_bundle)
{
	return std::find(m_active_system_bundles.begin(), m_active_system_bundles.end(), p_bundle) != m_active_system_bundles.end();
}

void systemManagerSTA::sendMessage(std::string p_sender, std::string p_destination, unsigned int p_message_code, void * p_data)
{
	auto* m = new message();
	m->m_sender = p_sender;
	m->m_destination = p_destination;
	m->m_message_code = p_message_code;
	m->m_data = p_data;

	EVENT_BUS.fire(m);
}

