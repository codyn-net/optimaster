#include "application.ih"

Application::Application(int &argc, char **&argv)
:
	d_command(*this),
	d_exitAtLast(false)
{
	enableEnvironment();

	d_discovery.onGreeting().add(*this, &Application::onGreeting);
	
	d_jobQueue.onJobAdded().add(*this, &Application::onIdleWork);
	d_workerPool.onWorkerIdle().add(*this, &Application::onIdleWork);
	d_workerPool.onWorkerRemoved().add(*this, &Application::onWorkerRemoved);
	
	parseArguments(argc, argv);
	
	d_discovery.listen();
	d_command.listen();
	
	// Initialize gcrypt
	gcry_check_version (0);
}
