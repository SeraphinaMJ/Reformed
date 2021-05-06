/*!***************************************************************************************
\file       Task.cpp
\author     Michael-Paul Moore
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This is the implementation for the task class
*****************************************************************************************/

//========Self Include==================================================================//
#include "Task.hpp"

////////==========================================================================////////
////////  Class (Task)                                                            ////////
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

		task::task(const thread_number & p_affinity, bool p_reschedule)
		          : m_afinity(p_affinity), m_reschedule(p_reschedule) {}

		task::task(const fps & max_fps, const thread_number & p_affinity) : m_afinity(p_affinity),
		                                                                    m_reschedule(true),
		                                                m_max_frq(FPStoFrequency(max_fps)) {}

		//======================================================================================//
		//        Getters & Setters                                                            ///
		//======================================================================================//

		thread_number task::getAffinity() const
		{
			return m_afinity;
		}

		void task::setAffinity(thread_number affinity)
		{
			m_afinity = affinity;
		}

		bool task::isRescheduled() const
		{
			return m_reschedule;
		}

		void task::markUnschedule()
		{
			m_reschedule = false;
		}

		void task::markReschedule()
		{
			m_reschedule = true;
		}

		bool task::isRemoved() const
		{
			return m_removed;
		}

		void task::markAdded()
		{
			m_removed = false;
		}

		void task::markRemoved()
		{
			m_removed = true;
		}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		bool task::aged()
		{
			if ((m_nxt_tk - std::chrono::high_resolution_clock::now()).count() <= 0)
			{
				m_nxt_tk = std::chrono::high_resolution_clock::now() + m_max_frq;
				return true;
			}
			return false;
		}
