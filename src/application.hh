/*
 * application.hh
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

#ifndef __OPTIMASTER_APPLICATION_H__
#define __OPTIMASTER_APPLICATION_H__

#include <optimization/discovery.hh>

#include "workermanager.hh"
#include "taskqueue.hh"
#include "jobmanager.hh"
#include "command.hh"

#include <glibmm.h>
#include <jessevdk/network/network.hh>

namespace optimaster
{
	class Application
	{
		optimization::Discovery d_discovery;

		JobManager d_jobManager;
		WorkerManager d_workerManager;

		Command d_command;

		TaskQueue d_taskQueue;

		sigc::connection d_idleDispatch;
		Glib::RefPtr<Glib::MainLoop> d_mainLoop;

		size_t d_tasksFailed;
		size_t d_tasksSuccess;
		std::map<std::string, bool> d_activeUsers;

		public:
			/* Constructor/destructor */
			Application(int &argc, char **&argv);
			~Application();

			/* Public functions */
			void Run(Glib::RefPtr<Glib::MainLoop> loop);

			JobManager const &Manager() const;
			JobManager &Manager();
		private:
			/* Private functions */
			void ParseArguments(int &argc, char **&argv);
			void EnableEnvironment();

			void SendWakeup();
			void OnGreeting(optimization::Discovery::Info &info);
			bool OnInterrupt(Glib::RefPtr<Glib::MainLoop> loop);

			void IdleDispatch();
			bool OnDispatch();

			bool OnPeriodicLogStatus();

			void OnWorkerAdded(Worker &worker);
			void OnWorkerRemoved(Worker &worker);
			void OnWorkerCommunication(Worker::CommunicationArgs &args);
			void OnWorkerTimeout(Worker &worker);

			void OnJobAdded(Job &job);
			void OnJobRemoved(Job &job);
			void OnJobCommunication(Job::CommunicationArgs &args);

			void HandleJobIdentify(Job                                               &job,
			                       optimization::messages::task::Communication const &communication);

			void HandleJobBatch(Job                                               &job,
			                    optimization::messages::task::Communication const &communication);
			void HandleJobToken(Job                                               &job,
			                    optimization::messages::task::Communication const &communication);

			void OnNotifyAvailable();
			std::string FailureToString(optimization::messages::task::Response::Failure const &failure) const;

			void LogStatus() const;
	};
}

#endif /* __MASTER_APPLICATION_H__ */
