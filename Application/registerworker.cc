#include "application.ih"

void Application::registerWorker(std::string const &connection) 
{
	Worker worker = d_workerPool.add(connection);
	
	if (worker)
	{
		worker.onFailed().addData(*this, &Application::onWorkerFailed, worker);
		
		worker.onSuccess().addAfter(*this, &Application::onWorkerSuccess);
		worker.onChallenge().addData(*this, &Application::onWorkerChallenge, worker);
	}
}
