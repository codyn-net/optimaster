#include "application.ih"

bool Application::onDispatch() 
{
	taskqueue::Task task;
	
	while (d_workerPool.hasIdleWorkers() && d_taskQueue.pop(task))
	{
		d_workerPool.activate(task);
	}
	
	return false;
}
