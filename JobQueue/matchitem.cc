#include "jobqueue.ih"

bool JobQueue::matchItem(Item const &item, Job const &job)
{
	return (item.job().name() == job.name());
}
