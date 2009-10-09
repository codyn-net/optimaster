#include "application.ih"

bool Application::onOptimizerClosed(Optimizer &optimizer)
{
	debug_master << "Optimizer disconnected: " << optimizer.id() << endl;
	
	// Remove all tasks from the task queue for this optimizer
	d_taskQueue.remove(optimizer.id());
}
