#include "task.ih"

Task::Task(size_t id, double priority, task::Task const &task) 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->id = id;
	d_data->priority = priority;
	d_data->task = task;

	d_data->overtake = 0;
}
