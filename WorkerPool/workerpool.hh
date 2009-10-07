#ifndef __MASTER_WORKER_POOL_H__
#define __MASTER_WORKER_POOL_H__

#include <base/base.hh>
#include "Worker/worker.hh"

#include <deque>

namespace optimaster
{
	class WorkerPool : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::deque<Worker> idleWorkers;
			std::deque<Worker> activeWorkers;
			
			std::map<std::string, Worker> allWorkers;
			
			void removeWorker(Worker &worker);

			void onWorkerClosed(int fd, Worker worker);
			void onWorkerResponse(optimization::messages::task::Response &response, Worker worker);
			
			base::signals::Signal<> onWorkerIdle;
			base::signals::Signal<Worker> onWorkerRemoved;
		};

		Data *d_data;

		public:
			/* Constructor/destructor */
			WorkerPool();
		
			/* Public functions */
			Worker add(std::string const &connection);
			
			bool hasIdleWorkers() const;
			bool activate(taskqueue::Task &task);
			
			void close();

			base::signals::Signal<> &onWorkerIdle();
			base::signals::Signal<Worker> &onWorkerRemoved();
		private:
			/* Private functions */
			std::string specifier(std::string const &protocol, std::string const &host, std::string const &port) const;
	};
	
	inline base::signals::Signal<> &WorkerPool::onWorkerIdle()
	{
		return d_data->onWorkerIdle;
	}
	
	inline base::signals::Signal<Worker> &WorkerPool::onWorkerRemoved()
	{
		return d_data->onWorkerRemoved;
	}
	
	inline bool WorkerPool::hasIdleWorkers() const
	{
		return d_data->idleWorkers.size() != 0;
	}
}

#endif /* __MASTER_WORKER_POOL_H__ */
