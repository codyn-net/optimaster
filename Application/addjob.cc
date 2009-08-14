#include "application.ih"

void Application::addJob(Job &job) 
{
	d_jobs[job.name()] = job;
	
	// Insert into job queue
	string chain = job.chain();
	
	if (chain != "" && job.user() != "")
	{
		chain = job.user() + ":" + chain;
	}
	
	job.initializeDatabase();
	
	if (chain == "" || d_jobs.find(chain) == d_jobs.end())
	{
		job.setChain("");
		d_jobQueue.queue(job);
	}
	
	d_command.jobAdded(job);
}
