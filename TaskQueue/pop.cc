#include "taskqueue.ih"

bool TaskQueue::pop(taskqueue::Task &task)
{
	if (d_data->queue.empty())
	{
		return false;
	}
	
	task = d_data->queue.front();
	d_data->queue.pop_front();
	
	// TODO: update priority sum?
	return true;
}
