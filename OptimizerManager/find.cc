#include "optimizermanager.ih"

bool OptimizerManager::find(size_t id, Optimizer &optimizer)
{
	map<int, Optimizer>::iterator found;
	
	found = d_optimizers.find(static_cast<int>(id));
	
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
