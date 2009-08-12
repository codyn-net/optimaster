#include "command.ih"

void Command::newIteration(Job &job) 
{
	command::Signal signal;
	
	signal.set_type(command::Signal::NewIteration);
	signal.mutable_newiteration()->set_name(job.name());
	command::Iteration *iteration = signal.mutable_newiteration()->mutable_iteration();
	
	db::SQLite &ddb = job.optimizer().db();
	
	db::Row row = ddb.query("SELECT `iteration`, `best_fitness`, `time` FROM `iteration` ORDER BY `iteration` DESC LIMIT 1");
	
	if (row && !row.done())
	{
		d_data->setIteration(row, *iteration);
		string serialized;
	
		if (Messages::create(signal, serialized))
		{
			d_data->signalClient.write(serialized);
		}
	}
}
