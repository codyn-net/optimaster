#include "workerpool.ih"

void WorkerPool::Data::onWorkerResponse(task::Response &response, Worker worker) 
{
	idleWorkers.push_back(worker);

	activeWorkers.erase(std::remove(activeWorkers.begin(), activeWorkers.end(), worker), activeWorkers.end());
	onWorkerIdle();
}
