/*!***************************************************************************************
\file       JobManager.cpp
\author     Michael-Paul Moore
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This is the implementation for the JobManager class.
*****************************************************************************************/

//========Self Include==================================================================//
#include "JobManager.hpp"

////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

		////////==========================================================================////////
		////////  Class                                                                   ////////
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

				jobManager::job::job() {}

				//======================================================================================//
				//        Getters & Setters                                                            ///
				//======================================================================================//

				thread_number jobManager::job::getAffinity() const
				{
					return m_tasks.front()->getAffinity();
				}

				void jobManager::job::setAfinity(const thread_number & afinity)
				{
					for (const auto & task : m_tasks)
						task->setAffinity(afinity);
				}

				bool jobManager::job::shouldReschedule() const
				{
					return m_tasks.front()->isRescheduled();
				}

				void jobManager::job::giveTask(task * new_task)
				{
					m_tasks.push_back(new_task);

					// TODO(MP): Implement for auto task grouping
				}

				//======================================================================================//
				//        Helper                                                                        //
				//======================================================================================//

				bool jobManager::job::aged()
				{
					return m_tasks.front()->aged();
				}

				void jobManager::job::clear()
				{
					for (const auto & task : m_tasks)
						task->markRemoved();
					m_tasks.clear();
				}

				bool jobManager::job::empty() const
				{
					return m_tasks.empty();
				}

				void jobManager::job::update()
				{
					for (auto& task : m_tasks) // update all the tasks
						task->onUpdate();
				}

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

		jobManager::jobManager() : m_worker_count(setThreadCound()),
		                           m_workers( new worker[m_worker_count]) {}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void jobManager::giveTask(task* p_task)
		{
			p_task->markAdded();
			job* newJob = new job;
			newJob->giveTask(p_task);
			m_jobs.first.lock();
			m_jobs.second.push_back(newJob);
			m_jobs.first.unlock();
		}

		void jobManager::update()
		{
			for (thread_number i = 0; i < m_worker_count; ++i)
			{
				worker& current_worker = m_workers[i];
				if (!current_worker.isWorking()) // check if finished working
				{
					if (current_worker.hasSomethingToDo()) // ad finished job back to list
						insertJob(current_worker.submitWork());

					m_jobs.first.lock();
					bool jobs_empty = m_jobs.second.empty();
					m_jobs.first.unlock();

					if(!jobs_empty)
					{
						m_jobs.first.lock();
						current_worker.giveJob(chooseJob(i)); // give job to worker
						m_jobs.first.unlock();
					}
				}
			}

			// main thread jobs
			m_jobs.first.lock();
			bool jobs_empty = m_jobs.second.empty();
			m_jobs.first.unlock();

			if (!jobs_empty)
			{
				m_jobs.first.lock();
				job * const job_pointer = chooseJob(MAIN_THREAD_AFFINITY); // give job to worker
				m_jobs.first.unlock();
				if (job_pointer) // job was chosen
				{
					job_pointer->update();
					insertJob(job_pointer);
				}
			}
		}

		//======================================================================================//
		//        Destructor                                                                   ///
		//======================================================================================//

		jobManager::~jobManager()
		{
			for (thread_number i = 0; i < m_worker_count; ++i)
			{
				m_workers[i].fire();
				if(m_workers[i].getThread().joinable()) // join the joinable ones
					m_workers[i].getThread().join();
			}
			for (const auto & it_job : m_jobs.second) // don't need locking we are shutting down
				delete it_job;
			delete [] m_workers;
		}

	///////============================================================================///////
	///////    Private                                                                 ///////
	///////============================================================================///////

		////////==========================================================================////////
		////////  Class                                                                   ////////
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
				//        Getters & Setters                                                            ///
				//======================================================================================//

				void jobManager::worker::fire()
				{
					m_fired = true;
				}

				bool jobManager::worker::hasSomethingToDo() const
				{
					return m_job_in_use;
				}

				jobManager::job * jobManager::worker::submitWork()
				{
					job* return_value = m_job_in_use;
					m_job_in_use = nullptr;
					return return_value;
				}

				void jobManager::worker::giveJob(job * const p_job)
				{
					m_job_in_use = p_job;

					if (p_job)
						m_working = true;
				}

				bool jobManager::worker::isWorking() const
				{
					return m_working;
				}

				std::thread & jobManager::worker::getThread()
				{
					return m_thread;
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
				void jobManager::worker::work()
				{
 					while (!m_fired)
						if (m_working)
						{
							m_job_in_use->update();
							m_working = false;
						}
						else
						{
							// this helps prevent busy waiting, but could hurt performance
							std::this_thread::yield();
						}
				}

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

		jobManager::job * jobManager::chooseJob(const thread_number & curr_thrd)
		{
			for (auto it_jb_ptr = m_jobs.second.begin(); it_jb_ptr != m_jobs.second.end(); ++it_jb_ptr)
			{
				job * const current_job = *it_jb_ptr;
				// TODO(MP): First check if it is scheduled
				if (!current_job->aged())
					continue;

				if (current_job->getAffinity() == NOT_SET_AFFINITY)
					current_job->setAfinity(curr_thrd);

				if(curr_thrd == MAIN_THREAD_AFFINITY && current_job->getAffinity() != MAIN_THREAD_AFFINITY)
					continue; // this causes shuttering?

				// check if this is the correct processor
				if (current_job->getAffinity() == NO_AFFINITY || current_job->getAffinity() == curr_thrd)
				{
					m_jobs.second.erase(it_jb_ptr); // removes it from list
					return current_job;
				}
			}

			return nullptr;
		}

		void jobManager::insertJob(job * p_job)
		{
			if (p_job->shouldReschedule())
			{
				m_jobs.first.lock();
				m_jobs.second.push_back(p_job);
				m_jobs.first.unlock();
			}
			else
			{
				p_job->clear();
				delete p_job;
			}
		}

		thread_number jobManager::setThreadCound()
		{
			thread_number result = static_cast<thread_number>(std::thread::hardware_concurrency()); // may return 0 if fails
			if (!result)
				return 1;
			return result;
		}
