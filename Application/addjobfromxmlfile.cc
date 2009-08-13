#include "application.ih"

void Application::addJobFromXmlFile(string const &filename, size_t priority, Job &job, std::string const &user) 
{
	xmlpp::DomParser parser;
	
	parser.set_substitute_entities();
	
	try
	{
		parser.parse_file(filename);
	}
	catch (xmlpp::internal_error &e)
	{
		throw InvalidJob("Could not read file `" + filename + "'");
	}
	
	addJobFromXml(parser, priority, job, user);
}
