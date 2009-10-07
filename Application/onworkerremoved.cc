#include "application.ih"

void Application::onWorkerRemoved(Worker &worker)
{
	worker.onResponse().removeData(*this, &Application::onWorkerResponse, worker);
	worker.onChallenge().removeData(*this, &Application::onWorkerChallenge, worker);
	
	if (worker.working())
	{
		// TODO: Make sure to reschedule the task
	}
}
