#include "application.ih"

void Application::parseArguments(int &argc, char **&argv) 
{
	// Parse config file first
	optimaster::Config &config = optimaster::Config::initialize(CONFDIR "/optimaster.conf");
	
	Glib::OptionGroup group("Master", "Optimization Master");
	Glib::OptionEntry discovery;
	
	discovery.set_long_name("discovery");
	discovery.set_short_name('d');
	discovery.set_description("Discovery address/namespace");

	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	Glib::ustring disc = s.str();
	group.add_entry(discovery, disc);
	
	Glib::OptionContext context;
	
	context.set_main_group(group);
	context.parse(argc, argv);
	
	vector<string> parts = String(disc).split(":", 2);
	string host = parts[0];
	string port = parts.size() == 2 ? parts[1] : "";

	if (AddressInfo::isIPAddress(host))
	{
		if (port != "")
		{
			config.discoveryAddress = host + ":" + port;
		}
		else
		{
			s.str("");
			s << host << ":" << optimization::Constants::DiscoveryPort;

			config.discoveryAddress = s.str();
		}
	}
	else
	{
		if (port != "")
		{
			config.discoveryAddress = optimization::Constants::DiscoveryGroup + ":" + port;
		}
		
		config.discoveryNamespace = host;
	}
	
	parts = String(config.discoveryAddress).split(":", 2);
	
	if (parts[0] == "")
	{
		parts[0] = optimization::Constants::DiscoveryGroup;
	}
	
	d_discovery.set(parts[0], parts[1]);
	d_discovery.setNs(config.discoveryNamespace);
}
