#include "command.ih"

void Command::Data::handleAddJob(Client &client, command::AddJobCommand const &cmd) 
{
	size_t priority;
	string token;
	
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

	Job job;
	
	try
	{
		application.addJobFromXml(cmd.description(), priority, job);
	}
	catch (Application::InvalidJob &e)
	{
		response(client, command::AddJob, false, ((string)"Invalid job description: ") + e.what());
		return;
	}
	
	job.setToken(token);
	response(client, command::AddJob, true, string("Added new job: ") + job.name());
}
