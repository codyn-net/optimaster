#include "taskqueue.ih"

void TaskQueue::remove(size_t id)
{
	std::list<taskqueue::Task>::iterator iter;
	
	iter = std::remove(d_data->queue.begin(), d_data->queue.end(), id);
	
	if (iter != d_data->queue.end())
	{
		d_data->queue.erase(iter);
	}
	
	// TODO: update priority sum
}
