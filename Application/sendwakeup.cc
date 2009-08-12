#include "application.ih"

void Application::sendWakeup()
{
	Client client = Client::Multicast(optimization::Constants::DiscoveryGroup, optimization::Constants::DiscoveryPort, 2);

	optimization::messages::discovery::Discovery disc;
	
	disc.set_type(optimization::messages::discovery::Discovery::WAKEUP);
	disc.set_namespace_(d_discovery.ns());

	optimization::messages::discovery::Wakeup *wakeup = disc.mutable_wakeup();

	wakeup->set_connection(d_discovery.connection());
	string serialized;
	
	if (optimization::Messages::create(disc, serialized))
	{
		client.write(serialized);
	}
}
