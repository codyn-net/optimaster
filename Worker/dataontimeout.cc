#include "worker.ih"

bool Worker::Data::onTimeout() 
{
	worker::Response::Failure failure;
	
	failure.set_type(worker::Response::Failure::Timeout);

	this->args.job.failed();
	onFailed(failure);

	return false;
}
