#include "jobqueue.ih"

bool JobQueue::jobDone(Job &job) const
{
	return d_data->solutionsPerJob.find(job.name()) == d_data->solutionsPerJob.end();
}
