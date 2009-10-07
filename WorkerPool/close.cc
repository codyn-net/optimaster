#include "workerpool.ih"

void WorkerPool::close()
{
	deque<Worker>::iterator iter;
	deque<Worker> cpc = d_data->activeWorkers;
	
	// Close connections to all the active workers first
	for (iter = cpc.begin(); iter != cpc.end(); ++iter)
	{
		iter->close();
	}
	
	cpc.clear();
	
	// REmove all the workers that are still left
	map<string, Worker> cp = d_data->allWorkers;
	map<string, Worker>::iterator it;
	
	for (it = cp.begin(); it != cp.end(); ++it)
	{
		d_data->removeWorker(it->second);
	}
	
	// Clear them all	
	d_data->activeWorkers.clear();
	d_data->idleWorkers.clear();
	d_data->allWorkers.clear();
}
