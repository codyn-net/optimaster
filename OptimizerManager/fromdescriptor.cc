#include "optimizermanager.ih"

bool OptimizerManager::fromDescriptor(int fd, Optimizer &optimizer)
{
	map<int, Optimizer>::iterator found = d_optimizers.find(fd);
	
	if (found != d_optimizers.end())
	{
		optimizer = found->second;
		return true;
	}
	else
	{
		return false;
	}
}
