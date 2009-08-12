#include "jobqueue.ih"

JobQueue::JobQueue() 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->prioritySum = 1;
}
