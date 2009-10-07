#include "workerpool.ih"

void WorkerPool::Data::onWorkerClosed(int fd, Worker worker) 
{
	if (Debug::enabled(Debug::Domain::Worker))
	{
		debug_worker << "Worker disconnected: " << worker.address().host(true) << ":" << worker.address().port(true) << endl;
	}
	
	removeWorker(worker);
}
