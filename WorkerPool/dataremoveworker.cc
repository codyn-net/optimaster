#include "workerpool.ih"

void WorkerPool::Data::removeWorker(Worker &worker) 
{
	deque<Worker>::iterator iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);
	activeWorkers.erase(iter, activeWorkers.end());
	
	iter = std::remove(idleWorkers.begin(), idleWorkers.end(), worker);
	idleWorkers.erase(iter, idleWorkers.end());

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
	worker.onFailed().removeData(*this, &WorkerPool::Data::onWorkerFailed, worker);
	worker.onSuccess().removeData(*this, &WorkerPool::Data::onWorkerSuccess, worker);
		
	onWorkerRemoved(worker);
	worker.close();
}
