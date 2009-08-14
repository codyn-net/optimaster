#include "application.ih"

void Application::newIteration(Job &job) 
{
	if (job.optimizer().iteration())
	{
		debug_master << "New iteration for " << job.name() << endl;
		d_jobQueue.queue(job);

		d_command.newIteration(job);
	}
	else
	{
		debug_master << "Job finished: " << job.name() << endl;
		cout << "# Job finished: `" << job.name() << "'" << endl;
	
		removeJob(job);
	
		if (d_exitAtLast && d_jobs.begin() == d_jobs.end())
		{
			cout << "# Last job finished, exiting master..." << endl;
			d_mainLoop->quit();
		}
	}
}
