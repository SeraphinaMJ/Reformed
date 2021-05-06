/*!***************************************************************************************
\file       SystemBundle.hpp
\author     Michael-Paul Moore
\date       6/24/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  System bundle interface, send this to system manager acts like plug-ins for
        systems!
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "SystemTask.hpp"
//======== 3rd Party Includes ==========================================================//
#include <map>
//======== Types =======================================================================//

using HRC_time_point_t = decltype(std::chrono::high_resolution_clock::now());

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class systemBase;

/*!***************************************************************************************
\par class: systemBundle
\brief   This is the system bundle, sent to the system manager.
         1) create your named bundle
         2) Input your systems in pairs, <name, system>, in the order you want them to run
*****************************************************************************************/
class systemBundle final
{
public:

	/*!***************************************************************************************
	\brief  Only constructs a bundle given a name and systems you want to put into it.  Order
          will be preserved when executing the update sequence of the systems
	\param bundle_name - name of the bundle being constructed
	\param systems - vector of systems paired with names
	\param thread_afinity - affinity setting to set thread
	*****************************************************************************************/
	systemBundle(const std::string& p_bundle_name,
	             const std::vector<std::pair<std::string, systemBase*>>&& p_systems,
	             const thread_number && p_thread_afinity = NO_AFFINITY);

	/*!***************************************************************************************
	\brief  Geter for system bundle's name.
	\return Name of system bundle.
	*****************************************************************************************/
	const std::string& name() const;

	/*!***************************************************************************************
	\brief  Getter for system task.
	\return Task associated with bundle.
	*****************************************************************************************/
	systemTask & task();

	/*!***************************************************************************************
	\brief  Getter for system task.
	\return Task associated with bundle.
	*****************************************************************************************/
	const systemTask & task() const;

	/*!***************************************************************************************
	\brief  Geter for systems in map form.
	\return Systems as a map.
	*****************************************************************************************/
	const std::map<std::string, systemBase*>& systemMap() const;

	/*!***************************************************************************************
	\brief  Geter for systems in vector form.
	\return Systems as a vector.
	*****************************************************************************************/
	const std::vector<systemBase*>& sysVec() const;

	/*!***************************************************************************************
	\brief  Allows a system to specify it's own frame rate (default is 1/60 seconds)
	\param p_frame_time - the new desired frame time for this system.
	*****************************************************************************************/
	void setDesiredFrameTime(float p_frame_time);

	/*!***************************************************************************************
	\brief  Updates the delta time for the frame
	\param p_current_time - The current execution time of this frame
	\param p_core_dt - the amount of time changed from last call
	*****************************************************************************************/
	void updateDT(const HRC_time_point_t & p_current_time, float p_core_dt);

	/*!***************************************************************************************
	\brief  Gets the delta time for this system as a float.
	\return Delta Time in seconds.
	*****************************************************************************************/
	float getDt() const;

	void initialize(systemManagerInterface * p_system_manager);
	
	void update();

	/*!***************************************************************************************
	\brief  Destructor.
	*****************************************************************************************/
	~systemBundle();

private:

	/*!***************************************************************************************
	\brief  Creates a map of systems given a vector of pair<std::string, systemBase*>s.
	\param systems - system pairs to turn into a map
	\return The map it created.
	*****************************************************************************************/
	std::map<std::string, systemBase*> createMap(
	                   const std::vector<std::pair<std::string, systemBase*>>& systems) const;

	/*!***************************************************************************************
	\brief  Creates a vector<systemBase*> given a vector of pair<std::string, systemBase*>s.
          Ordering of the entries will be preserved.
	\param systems - system pairs to turn into a map
	\return The vector it created.
	*****************************************************************************************/
	std::vector<systemBase*> createVector(
	                       const std::vector<std::pair<std::string, systemBase*>>& systems) const;

	systemTask                               m_task;               /*!< task corresponding to this
	                                                               * bundle */
	const std::string                        m_bundle_name;        /*!< name of this bundle */
	const std::vector<systemBase*>           m_systems_in_vector; /*!< systems stored in a
	                                                               * vector */
	const std::map<std::string, systemBase*> m_systems_in_map;     //!< systems stored in a map

	// Framerate control functionality
	float m_desired_frame_time = 1.0f / 30.0f; //!< desired frame time for this system. 1/30 seconds default.
	float m_dt = 0.0f; //!< current stored delta time in seconds
	bool m_should_run_this_call = true; //!< flag to indicate if the derived methods should be called this value of dt
	float m_time_to_next_frame = 0.0f; //!< amount of time remaining until next frame
	HRC_time_point_t m_lastTime	= std::chrono::high_resolution_clock::now(); //!< Last frames start

};
