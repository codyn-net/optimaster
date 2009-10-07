#include "taskqueue.ih"

bool TaskQueue::insert(taskqueue::Task &task) 
{
	// Return true if task is inserted at the end of the queue
	list<taskqueue::Task>::reverse_iterator iter;
	
	for (iter = d_data->queue.rbegin(); iter != d_data->queue.rend(); ++iter)
	{
		if (!task.canOvertake(*iter))
			break;
	}
	
	if (iter == d_data->queue.rbegin())
	{
		d_data->queue.push_back(task);
		return true;
	}
	else
	{
		--iter;

		task.overtake(*iter);
		d_data->queue.insert(iter.base(), task);
		return false;
	}
}
