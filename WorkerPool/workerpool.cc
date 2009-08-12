#include "workerpool.ih"

WorkerPool::WorkerPool() 
{
	d_data = new Data();
	addPrivateData(d_data);
}
