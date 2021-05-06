/*!***************************************************************************************
\file       SystemTask.hpp
\author     Michael-Paul Moore
\date       6/29/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  System task class implementation.
*****************************************************************************************/

//========Self Include==================================================================//
#include "SystemTask.hpp"

//========1st Party Includes============================================================//
#include "SystemBase.hpp"
#include "SystemBundle.hpp"
#include "SystemManager.hpp"

////////==========================================================================////////
////////  Class (systemTask)                                                      ////////
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

		systemTask::systemTask(const systemBundle & system_bundle,
			const thread_number & p_afinity)
			: task(p_afinity), m_sys_bndl(system_bundle) {}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void systemTask::waitForInitialization() const
		{
			// wait till it is done initializing
			while (m_state < st_initialized)
			{
				m_system_manager->update();
			}
			while (!isRemoved())
			{
				m_system_manager->update();
			}
		}

		void systemTask::sendInitialize(systemManagerInterface * system_manager)
		{
			m_system_manager = system_manager;
			m_to_run = &systemTask::initialize;
			markUnschedule();
			m_state = st_initializing;
		}

		void systemTask::sendRun()
		{
			waitForInitialization(); // Should freeze if uninitialized.

			m_to_run = &systemTask::update;
			markReschedule();
			m_state = st_running;
		}

		void systemTask::sendSuspend()
		{
			markUnschedule();
			m_state = st_initialized;
		}

		void systemTask::sendShutdown()
		{
			m_to_run = &systemTask::shutDown;

			// done now so if someone queries the state they don't redo this step
			m_state = st_shutting_down;
		}

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		////==================================================================================////
		////      Overridden                                                                  ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void systemTask::onUpdate()
		{
			if (m_to_run)
				(*this.*m_to_run)();
		}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		void systemTask::initialize()
		{
			for (auto& sys : m_sys_bndl.sysVec())
				sys->initialize(m_system_manager);
			m_to_run = nullptr;
			m_state = st_initialized;
			markUnschedule();
		}

		void systemTask::update()
		{
		}

		void systemTask::shutDown()
		{
			// backwards iteration
			for (auto sys_it = m_sys_bndl.sysVec().rbegin(); sys_it != m_sys_bndl.sysVec().rend(); )
			{
				(*sys_it)->shutdown();
				++sys_it;
			}

			m_to_run = nullptr; // now it can't be accidentally run again
			markUnschedule(); // to get the job manager to remove it
			m_state = st_shut_down;
		}
