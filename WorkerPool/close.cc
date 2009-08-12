#include "workerpool.ih"

void WorkerPool::close()
{
	deque<Worker>::iterator iter;
	deque<Worker> cpc = d_data->activeWorkers;
	
	for (iter = cpc.begin(); iter != cpc.end(); ++iter)
	{
		iter->close();
	}
	
	cpc.clear();
	
	map<string, Worker> cp = d_data->allWorkers;
	map<string, Worker>::iterator it;
	
	for (it = cp.begin(); it != cp.end(); ++it)
	{
		d_data->removeWorker(it->second);
	}
	
	d_data->activeWorkers.clear();
	d_data->idleWorkers.clear();
	d_data->activeWorkersPerJob.clear();
	d_data->allWorkers.clear();
}
