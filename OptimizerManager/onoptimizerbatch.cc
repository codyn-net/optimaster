#include "optimizermanager.ih"

bool OptimizerManager::onOptimizerBatch(optimization::messages::task::Batch &batch, int fd)
{
	Optimizer optimizer;
	
	if (!fromDescriptor(fd, optimizer))
	{
		return false;
	}
	
	BatchArgs args = {optimizer, batch};
	onBatch(args);
	
	return false;
}
