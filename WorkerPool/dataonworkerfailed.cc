#include "workerpool.ih"

void WorkerPool::Data::onWorkerFailed(optimization::messages::worker::Response::Failure &failure, Worker worker) 
{
	workerFinished(worker);
}
