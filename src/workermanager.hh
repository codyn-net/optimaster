/*
 * workermanager.hh
 * This file is part of optimaster
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
 *
 * optimaster is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * optimaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with optimaster; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#ifndef __OPTIMASTER_WORKER_MANAGER_H__
#define __OPTIMASTER_WORKER_MANAGER_H__

#include <jessevdk/base/base.hh>
#include "worker.hh"
#include "task.hh"

#include <deque>
#include <map>

namespace optimaster
{
	class WorkerManager
	{
		std::deque<Worker> d_idleWorkers;
		std::map<size_t, Worker> d_activeWorkers;
		std::map<size_t, Worker> d_workers;

		std::map<std::string, Worker> d_workersHash;

		public:
			~WorkerManager();

			bool Add(std::string const &connection, Worker &worker);
			bool Add(std::string const &connection);

			bool Find(size_t id, Worker &worker);

			bool Idle(Worker &worker);

			size_t Size() const;
			size_t Active() const;

			/**
			  * @brief OnAdded signal
			  *
			  * Signal emitted when a worker was added to the manager.
			  */
			jessevdk::base::signals::Signal<Worker> OnAdded;
			
			/**
			  * @brief OnRemoved signal
			  *
			  * Signal emitted when a worker was removed to the manager.
			  */
			jessevdk::base::signals::Signal<Worker> OnRemoved;
			
			/**
			  * @brief OnNotifyAvailable signal
			  *
			  * Signal emitted when a worker has become idle
			  */
			jessevdk::base::signals::Signal<> OnNotifyAvailable;
		private:
			/* Private functions */
			void OnWorkerClosed(Communicator &communicator);
			void RemoveWorker(Worker &worker);

			void OnWorkerActivated(Worker &worker);
			void OnWorkerDeactivated(Worker &worker);
	};
}

#endif /* __OPTIMASTER_WORKER_MANAGER_H__ */
