#include "application.ih"

void Application::addJobFromXml(xmlpp::DomParser &parser, size_t priority, Job &jobret, std::string const &user, std::string const &chain)
{	
	if (!parser)
	{
		throw InvalidJob("Could not parse xml");
	}
	
	xmlpp::Document *doc = parser.get_document();
	xmlXIncludeProcess(doc->cobj());
	
	xmlpp::Node *root = doc->get_root_node();
	
	xmlpp::NodeSet jobs = root->find("/job");
	
	if (jobs.begin() == jobs.end())
	{
		throw InvalidJob("Could not find root /job node");
	}
	
	xmlpp::Element *job = dynamic_cast<xmlpp::Element *>(*jobs.begin());
	
	/* Get required nodes */
	xmlpp::Attribute *attr;
	
	attr = job->get_attribute("name");
	
	if (!attr)
	{
		throw InvalidJob("Job does not have a name");
	}
	
	string origJobName = attr->get_value();
	
	if (origJobName == "")
	{
		throw InvalidJob("Job name cannot be empty");
	}
	
	if (user != "")
	{
		origJobName = user + ":" + origJobName;
	}
	
	size_t i = 0;
	string jobName = origJobName;
	
	while (d_jobs.find(jobName) != d_jobs.end())
	{
		stringstream s;
		
		s << origJobName << "_" << (++i);
		jobName = s.str();
	}

	xmlpp::Element *child;

	/* Get optimizer */
	if (!getFirstChild(job, "optimizer", child))
	{
		throw InvalidJob("Optimizer not specified");
	}
	
	attr = child->get_attribute("name");
	
	if (!attr)
	{
		throw InvalidJob("Optimizer name not specified");
	}
	
	/* Lookup optimizer from optimizer manager */
	Cloneable<optimization::Optimizer> optimizer;
	optimizer = d_optimizerManager.lookup(attr->get_value());
	
	if (!optimizer)
	{
		throw InvalidJob("Optimizer not found");
	}
	
	debug_master << "Found optimizer `" << optimizer->name() << "' for job `" << jobName <<  "'" << endl;
		
	/* Parse settings for optimizer */
	settingsFromXml(optimizer->settings(), child);
	
	/* Parse extensions for optimizer */
	extensionsFromXml(d_optimizerExtensionManager, optimizer, child);
	
	optimization::Fitness fitness;
	
	/* Parse fitness function */
	if (getFirstChild(job, "fitness", child))
	{
		settingsFromXml(fitness.settings(), child);
		fitness.reset();
	}
	
	/* Parse dispatcher */
	if (!getFirstChild(job, "dispatcher", child))
	{
		throw InvalidJob("Dispatcher not specified");
	}
	
	attr = child->get_attribute("name");
	
	if (!attr || attr->get_value() == "")
	{
		throw InvalidJob("Dispatcher name not specified");
	}
	
	Dispatcher dispatcher(attr->get_value());
	settingsFromXml(dispatcher, child);

	optimization::Boundaries boundaries;
	
	// Parse boundaries
	if (getFirstChild(job, "boundaries", child))
	{
		parseBoundaries(boundaries, child);
	}

	optimization::Parameters parameters;
	
	// Parse parameters
	if (getFirstChild(job, "parameters", child))
	{
		parseParameters(parameters, boundaries, child);
	}
	
	string datafile;
	
	if (user != "")
	{
		datafile = user + "/" + jobName.substr(user.size() + 1);
	}
	else
	{
		datafile = jobName;
	}

	string filename = FileSystem::uniqueName(dataDirectory() + "/" + datafile + ".db");
	FileSystem::mkdirs(FileSystem::dirname(filename));

	Job j(jobName, optimizer, fitness, parameters, boundaries, dispatcher);

	j.setPriority(priority);
	j.setUser(user);
	j.setChain(chain);

	optimizer->setDataFilename(filename);
	
	addJob(j);
	jobret = j;
}
