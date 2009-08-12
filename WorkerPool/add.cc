#include "workerpool.ih"

Worker WorkerPool::add(string const &connection) 
{
	/* Check if already registered */
	if (d_data->allWorkers.find(connection) != d_data->allWorkers.end())
	{
		debug_worker << "Worker is already registered: " << connection << endl;
		return Worker();
	}
	
	debug_worker << "Adding worker: " << connection << endl;

	/* Connect to worker */
	Worker worker(Client::resolve<Worker>(AddressInfo::Parse(connection)));
	
	if (worker)
	{
		debug_worker << "Ready for work: " << connection << endl;
	
		d_data->allWorkers[connection] = worker;
		d_data->idleWorkers.push_back(worker);
		
		worker.onClosed().addData(*d_data, &WorkerPool::Data::onWorkerClosed, worker);
		worker.onFailed().addData(*d_data, &WorkerPool::Data::onWorkerFailed, worker);
		worker.onSuccess().addData(*d_data, &WorkerPool::Data::onWorkerSuccess, worker);
		
		d_data->onWorkerIdle();
	}
	else
	{
		debug_worker << "Could not connect to worker: " << connection << endl;
	}
	
	return worker;
}
