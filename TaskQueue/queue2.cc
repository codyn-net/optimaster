#include "taskqueue.ih"

void TaskQueue::queue(taskqueue::Task &task)
{
	updatePrioritySum(task.priority());
	
	task.setOvertake(d_data->prioritySum / task.priority());
	task.sequence();
	
	bool same = (d_data->queue.begin() == d_data->queue.end() || d_data->queue.back().id() != task.id());
	
	if (insert(task) && same)
	{
		list<taskqueue::Task>::iterator iter = d_data->queue.end();
		
		// task was inserted at the end, so we go 2 back to get the
		// previously last item and squence it with the new item
		--iter;
		--iter;
		
		if (iter != d_data->queue.end())
		{
			iter->sequence(task);
		}
	}
}
