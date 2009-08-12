#include "item.ih"

JobQueue::Item::Item(Job &job, optimization::Solution &solution) 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->job = job;
	d_data->solution = solution;
	d_data->overtake = 0;
}
