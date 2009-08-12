#include "application.ih"

void Application::onWorkerRemoved(Worker &worker)
{
	worker.onFailed().removeData(*this, &Application::onWorkerFailed, worker);
	worker.onSuccess().remove(*this, &Application::onWorkerSuccess);
	worker.onChallenge().removeData(*this, &Application::onWorkerChallenge, worker);
}
