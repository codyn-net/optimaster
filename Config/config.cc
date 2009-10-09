#include "config.ih"

Config::Config()
{
	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	acceptAnonymous = true;
	maxTaskFailures = 5;
	discoveryAddress = s.str();
	
	string user;
	if (Environment::variable("USER", user))
	{
		discoveryNamespace = user;
	}
	
	s.str("");
	s << ":" << optimization::Constants::MasterPort;
	listenAddress = s.str();

	registerProperty("accept anonymous", acceptAnonymous);
	registerProperty("discovery namespace", discoveryNamespace);
	registerProperty("discovery address", discoveryAddress);
	registerProperty("listen address", listenAddress);
	registerProperty("max task failures", maxTaskFailures);
}
