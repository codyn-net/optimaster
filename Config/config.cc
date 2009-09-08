#include "config.ih"

Config::Config()
{
	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	acceptAnonymous = true;
	maxJobs = 0;
	discoveryAddress = s.str();
	
	s.str("");
	s << "127.0.0.1:" << optimization::Constants::CommandPort;
	commandAddress = s.str();
	
	string path;
	
	if (!Environment::variable("OPTIMIZATION_DATA_PATH", path))
	{
		path = "/var/optimization";
	}
	
	dataDirectory = path;

	string user;
	if (Environment::variable("USER", user))
	{
		discoveryNamespace = user;
	}

	registerProperty("accept anonymous", acceptAnonymous);
	registerProperty("discovery namespace", discoveryNamespace);
	registerProperty("discovery address", discoveryAddress);
	registerProperty("command address", commandAddress);
	registerProperty("data directory", dataDirectory);

	registerProperty("maximum jobs", maxJobs);
}
