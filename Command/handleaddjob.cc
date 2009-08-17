
#include "command.ih"

void Command::Data::handleAddJob(Client &client, command::AddJobCommand const &cmd) 
{
	size_t priority;
	string token;
	string user;
	string chain;

	optimaster::Config &config = Config::instance();
	
	if (!cmd.has_token() && !config.acceptAnonymous)
	{
		response(client, command::AddJob, false, "Tokenless jobs are not allowed");
		return;
	}
	
	if (config.maxJobs != 0 && static_cast<int>(application.jobs().size()) >= config.maxJobs)
	{
		response(client, command::AddJob, false, "Too many jobs");
		return;
	}
	
	if (cmd.has_priority())
	{
		priority = cmd.priority();
	}
	else
	{
		priority = 3;
	}
	
	if (cmd.has_token())
	{
		token = cmd.token();
	}
	
	if (cmd.has_user())
	{
		user = cmd.user();
	}
	
	if (cmd.has_chain())
	{
		chain = cmd.chain();
	}
	
	Job job;
	
	try
	{
		application.addJobFromXml(cmd.description(), priority, job, user, chain);
	}
	catch (Application::InvalidJob &e)
	{
		response(client, command::AddJob, false, ((string)"Invalid job description: ") + e.what());
		return;
	}
	
	job.setToken(token);
	response(client, command::AddJob, true, string("Added new job: ") + job.name());
}
