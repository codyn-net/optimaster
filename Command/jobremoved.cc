#include "command.ih"

void Command::jobRemoved(Job &job) 
{
	command::Signal signal;
	
	signal.set_type(command::Signal::JobRemoved);
	signal.mutable_jobremoved()->set_name(job.name());
	
	string serialized;
	
	if (Messages::create(signal, serialized))
	{
		d_data->signalClient.write(serialized);
	}
}
