#include "taskqueue.ih"

void TaskQueue::queue(size_t id, task::Batch &tasks) 
{
	if (tasks.tasks_size() == 0)
	{
		return;
	}
	
	Cloneable<taskqueue::Task> prev;
	updatePrioritySum(tasks.tasks_size() * tasks.priority());
	
	for (size_t idx = 0; idx < tasks.tasks_size(); ++idx)
	{
		taskqueue::Task task(id, tasks.priority(), tasks.tasks(idx));
		task.setOvertake(d_data->prioritySum / tasks.priority());
		
		if (insert(task))
		{
			if (prev)
			{
				prev->sequence(task);
			}
			
			prev = task;
		}
	}
}
