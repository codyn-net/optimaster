#include "optimizermanager.ih"

void OptimizerManager::onNewConnection(Client &client)
{
	client.onClosed().add(*this, &OptimizerManager::onConnectionClosed);
	
	Optimizer optimizer(client);
	
	optimizer.onBatch().addData(*this, &OptimizerManager::onOptimizerBatch, client.fd(), 0);
	d_optimizers[client.fd()] = optimizer;
	
	if (Debug::enabled(Debug::Domain::Master))
	{
		debug_master << "New optimizer connected: " << optimizer.id() << " (" << client.address().host(true) << ":" << client.address().port(true) << ")" << endl;
	}
}
