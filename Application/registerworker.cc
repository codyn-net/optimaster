#include "application.ih"

void Application::registerWorker(std::string const &connection) 
{
	Worker worker = d_workerPool.add(connection);
	
	if (worker)
	{
		worker.onResponse().addData(*this, &Application::onWorkerResponse, worker);
		worker.onChallenge().addData(*this, &Application::onWorkerChallenge, worker);
	}
}
