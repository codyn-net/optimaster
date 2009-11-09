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
#include "optimizermanager.hh"

#include <glibmm.h>
#include <network/network.hh>

namespace optimaster
{
	class Application
	{
		optimization::Discovery d_discovery;

		OptimizerManager d_optimizerManager;
		WorkerManager d_workerManager;

		TaskQueue d_taskQueue;

		sigc::connection d_idleDispatch;
		Glib::RefPtr<Glib::MainLoop> d_mainLoop;

		public:
			/* Constructor/destructor */
			Application(int &argc, char **&argv);
			~Application();

			/* Public functions */
			void Run(Glib::RefPtr<Glib::MainLoop> loop);
		private:
			/* Private functions */
			void ParseArguments(int &argc, char **&argv);
			void EnableEnvironment();

			void SendWakeup();
			void OnGreeting(optimization::Discovery::Info &info);
			bool OnInterrupt(Glib::RefPtr<Glib::MainLoop> loop);

			void IdleDispatch();
			bool OnDispatch();

			void OnWorkerAdded(Worker &worker);
			void OnWorkerRemoved(Worker &worker);
			void OnWorkerCommunication(Worker::CommunicationArgs &args);
			void OnWorkerTimeout(Worker &worker);

			void OnOptimizerAdded(Optimizer &optimizer);
			void OnOptimizerRemoved(Optimizer &optimizer);
			void OnOptimizerCommunication(Optimizer::CommunicationArgs &args);

			void HandleOptimizerBatch(Optimizer                                         &optimizer,
			                          optimization::messages::task::Communication const &communication);
			void HandleOptimizerToken(Optimizer                                         &optimizer,
			                          optimization::messages::task::Communication const &communication);

			void OnNotifyAvailable();
	};
}

#endif /* __MASTER_APPLICATION_H__ */
