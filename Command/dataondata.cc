#include "command.ih"

void Command::Data::onData(FileDescriptor::DataArgs &args, Client client)
{
	vector<command::Command> cmds;
	vector<command::Command>::iterator iter;
		
	Messages::extract(args, cmds);

	for (iter = cmds.begin(); iter != cmds.end(); ++iter)
	{
		command::Command &c = *iter;
		
		switch (c.type())
		{
			case command::AddJob:
				handleAddJob(client, c.addjob());
			break;
			case command::RemoveJob:
				handleRemoveJob(client, c.rmjob());
			break;
			case command::ListJobs:
				handleListJobs(client, c.listjobs());
			break;
			case command::JobProgress:
				handleJobProgress(client, c.jobprogress());
			break;
			case command::JobInfo:
				handleJobInfo(client, c.jobinfo());
			break;
			default:
				/* Write back response... */
				response(client, c.type(), false, "Unknown command");
			break;
		}
	}
}
