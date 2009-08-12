#include "item.ih"

JobQueue::Item *JobQueue::Item::clone() const
{
	return new JobQueue::Item(*this);
}
