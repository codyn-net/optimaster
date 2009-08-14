#include "workerpool.ih"

void WorkerPool::Data::workerFinished(Worker &worker) 
{
	/* Add worker back to idle pool */
	idleWorkers.push_back(worker);
	activeWorkers.erase(std::remove(activeWorkers.begin(), activeWorkers.end(), worker), activeWorkers.end());
	--(activeWorkersPerJob[worker.job().name()]);
	
	onWorkerIdle();
}
