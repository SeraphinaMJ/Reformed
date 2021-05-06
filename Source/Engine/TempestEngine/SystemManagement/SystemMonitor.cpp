/*!***************************************************************************************
\file       SystemMonitor.cpp
\author     Aaron Damyen
\date       3/15/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation for the system monitoring functions
*****************************************************************************************/

//======== Self Include ================================================================//
#include "SystemMonitor.hpp"
//======== 1st Party Includes ==========================================================//
#include "../Utility/Debug.hpp"
//======== 3rd Party Includes ==========================================================//
#include <iomanip>
#include "../SceneManagement/SceneSystem.hpp"
#include <SystemBundle.hpp>

//======== Types =======================================================================//
struct monitoring_systemData
{
	bool shouldGenStrings = true;
	std::string fps_string;
	float fps_measure;
};

//======== Defines =====================================================================//

//======== Static Declarations =========================================================//
static float updateFPS(float p_old_measure, float p_current_frame);

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

void systemMonitor::showFPS(bool p_visible)
{
	m_show_fps = p_visible;
}

void systemMonitor::showCounts(bool p_visible)
{
	m_show_counts = p_visible;
}

//======== Functionality ===============================================================//
#if _DEBUG
void systemMonitor::onEndFrame()
{
	
	static std::unordered_map<systemBase*, monitoring_systemData> nameStrings;
	unsigned int count = 0;
	
	for (auto l_bundle_name : getSystemManager()->getActiveBundleNames())
	{
		auto * l_bundle = getSystemManager()->getBundle(l_bundle_name);
		auto l_system_names = getSystemManager()->getSystemNamesInBundle(l_bundle_name);
		for (auto l_system_name : l_system_names)
		{
			systemBase * l_sys = getSystemManager()->getSystem(l_system_name);
			monitoring_systemData & l_sys_entry = nameStrings[l_sys];
			// measure
			float l_current_fps = 1.0f / l_bundle->getDt();

			// calculate
			l_sys_entry.fps_measure = updateFPS(l_sys_entry.fps_measure, l_current_fps);

			// display
			if (m_show_fps)
			{
				if (l_sys_entry.shouldGenStrings)
				{
					l_sys_entry.fps_string = l_system_name + " FPS: ";
					l_sys_entry.shouldGenStrings = false;
				}
				std::stringstream ss;
				ss << l_sys_entry.fps_string << std::fixed << std::setprecision(2) << l_sys_entry.fps_measure;
				debug::displayText(ss.str(), "arial", -500, 350 - (count * 22.0f), .5f, color::cyan);
			}
			count++;
		}
	}

	if(m_show_counts)
	{
		sceneSystem * l_scene_sys = getSystemManager()->getSystem<sceneSystem>();
		std::stringstream ss;
		ss << l_scene_sys->getSpaceCount() << " spaces";
		debug::displayText(ss.str(), "arial", 0, 350, 0.5f, color::cyan);
		ss.str("");
		ss << l_scene_sys->getGameObjectCount() << "  objects";
		debug::displayText(ss.str(), "arial", 0, 325, 0.5f, color::cyan);
	}
}
#endif

//======== Helper Functions ============================================================//

static float updateFPS(float p_old_measure, float p_current_frame)
{
		constexpr float smoothing = 0.975f; // larger = more smoothing
		constexpr float one_minus_smoothing = 1.0f - smoothing;
		float l_new_measure = (p_old_measure * smoothing) + (p_current_frame * one_minus_smoothing);
		return l_new_measure;	
}
