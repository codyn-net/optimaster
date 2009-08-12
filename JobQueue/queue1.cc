#include "jobqueue.ih"

void JobQueue::queue(Job &job) 
{
	vector<Cloneable<optimization::Solution> > &solutions = job.optimizer().solutions();

	if (solutions.size() == 0)
		return;
	
	debug_master << "Queue job: " << job.name() << endl;

	vector<Cloneable<optimization::Solution> >::iterator iter;	
	Cloneable<Item> prev;
	
	updatePrioritySum(solutions.size() * job.priority());
	
	for (iter = solutions.begin(); iter != solutions.end(); ++iter)
	{
		Item item(job, **iter);
		
		item.setOvertake(d_data->prioritySum / job.priority());
		
		if (insert(item))
		{
			if (prev)
				prev->sequence(item);
			
			prev = item;
		}
	}
	
	debug_master << "Queuing " << solutions.size() << " solutions for " << job.name() << endl;
	addSolutionsPerJob(job, solutions.size());
}
