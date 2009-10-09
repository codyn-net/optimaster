#include "application.ih"

bool Application::onOptimizerBatch(OptimizerManager::BatchArgs &args)
{
	// Queue the batch of tasks
	debug_master << "Received optimizer batch (" << args.optimizer.id() << "): " << args.batch.tasks_size() << endl;
	
	d_taskQueue.queue(args.optimizer.id(), args.batch);
	onIdleWork();
}
