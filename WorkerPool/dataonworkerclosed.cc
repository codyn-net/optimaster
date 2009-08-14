#include "workerpool.ih"

void WorkerPool::Data::onWorkerClosed(int fd, Worker worker) 
{
	if (Debug::enabled(Debug::Domain::Worker))
	{
		debug_worker << "Worker disconnected: " << worker.address().host(true) << ":" << worker.address().port(true) << endl;
	}
	
	deque<Worker>::iterator iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);
	
	if (iter != activeWorkers.end())
	{
		/* Worker was active when it was disconnected... */
		worker.failed(optimization::messages::worker::Response::Failure::Disconnected);
	}
	
	removeWorker(worker);
}
