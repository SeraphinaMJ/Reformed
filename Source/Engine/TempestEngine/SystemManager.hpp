/*!***************************************************************************************
\file       SystemManager.hpp
\author     Michael-Paul Moore
\date       6/29/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This is the interface for system manager. Give it systems. Tell it to initialize
        them, then tell it to run them.
*****************************************************************************************/

#pragma once

//========1st Party Includes============================================================//
#include "Configuration.hpp"
#include "Threading/JobManager.hpp"
#include "SystemManagement/SystemManagerInterface.hpp"

//========3rd Party Includes============================================================//
#include <map>
#include <vector>

//========Forward Deceleration==========================================================//
class gameObjectGatherer;
class systemBundle;
class systemBase;


////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: systemManager
\brief   Manages systems.
*****************************************************************************************/
class systemManager : public systemManagerInterface
{
	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////
	public:

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

		/*!***************************************************************************************
		\brief  Constructs the job manager to be set 7 threads. TODO: make it try to use
		        available - 1.
		*****************************************************************************************/
		systemManager();

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Initialises the system manager and provides access to the configuration information.
		\param  p_conf_function - pointer to a function that retrieves the configuration information.
		*****************************************************************************************/
		void initialize(std::function<configuration ()> p_conf_function) override;

		/*!***************************************************************************************
		\brief  Receives system bundles to add.
		\param  system_bundles - bundles to add
		*****************************************************************************************/
		void addSystemBundles(const std::vector<systemBundle*>& system_bundles) override;

		/*!***************************************************************************************
		\brief  Initializes system bundles given by name.
		\param  system_bundle_names - bundle names to initialize
		\return Game object gatherer.
		*****************************************************************************************/
		void initializeSystemBundles(const std::vector<std::string> & system_bundle_names) override;

		/*!***************************************************************************************
		\brief  Calls job manager update
		*****************************************************************************************/
		void update() override;

		/*!***************************************************************************************
		\brief  Holds main thread till initialization is finished. Don't use this outside main
		        thread!
		\param  system_bundle_names - names of the bundles to wait for
		*****************************************************************************************/
		void waitForBundleInitialization(std::vector<std::string> const& system_bundle_names);

		/*!***************************************************************************************
		\brief  Start task for normal running for system bundles.
		\param  system_bundle_names - to start
		*****************************************************************************************/
		void activateSystemBundles(const std::vector<std::string> & system_bundle_names) override;

		/*!***************************************************************************************
		\brief  Sets game object gatherer.
		\param  gatherer - to set too
		*****************************************************************************************/
		void setGameObjectGatherer(gameObjectGatherer * gatherer) override;

		/*!***************************************************************************************
		\brief  Gets game object gatherer.
		\return Game object gatherer.
		*****************************************************************************************/
		gameObjectGatherer * getGameObjectGatherer() const override;

		/*!***************************************************************************************
		\brief  For getting a system by name.
		\param system_name - that you are trying to get
		\return Pointer to system
		*****************************************************************************************/
		systemBase * getSystem(const std::string & system_name) override;

		/*!***************************************************************************************
		\brief  Template version, does casting for you and doesn't require name just 'SYSTEM_NAME'
		implemented for T
		\return Pointer to system
		*****************************************************************************************/
		template< typename T >
		T * getSystem() { return reinterpret_cast<T *>(getSystem(T::getName())); }

		/*!***************************************************************************************
		\brief  Determines if a system is current active and running
		\param p_system_ptr - pointer to the system to check
		\return bool - true if the system is active, false otherwise
		*****************************************************************************************/
		bool isSystemActive(systemBase* p_system_ptr) override;

		/*!***************************************************************************************
		\brief  Sends a task to the job manager via the system manager.
		\param p_task - that you are trying to send
		*****************************************************************************************/
		void sendTask(task& p_task) override;

		/*!***************************************************************************************
		\brief  Cleans up system manager.
		*****************************************************************************************/
		void shutdown() override;

		/*!***************************************************************************************
		\brief  Retrieves the function that can retrieve the configuration settings.
		\return std::function<configuration ()> - function that retrieves the configuration
		        settings.
		*****************************************************************************************/
		configurationFunctionPtr getConfigurationFunction() const override;

