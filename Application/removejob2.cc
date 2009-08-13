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
	
	// Check if there were jobs that depended on this guy
	for (iter = d_jobs.begin(); iter != d_jobs.end(); ++iter)
	{
		Job &j = iter->second;
		
		if (j.chain() == "")
		{
			continue;
		}
		
		string chain = j.chain();
	
		if (j.user() != "")
		{
			chain = j.user() + ":" + chain;
		}
	
		if (d_jobs.find(chain) == d_jobs.end())
		{
			d_jobQueue.queue(j);
		}
	}

	return true;
}
