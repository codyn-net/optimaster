#include "taskqueue.ih"

TaskQueue::TaskQueue() 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->prioritySum = 1;
}
