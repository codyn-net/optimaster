#include "application.ih"

bool Application::removeJob(string const &name)
{
	Job job;
	
	if (this->job(name, job) && job.token() == "")
	{
		return removeJob(job);
	}
	else if (jobByToken(name, job))
	{
		return removeJob(job);
	}
	else
	{
		return false;
	}
}
