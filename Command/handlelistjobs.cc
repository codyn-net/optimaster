#include "command.ih"

void Command::Data::handleListJobs(Client &client, command::ListJobsCommand const &command) 
{
	// Iterate over all the active jobs, you know what to do
	command::Response response = createResponse(command::ListJobs, true, "");
	
	command::ListJobsResponse &ljr = *response.mutable_listjobs();
	vector<Job> jobs = application.jobs();
	
	for (vector<Job>::iterator iter = jobs.begin(); iter != jobs.end(); ++iter)
	{
		command::Job *job = ljr.add_jobs();
		
		setJobInfo(*iter, *job);
	}
	
	string serialized;
	
	if (Messages::create(response, serialized))
	{
		client.write(serialized);
	}
}
