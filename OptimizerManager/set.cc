#include "optimizermanager.ih"

void OptimizerManager::set(string const &host, string const &port)
{
	d_server.set(host, port);
}
