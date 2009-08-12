#include "application.ih"

vector<Job> Application::jobs() const
{
	map<string, Job>::const_iterator iter;
	vector<Job> ret;
	
	for (iter = d_jobs.begin(); iter != d_jobs.end(); ++iter)
	{
		ret.push_back(iter->second);
	}
	
	return ret;
}
