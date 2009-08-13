#include "application.ih"

void Application::parseArguments(int &argc, char **&argv) 
{
	// Parse config file first
	optimaster::Config &config = optimaster::Config::initialize(CONFDIR "/optimaster.conf");

	Glib::OptionGroup group("Master", "Optimization Master");
	Glib::OptionEntry discovery;
	
	discovery.set_long_name("discovery");
	discovery.set_short_name('d');
	discovery.set_description("Discovery port");

	stringstream s;
	s << ":" << optimization::Constants::DiscoveryPort;

	Glib::ustring disc = s.str();
	group.add_entry(discovery, disc);
	
	Glib::OptionEntry command;
	command.set_long_name("command");
	command.set_short_name('c');
	command.set_description("Command port");
	
	group.add_entry(command, config.commandPort);
	
	
	Glib::OptionEntry jobs;
	jobs.set_long_name("jobs");
	jobs.set_short_name('j');
	jobs.set_description("Comma separated list of jobs to start");
	
	Glib::ustring thejobs;
	group.add_entry(jobs, thejobs);


	Glib::OptionEntry exitt;
	exitt.set_long_name("exit");
	exitt.set_short_name('e');
	exitt.set_description("Exit after last job has ended");
	
	group.add_entry(exitt, d_exitAtLast);
	
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
	d_discovery.set(parts[0], parts[1]);
	d_discovery.setNs(config.discoveryNamespace);

	d_command.set(config.commandPort);
	
	parts = String(thejobs).split(",");
	
	for (vector<string>::iterator iter = parts.begin(); iter != parts.end(); ++iter)
	{
		if (*iter == "")
		{
			continue;
		}

		Job job;
		
		try
		{
			addJobFromXmlFile(*iter, 0, job);

			cout << "# Scheduled job `" << job.name() << "'..." << endl;
		}
		catch (InvalidJob &e)
		{
			cout << "# Error while adding job `" << *iter << "': " << e.what() << endl;
		}
	}
}
