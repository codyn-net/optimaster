#include "workerpool.ih"

void WorkerPool::Data::removeWorker(Worker &worker) 
{
	// Remove from active list
	deque<Worker>::iterator iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);
	activeWorkers.erase(iter, activeWorkers.end());
	
	// Remove from idle list
	iter = std::remove(idleWorkers.begin(), idleWorkers.end(), worker);
	idleWorkers.erase(iter, idleWorkers.end());

	// Remove from all workers
	map<string, Worker>::iterator miter;
	
	for (miter = allWorkers.begin(); miter != allWorkers.end(); ++miter)
	{
		if (miter->second == worker)
		{
			allWorkers.erase(miter);
			break;
		}
	}

	worker.onClosed().removeData(*this, &WorkerPool::Data::onWorkerClosed, worker);
	worker.onResponse().removeData(*this, &WorkerPool::Data::onWorkerResponse, worker);

	onWorkerRemoved(worker);
	worker.close();
}
