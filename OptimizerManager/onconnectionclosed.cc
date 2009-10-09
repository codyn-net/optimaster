#include "optimizermanager.ih"

void OptimizerManager::onConnectionClosed(int fd)
{
	Optimizer optimizer;
	
	if (fromDescriptor(fd, optimizer))
	{
		onOptimizerClosed(optimizer);	
		d_optimizers.erase(fd);
	}
}
