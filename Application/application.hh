/*
 *  application.hh - This file is part of optimaster
 *
 *  Copyright (C) 2009 - Jesse van den Kieboom
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef __MASTER_APPLICATION_H__
#define __MASTER_APPLICATION_H__

#include <optimization/Discovery/discovery.hh>

#include "WorkerPool/workerpool.hh"
#include "TaskQueue/taskqueue.hh"
#include "OptimizerManager/optimizermanager.hh"

#include <glibmm.h>
#include <network/network.hh>

namespace optimaster
{
	class Application
	{
		optimization::Discovery d_discovery;
		OptimizerManager d_optimizerManager;

		WorkerPool d_workerPool;
		TaskQueue d_taskQueue;

		sigc::connection d_idleWork;
		Glib::RefPtr<Glib::MainLoop> d_mainLoop;
		
		public:
			/* Constructor/destructor */
			Application(int &argc, char **&argv);
			~Application();

			/* Public functions */		
			void run(Glib::RefPtr<Glib::MainLoop> loop);
		private:
			/* Private functions */
			void parseArguments(int &argc, char **&argv);
			bool onGreeting(optimization::Discovery::Info &info);
			
			void registerWorker(std::string const &connection);
			bool onInterrupt(Glib::RefPtr<Glib::MainLoop> loop);
			
			void onIdleWork();
			bool onDispatch();
			
			void onWorkerRemoved(Worker &worker);

			void onWorkerResponse(optimization::messages::task::Response &response, Worker worker);
			void onWorkerChallenge(std::string &challenge, Worker worker);
			
			void sendWakeup();
			void enableEnvironment();
			//std::string hashToken(Job const &job, std::string const &challenge);
			
			bool onOptimizerClosed(Optimizer &optimizer);
			bool onOptimizerBatch(OptimizerManager::BatchArgs &args);
	};
}

#endif /* __MASTER_APPLICATION_H__ */
