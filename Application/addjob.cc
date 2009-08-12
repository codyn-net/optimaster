#include "application.ih"

void Application::addJob(Job &job) 
{
	d_jobs[job.name()] = job;
	
	/* Insert into job queue */
	d_jobQueue.queue(job);
	d_command.jobAdded(job);
}
