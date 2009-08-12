#include "worker.ih"

bool Worker::Data::onTimeout() 
{
	bool timeout = true;
	
	++failures;
	onFailed(timeout);
	return false;
}
