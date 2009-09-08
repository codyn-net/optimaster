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
	
	Glib::OptionEntry command;
	command.set_long_name("command");
	command.set_short_name('c');
	command.set_description("Command listen address");
	
	group.add_entry(command, config.commandAddress);
	
	Glib::OptionEntry jobs;
	jobs.set_long_name("jobs");
	jobs.set_short_name('j');
	jobs.set_description("Comma separated list of jobs to start");
	
	Glib::ustring thejobs;
	group.add_entry(jobs, thejobs);


	Glib::OptionEntry datadir;
	datadir.set_long_name("data");
	datadir.set_short_name('t');
	datadir.set_description("Data files output directory");
	
	group.add_entry(datadir, config.dataDirectory);

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
	
	if (parts[0] == "")
	{
		parts[0] = optimization::Constants::DiscoveryGroup;
	}
	
	d_discovery.set(parts[0], parts[1]);
	d_discovery.setNs(config.discoveryNamespace);

	parts = String(config.commandAddress).split(":", 2);
	
	if (parts.size() == 1 || parts[1] == "")
	{
		if (parts.size() == 1)
		{
			parts.push_back("");
		}

		stringstream s;
		s << optimization::Constants::CommandPort;
		
		parts[1] = s.str();
	}

	d_command.set(parts[0], parts[1]);
	
	// Check for data output directory existence
	if (!FileSystem::directoryExists(config.dataDirectory))
	{
		if (!FileSystem::mkdirs(config.dataDirectory))
		{
			cerr << "# Data directory `" << config.dataDirectory << "` does not exist!" << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	// Check if data dir is writable
	if (access(config.dataDirectory.c_str(), R_OK | W_OK | X_OK) != 0)
	{
		cerr << "# Incorrect permissions for data directory `" << config.dataDirectory << "`!" << endl;
		exit(EXIT_FAILURE);
	}
	
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
