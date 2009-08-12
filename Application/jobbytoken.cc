#include "application.ih"

bool Application::jobByToken(string const &token, Job &job) const
{
	if (token == "")
	{
		return false;
	}
	
	map<string, Job>::const_iterator iter;
	
	for (iter = d_jobs.begin(); iter != d_jobs.end(); ++iter)
	{
		if (iter->second.token() == token)
		{
			job = iter->second;
			return true;
		}
	}
	
	return false;
}
