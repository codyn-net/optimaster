#include "application.ih"

void Application::sendWakeup()
{
	// Create new multicast client
	Client client = Client::Multicast(optimization::Constants::DiscoveryGroup, optimization::Constants::DiscoveryPort, 2);

	// Make wakeup discovery message with certain namespace
	optimization::messages::discovery::Discovery disc;
	
	disc.set_type(optimization::messages::discovery::Discovery::TypeWakeup);
	disc.set_namespace_(d_discovery.ns());

	optimization::messages::discovery::Wakeup *wakeup = disc.mutable_wakeup();

	// Set the connection
	wakeup->set_connection(d_discovery.connection());
	string serialized;
	
	if (optimization::Messages::create(disc, serialized))
	{
		client.write(serialized);
	}
}
