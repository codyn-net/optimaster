#include "application.ih"

void Application::onWorkerRemoved(Worker &worker)
{
	worker.onResponse().removeData(*this, &Application::onWorkerResponse, worker);
	worker.onChallenge().removeData(*this, &Application::onWorkerChallenge, worker);
	
	if (worker.working())
	{
		if (Debug::enabled(Debug::Domain::Worker))
		{
			debug_worker << "Worker (" << worker.address().host(true) << ":" << worker.address().port(true) << ") disconnected while working, reschedule (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
		}

		d_taskQueue.queue(worker.task());
	}
	else
	{
		if (Debug::enabled(Debug::Domain::Worker))
		{
			debug_worker << "Worker (" << worker.address().host(true) << ":" << worker.address().port(true) << ") disconnected" << endl;
		}
	}
}
