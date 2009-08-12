#include "command.ih"

void Command::jobAdded(Job &job) 
{
	command::Signal signal;
	
	signal.set_type(command::Signal::JobAdded);
	signal.mutable_jobadded()->set_name(job.name());
	
	string serialized;
	
	if (Messages::create(signal, serialized))
	{
		d_data->signalClient.write(serialized);
	}
}
