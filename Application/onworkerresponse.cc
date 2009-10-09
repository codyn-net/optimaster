#include "application.ih"

void Application::onWorkerResponse(task::Response &response, Worker worker)
{
	taskqueue::Task &task = worker.task();
	Optimizer optimizer;
	
	if (!d_optimizerManager.find(task.id(), optimizer))
	{
		return;
	}
	
	if (response.status() == task::Response::Success)
	{
		// Simply relay to optimizer
		if (Debug::enabled(Debug::Domain::Worker))
		{
			debug_worker << "Worker success (" << worker.address().host(true) << ":" << worker.address().port(true) << ") for (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
		}

		optimizer.response(response);
	}
	else
	{
		// Check how many times this task failed
		task.failed();
		
		if (task.failures() >= Config::instance().maxTaskFailures)
		{
			if (Debug::enabled(Debug::Domain::Worker))
			{
				debug_worker << "Task failed to many times (" << worker.address().host(true) << ":" << worker.address().port(true) << ") for (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
			}

			// Relay failure to optimizer
			optimizer.response(response);
		}
		else
		{
			if (Debug::enabled(Debug::Domain::Worker))
			{
				debug_worker << "Task failed, rescheduling (" << worker.address().host(true) << ":" << worker.address().port(true) << ") for (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
			}
		
			// Reschedule task
			d_taskQueue.queue(task);
		}
	}
}