		/*!***************************************************************************************
		\brief  Retrieves the names of all bundles in the system
		\return std::vector<std::string> - vector of all bundle names
		*****************************************************************************************/
		std::vector<std::string> getAllBundleNames() const override;

		/*!***************************************************************************************
		\brief  Retrieves the names of all active bundles in the system
		\param std::vector<std::string> - vector of all active bundle names
		*****************************************************************************************/
		std::vector<std::string> getActiveBundleNames() const override;

		/*!***************************************************************************************
		\brief  Activates a specific bundle.  No effect if bundle is already active.
		\param p_bundle_name - the name of the bundle to attempt activation.
		*****************************************************************************************/
		void enableBundle(const std::string & p_bundle_name) override;

		/*!***************************************************************************************
		\brief  Deactivates a specific bundle.  No effect if bundle is already inactive.
		\param p_bundle_name - the name of the bundle to attempt deactivation.
		*****************************************************************************************/
		void disableBundle(const std::string & p_bundle_name) override;

		void suspendBundle(const std::string & p_bundle_name) override;

		void sendMessage(std::string p_sender, std::string p_destination, unsigned int p_message_code, void * p_data) override;

		/*!***************************************************************************************
		\brief  Retrieves a bundle by name
		\param  system_name - name of system to find
		\return systemBundle * - Returns a pointer to the bundle requested, or nullptr if not found.
		*****************************************************************************************/
		systemBundle * getBundle(const std::string & p_name) override;


	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
	private:

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

		/*!***************************************************************************************
		\brief  Activates a bundle by adding to the job manager.
		\param  system_name - name of system to add
		*****************************************************************************************/
		void activateSystemBundle(const std::string &system_name);

		/*!***************************************************************************************
		\brief  Shuts down a bundle by removing it from the job manager.
		\param  system_name - name of system to shutdown
		\param  p_bundle - Pointer to bundle to be shutdown
		*****************************************************************************************/
		void shutdownSystemBundle(const std::string & p_name, systemBundle * p_bundle);

		/*!***************************************************************************************
		\brief  Suspends a bundle by suspending the task
		\param  system_name - name of system to suspend
		*****************************************************************************************/
		void suspendSystemBundle(const std::string & p_name);

		/*!***************************************************************************************
		\brief  Tells all systems bundles to shutdown.
		*****************************************************************************************/
		void sendShutdownAllSystemBundles();

		/*!***************************************************************************************
		\brief  Makes sure all systems are activated (used for shutdown process)
		*****************************************************************************************/
		void activateAllSystemBundles();

		/*!***************************************************************************************
		\brief  Checks if a system is already active.
		\param p_system_bundle - system bundle to check if it is active
		\return True if the system is active, false if it is not.
		*****************************************************************************************/
		bool alreadyActive(const systemBundle* const p_system_bundle) const;

		/*!***************************************************************************************
		\brief  Destroys all system bundles
		*****************************************************************************************/
		void destroyAllSystems();

		/////================================================================================/////
		/////     Data                                                                       /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Configurable                                                            ////
		////==================================================================================////

		//======================================================================================//
		//        Systems                                                                       //
		//======================================================================================//
		std::map<std::string, std::shared_ptr<systemBundle>> m_sys_bndl_mast;         /*!< AKA: member system
		                                                              * bundle master. Master
		                                                              * list of system bundles
		                                                              * (string is the group
		                                                              * name) */
		std::map<std::string, std::shared_ptr<systemBase>>   m_system_master;         /*!< master list of systems
		                                                              * (string is the system
		                                                              * name) */
		std::vector<systemBundle*>           m_active_system_bundles; /*!< List of active system
		                                                              * bundles. */

		//======================================================================================//
		//        Misc.                                                                         //
		//======================================================================================//

		configurationFunctionPtr m_conf_function; /*!< function that can retrieve the
		                                                * configuration settings. */
		gameObjectGatherer* m_go_gatherer; /*!< the gatherer for collecting lists of game
		                                    * objects */
		jobManager          m_jobManager;  //!< job manager that system  manager owns
};
