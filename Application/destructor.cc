#include "application.ih"

Application::~Application()
{
	d_optimizerManager.onBatch.remove(*this, &Application::onOptimizerBatch);
	d_optimizerManager.onOptimizerClosed.remove(*this, &Application::onOptimizerClosed);
	
	d_discovery.onGreeting().remove(*this, &Application::onGreeting);
	
	d_workerPool.onWorkerIdle().remove(*this, &Application::onIdleWork);
	d_workerPool.onWorkerRemoved().remove(*this, &Application::onWorkerRemoved);
}
