#include "command.ih"

void Command::Data::handleJobInfo(network::Client &client, optimization::messages::command::JobInfoCommand const &command) 
{
	// Find the job
	Job job;
	
	if (!application.job(command.name(), job))
	{
		response(client, command::JobInfo, false, "Could not find job");
		return;
	}
	
	command::Response response = createResponse(command::JobInfo, true, "");
	command::JobInfoResponse &info = *response.mutable_jobinfo();
	
	setJobInfo(job, *info.mutable_job());
	
	string serialized;
	
	if (Messages::create(response, serialized))
	{
		client.write(serialized);
	}
}
