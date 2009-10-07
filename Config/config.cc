#include "config.ih"

Config::Config()
{
	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	acceptAnonymous = true;
	discoveryAddress = s.str();
	
	string user;
	if (Environment::variable("USER", user))
	{
		discoveryNamespace = user;
	}

	registerProperty("accept anonymous", acceptAnonymous);
	registerProperty("discovery namespace", discoveryNamespace);
	registerProperty("discovery address", discoveryAddress);
}
