#include "taskqueue.ih"

bool TaskQueue::empty() const
{
	return d_data->queue.empty();
}
