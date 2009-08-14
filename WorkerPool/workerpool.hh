#ifndef __MASTER_WORKER_POOL_H__
#define __MASTER_WORKER_POOL_H__

#include <base/base.hh>
#include "Worker/worker.hh"

#include <queue>

namespace optimaster
{
	class WorkerPool : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::deque<Worker> idleWorkers;
			std::deque<Worker> activeWorkers;
			
			std::map<std::string, size_t> activeWorkersPerJob;
			
			std::map<std::string, Worker> allWorkers;
			
			void workerFinished(Worker &worker);
			void removeWorker(Worker &worker);

			void onWorkerClosed(int fd, Worker worker);
			void onWorkerFailed(optimization::messages::worker::Response::Failure &failure, Worker worker);
			void onWorkerSuccess(Worker::SuccessArgs &args, Worker worker);
			
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
			bool activateWorker(Job &job, optimization::Solution &solution, Worker &worker);
			
			size_t active(Job &job) const;
			
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
