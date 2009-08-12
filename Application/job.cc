#include "application.ih"

bool Application::job(string const &name, Job &job) const
{
	map<string, Job>::const_iterator iter = d_jobs.find(name);
	
	if (iter == d_jobs.end())
	{
		return false;
	}
	
	job = iter->second;
	return true;
}
