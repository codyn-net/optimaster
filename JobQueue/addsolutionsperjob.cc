#include "jobqueue.ih"

void JobQueue::addSolutionsPerJob(Job &job, size_t num) 
{
	string const &name = job.name();

	if (d_data->solutionsPerJob.find(name) == d_data->solutionsPerJob.end())
		d_data->solutionsPerJob[name] = num;
	else
		d_data->solutionsPerJob[name] += num;
	
	d_data->onJobAdded();
}
