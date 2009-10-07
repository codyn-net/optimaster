#include "application.ih"

Application::Application(int &argc, char **&argv)
{
	enableEnvironment();

	d_discovery.onGreeting().add(*this, &Application::onGreeting);
	
	d_workerPool.onWorkerIdle().add(*this, &Application::onIdleWork);
	d_workerPool.onWorkerRemoved().add(*this, &Application::onWorkerRemoved);
	
	parseArguments(argc, argv);
	
	d_discovery.listen();
	
	// Initialize gcrypt
	gcry_check_version (0);
}
