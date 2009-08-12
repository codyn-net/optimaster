#include "workerpool.ih"

void WorkerPool::Data::workerFinished(Worker &worker) 
{
	if (worker.failures() >= 5)
	{
		/* Remove worker permanently, failed too many times */
		debug_worker << "Worker failed to many times (removing)" << endl;
		removeWorker(worker);
	}
	else
	{
		/* Add worker back to idle pool */
		idleWorkers.push_back(worker);
		activeWorkers.erase(std::remove(activeWorkers.begin(), activeWorkers.end(), worker), activeWorkers.end());
		--(activeWorkersPerJob[worker.job().name()]);
		
		onWorkerIdle();
	}
}
