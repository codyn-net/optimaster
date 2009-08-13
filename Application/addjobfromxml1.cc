#include "application.ih"

void Application::addJobFromXml(string const &description, size_t priority, Job &job, std::string const &user, std::string const &chain) 
{
	xmlpp::DomParser parser;
	
	parser.set_substitute_entities();
	parser.parse_memory(description);
	
	addJobFromXml(parser, priority, job, user, chain);
}
