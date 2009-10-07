#ifndef __MASTER_APPLICATION_H__
#define __MASTER_APPLICATION_H__

#include <optimization/Discovery/discovery.hh>

#include "WorkerPool/workerpool.hh"
#include "TaskQueue/taskqueue.hh"

#include <glibmm.h>

namespace optimaster
{
	class Application
	{
		optimization::Discovery d_discovery;

		WorkerPool d_workerPool;
		TaskQueue d_taskQueue;

		sigc::connection d_idleWork;
		Glib::RefPtr<Glib::MainLoop> d_mainLoop;
		
		public:
			/* Constructor/destructor */
			Application(int &argc, char **&argv);

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
	};
}

#endif /* __MASTER_APPLICATION_H__ */
