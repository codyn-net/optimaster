#include "job.ih"

Job::Job() 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->executionTime = 1;
	d_data->priority = 1;
	d_data->failures = 0;
}
