#include "worker.ih"

void Worker::failed(worker::Response::Failure::Type type, string const &message)
{
	worker::Response::Failure failure;
	
	failure.set_type(type);
	failure.set_message(message);
	
	d_data->onFailed(failure);
}
