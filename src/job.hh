/*
 * job.hh
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

#ifndef __OPTIMASTER_JOB_H__
#define __OPTIMASTER_JOB_H__

#include <jessevdk/network/network.hh>
#include <optimization/messages.hh>
#include <map>
#include <vector>

#include "worker.hh"

namespace optimaster
{
	class Job : public Communicator
	{
		public:
			/* Constructor/destructor */
			Job();
			Job(jessevdk::network::Client &client);
			
			void Add(Worker &worker);
			std::vector<Worker> &ActiveWorkers();
			
			double AverageRunTime() const;

			std::string const &Name() const;
			void SetName(std::string const &name);

			std::string const &User() const;
			void SetUser(std::string const &user);

			double Priority() const;
			double Timeout() const;

			void SetTimeout(double timeout);
			void SetPriority(double priority);

			size_t TasksFailed() const;
			void SetTasksFailed(size_t num);

			size_t TasksSuccess() const;
			void SetTasksSuccess(size_t num);

			double Progress() const;
			void SetProgress(double progress);

			Glib::TimeVal const &Started() const;
		private:
			struct Data : public Communicator::Data
			{
				std::vector<Worker> activeWorkers;
				void OnWorkerDeactivated(Worker &worker);
				
				std::list<double> lastRunTimes;

				std::string name;
				std::string user;
				double priority;
				double timeout;

				size_t tasksFailed;
				size_t tasksSuccess;

				double progress;

				Glib::TimeVal started;
			};

			Data *d_data;
	};
}

#endif /* __MASTER_JOB_H__ */
