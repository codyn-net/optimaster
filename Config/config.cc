#include "config.ih"

Config::Config()
{
	stringstream s;
	s << ":" << optimization::Constants::DiscoveryPort;

	acceptAnonymous = true;
	maxJobs = 0;
	discoveryAddress = s.str();
	commandPort = optimization::Constants::CommandPort;
	
	string path;
	
	if (!Environment::variable("OPTIMIZATION_DATA_PATH", path))
	{
		path = "/var/optimization";
	}
	
	dataDirectory = path;

	registerProperty("accept anonymous", acceptAnonymous);
	registerProperty("discovery namespace", discoveryNamespace);
	registerProperty("discovery address", discoveryAddress);
	registerProperty("command port", commandPort);
	registerProperty("data directory", dataDirectory);

	registerProperty("maximum jobs", maxJobs);
}
