#include "command.ih"

void Command::Data::handleJobProgress(Client &client, command::JobProgressCommand const &command) 
{
	// Find the job
	Job job;
	
	if (!application.job(command.name(), job))
	{
		response(client, command::JobProgress, false, "Could not find job");
		return;
	}
	
	command::Response response = createResponse(command::JobProgress, true, "");
	command::JobProgressResponse *pgs = response.mutable_jobprogress();
	
	db::SQLite &ddb = job.optimizer().db();
	
	db::Row row = ddb.query("SELECT `iteration`, `best_values`, `best_names`, `best_fitness`, `time` FROM `iteration` ORDER BY `iteration`");
	
	if (!row.done())
	{
		while (row)
		{
			command::Iteration *iteration = pgs->add_iterations();

			setIteration(row, *iteration);
			row.next();
		}
	}
	
	string serialized;
	
	if (Messages::create(response, serialized))
	{
		client.write(serialized);
	}
}
