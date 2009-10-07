#include "workerpool.ih"

bool WorkerPool::activate(taskqueue::Task &task)
{
	if (!hasIdleWorkers())
	{
		return false;
	}
	
	Worker worker = d_data->idleWorkers.front();

	d_data->idleWorkers.pop_front();
	d_data->activeWorkers.push_back(worker);
	
	worker.send(task);
	return true;
}
