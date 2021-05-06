/*!***************************************************************************************
\file       SystemBundle.cpp
\author     Michael-Paul Moore
\date       6/29/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  System bundle functionality.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "SystemBundle.hpp"
//======== 1st Party Includes ==========================================================//
#include "SystemBase.hpp"
#include "SystemTask.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
systemBundle::systemBundle(const std::string & p_bundle_name,
                           const std::vector<std::pair<std::string, systemBase*>>&& p_systems,
                           const thread_number && p_thread_afinity)
                          : m_task(*this, p_thread_afinity), m_bundle_name(p_bundle_name),
                            m_systems_in_vector(createVector(p_systems)),
                            m_systems_in_map(createMap(p_systems)) { }

systemBundle::~systemBundle()
{
	for (auto i_iter = m_systems_in_vector.rbegin(); i_iter != m_systems_in_vector.rend(); )
	{
		delete *i_iter;
		++i_iter;
	}
}

//======== Getters & Setters ===========================================================//
const std::string& systemBundle::name() const
{
	return m_bundle_name;
}

systemTask & systemBundle::task()
{
	return m_task;
}

const systemTask & systemBundle::task() const
{
	return const_cast<systemBundle*>(this)->task();
}

const std::map<std::string, systemBase*>& systemBundle::systemMap() const
{
	return m_systems_in_map;
}

const std::vector<systemBase*>& systemBundle::sysVec() const
{
	return m_systems_in_vector;
}

void systemBundle::setDesiredFrameTime(float p_frame_time)
{
	m_desired_frame_time = p_frame_time;
}

float systemBundle::getDt() const
{
	return m_dt;
}



//======== Overrides ===================================================================//

//======== Functionality ===============================================================//
void systemBundle::updateDT(const HRC_time_point_t & p_current_time, float p_core_dt)
{
	// calculate and set dt
	m_time_to_next_frame -= p_core_dt;
	if (m_time_to_next_frame <= 0.0f)
	{
		m_should_run_this_call = true;
		m_dt = (p_current_time - m_lastTime).count() / float(1E09);
		m_lastTime = p_current_time; // update last frame start
		m_time_to_next_frame += m_desired_frame_time;
	}
	else
	{
		m_should_run_this_call = false;
	}
}

void systemBundle::initialize(systemManagerInterface * p_system_manager)
{
	updateDT(std::chrono::high_resolution_clock::now(), 0.0f);  // pump dt to clear out old initial value
	for(const auto & l_system : systemMap())
	{
		l_system.second->initialize(p_system_manager);
	}
}

void systemBundle::update()
{
	for(auto l_system : sysVec())
	{
		l_system->startFrame();
	}
	for(auto l_system : sysVec())
	{
		l_system->update(getDt());
	}
	for(auto l_system : sysVec())
	{
		l_system->endFrame();
	}
}


//======== Helper Functions ============================================================//

std::map<std::string, systemBase*> systemBundle::createMap(const std::vector<std::pair<std::string, systemBase*>>& p_systems) const
{
	std::map<std::string, systemBase*> result;
	for (const auto & system_pair : p_systems)
		result[system_pair.first] = system_pair.second;
	return result;
}

std::vector<systemBase*> systemBundle::createVector(const std::vector<std::pair<std::string, systemBase*>>& p_systems) const
{
	// preserve ordering
	std::vector<systemBase*> l_result;
	for(size_t i_index = 0; i_index < p_systems.size(); ++i_index)
	{
		l_result.push_back(p_systems[i_index].second);
	}
	return l_result;
}
