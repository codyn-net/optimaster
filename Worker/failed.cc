#include "worker.ih"

void Worker::failed(bool timeout) 
{
	++d_data->failures;
	d_data->onFailed(timeout);
}
