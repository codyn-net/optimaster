#include "workerpool.ih"

size_t WorkerPool::active(Job &job) const
{
	map<string, size_t>::iterator iter = d_data->activeWorkersPerJob.find(job.name());
	
	if (iter == d_data->activeWorkersPerJob.end())
		return 0;
	else
		return iter->second;		
}
