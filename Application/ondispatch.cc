#include "application.ih"

bool Application::onDispatch() 
{
	while (d_jobQueue.hasJobs() && d_workerPool.hasIdleWorkers())
	{
		Job job;
		optimization::Solution solution;
		
		d_jobQueue.pop(job, solution);
		
		Worker worker;
		d_workerPool.activateWorker(job, solution, worker);
	}
	
	return false;
}
