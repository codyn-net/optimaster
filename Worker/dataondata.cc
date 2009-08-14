#include "worker.ih"

bool Worker::Data::onData(os::FileDescriptor::DataArgs &args) 
{
	vector<worker::Response> response;
	vector<worker::Response>::iterator iter;
	
	Messages::extract(args, response);
	
	if (response.begin() == response.end())
	{
		return false;
	}
	
	worker::Response &r = response[0];
	
	if  (r.status() == worker::Response::Challenge)
	{
		// Send a token response
		string challenge = r.challenge();
		onChallenge(challenge);

		return false;
	}
	
	if (timeout)
	{
		timeout.disconnect();
	}
	
	if (r.status() != worker::Response::Success)
	{
		debug_worker << "Got response, worker failed (" << r.id() << "), " << r.status() << "..." << endl;
		this->args.job.failed();
		
		if (!r.has_failure())
		{
			r.mutable_failure()->set_type(worker::Response::Failure::Unknown);
		}
		
		onFailed(*r.mutable_failure());
	}
	else if (this->args.solution.id() != static_cast<size_t>(r.id()))
	{
		this->args.job.optimizer().log(optimization::Optimizer::LogType::Error) << "Worker responded with wrong id. Expected " << this->args.solution.id() << " but got " << static_cast<size_t>(r.id()) << optimization::Optimizer::Logger::End();
		debug_worker << "Got response, but for wrong request..." << endl;
		
		worker::Response::Failure failure;
		failure.set_type(worker::Response::Failure::WrongRequest);
		
		onFailed(failure);
	}
	else
	{
		debug_worker << "Got response, worker success " << "(" << r.id() << ")" << endl;

		this->args.fitness.clear();
		this->args.response = r;
		
		for (int i = 0; i < r.fitness_size(); ++i)
			this->args.fitness[r.fitness(i).name()] = r.fitness(i).value();

		this->args.job.failed(0);
		onSuccess(this->args);
	}

	return false;
}
