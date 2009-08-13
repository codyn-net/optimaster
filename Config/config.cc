#include "config.ih"

Config::Config()
{
	stringstream s;
	s << ":" << optimization::Constants::DiscoveryPort;

	acceptAnonymous = true;
	maxJobs = 0;
	discoveryAddress = s.str();
	commandPort = optimization::Constants::CommandPort;

	registerProperty("accept anonymous", acceptAnonymous);
	registerProperty("discovery namespace", discoveryNamespace);
	registerProperty("discovery address", discoveryAddress);
	registerProperty("command port", commandPort);

	registerProperty("maximum jobs", maxJobs);
}
