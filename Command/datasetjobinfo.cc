#include "command.ih"

void Command::Data::setJobInfo(Job &job, command::Job &message)
{
	optimization::Optimizer &optimizer = job.optimizer();
	
	message.set_name(job.name());
	message.set_priority(job.priority());
	message.set_optimizer(optimizer.name());
	message.set_maxiterations(optimizer.settings().property<size_t>("maxIterations"));
	
	db::SQLite &ddb = job.optimizer().db();
	
	db::Row row = ddb.query("SELECT `iteration`, `time` FROM `iteration` ORDER BY `iteration` DESC LIMIT 1");
	
	if (!row.done())
	{
		message.set_iteration(row.get<size_t>(0));
		message.set_time(row.get<size_t>(1));
	}
	else
	{
		message.set_iteration(0);
		message.set_time(0);
	}
}
