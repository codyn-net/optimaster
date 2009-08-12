#include "command.ih"

void Command::Data::handleRemoveJob(Client &client, command::RemoveJobCommand const &command) 
{
	if (!application.removeJob(command.name()))
	{
		response(client, command::RemoveJob, false, "Job not found");
	}
	else
	{
		response(client, command::RemoveJob, true, "Job successfully removed");
	}
}
