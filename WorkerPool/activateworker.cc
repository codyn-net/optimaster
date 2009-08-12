#include "workerpool.ih"

bool WorkerPool::activateWorker(Job &job, optimization::Solution &solution, Worker &worker)
{
	if (!hasIdleWorkers())
		return false;
	
	worker = d_data->idleWorkers.front();

	d_data->idleWorkers.pop_front();
	d_data->activeWorkers.push_back(worker);
	
	if (d_data->activeWorkersPerJob.find(job.name()) == d_data->activeWorkersPerJob.end())
	{
		d_data->activeWorkersPerJob[job.name()] = 1;
	}
	else
	{
		++(d_data->activeWorkersPerJob[job.name()]);
	}
	
	debug_worker << "Activating worker [" << worker.address().host() << ":" << worker.address().port() << "] for " << job.name() << " (" << solution.id() << ")" << endl;
	worker.sendJob(job, solution);

	return true;
}
