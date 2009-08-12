#include "jobqueue.ih"

void JobQueue::remove(Job const &job)
{
	// Remove all from the queue
	map<string, size_t>::iterator iter = d_data->solutionsPerJob.find(job.name());
	
	if (iter != d_data->solutionsPerJob.end())
	{
		d_data->solutionsPerJob.erase(iter);
	}
	
	list<Item>::iterator remove = remove_if(d_data->queue.begin(), d_data->queue.end(), functional::memcb(*this, &JobQueue::matchItem, job));
	
	d_data->queue.erase(remove, d_data->queue.end());
}
