#include "jobqueue.ih"

void JobQueue::queue(Job &job, optimization::Solution &solution) 
{
	updatePrioritySum(job.priority());
	
	Item item(job, solution);
	item.setOvertake(d_data->prioritySum / job.priority());
	
	bool same = (d_data->queue.begin() == d_data->queue.end() || d_data->queue.back().job() != job);

	if (insert(item) && same)
	{
		list<Item>::iterator iter = d_data->queue.end();
		
		// item was inserted at the end, so we go 2 back to get the
		// previously last item and sequence it with the new item
		--iter;
		--iter;
		
		if (iter != d_data->queue.end())
		{
			iter->sequence(item);
		}
	}
	
	addSolutionsPerJob(job, 1);
}
