#include "optimizermanager.ih"

OptimizerManager::~OptimizerManager()
{
	d_server.onNewConnection().remove(*this, &OptimizerManager::onNewConnection);
	
	map<int, Optimizer>::iterator iter;
	
	for (iter = d_optimizers.begin(); iter != d_optimizers.end(); ++iter)
	{
		iter->second.client().onClosed().remove(*this, &OptimizerManager::onConnectionClosed);
	}
	
	d_optimizers.clear();
}
