#include "workerpool.ih"

void WorkerPool::Data::onWorkerFailed(bool timeout, Worker worker) 
{
	workerFinished(worker);
}
