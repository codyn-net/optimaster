#include "optimizermanager.ih"

OptimizerManager::OptimizerManager()
:
	d_server(Constants::MasterPort)
{
	d_server.onNewConnection().add(*this, &OptimizerManager::onNewConnection);
}
