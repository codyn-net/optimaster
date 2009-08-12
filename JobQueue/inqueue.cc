#include "jobqueue.ih"

size_t JobQueue::inQueue(Job &job) const
{
	map<string, size_t>::iterator iter = d_data->solutionsPerJob.find(job.name());
	
	if (iter == d_data->solutionsPerJob.end())
		return 0;
	else
		return iter->second;	
}
