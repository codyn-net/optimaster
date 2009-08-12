#include "application.ih"

bool Application::removeJob(Job &job)
{
	map<string, Job>::iterator iter;
	
	iter = d_jobs.find(job.name());
	
	if (iter == d_jobs.end())
	{
		return false;
	}
	
	// Remove all open tasks in the queue
	d_jobQueue.remove(job);
	d_jobs.erase(iter);
	
	d_command.jobRemoved(job);

	return true;
}
