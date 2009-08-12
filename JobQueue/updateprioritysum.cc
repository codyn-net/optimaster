#include "jobqueue.ih"

void JobQueue::updatePrioritySum(double num) 
{
	list<Item>::iterator iter;
	double prioritySum = d_data->prioritySum + num;
	double ratio = prioritySum / d_data->prioritySum;
	d_data->prioritySum = prioritySum;
	
	for (iter = d_data->queue.begin(); iter != d_data->queue.end(); ++iter)
	{
		iter->setOvertake(iter->overtake() * ratio);
	}
}
