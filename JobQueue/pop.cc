#include "jobqueue.ih"

bool JobQueue::pop(Job &job, optimization::Solution &solution) 
{
	if (!hasJobs())
		return false;
	
	Item item = d_data->queue.front();
	d_data->queue.pop_front();
	
	job = item.job();
	solution = item.solution();
	
	--d_data->solutionsPerJob[job.name()];	
	return true;
}
