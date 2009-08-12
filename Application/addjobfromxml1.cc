#include "application.ih"

void Application::addJobFromXml(string const &description, size_t priority, Job &job) 
{
	xmlpp::DomParser parser;
	
	parser.set_substitute_entities();
	parser.parse_memory(description);
	
	addJobFromXml(parser, priority, job);
}
