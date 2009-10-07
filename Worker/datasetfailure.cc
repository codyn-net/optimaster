#include "worker.ih"

void Worker::Data::setFailure(task::Response::Failure::Type type, string const &message)
{
	task::Response::Failure *failure = response.mutable_failure();
	
	failure->set_type(type);
	failure->set_message(message);

	response.set_status(task::Response::Failed);
}
