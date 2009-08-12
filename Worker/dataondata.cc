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
	
	if (r.status() != worker::Response::Success || this->args.solution.id() != static_cast<size_t>(r.id()))
	{
		bool timeout = r.has_failure() && r.failure().type() == worker::Response::Failure::Timeout;
		
		if (r.status() != worker::Response::Success)
		{
			debug_worker << "Got response, worker failed (" << r.id() << "), " << r.status() << "..." << endl;
		}
		else
		{
			this->args.job.optimizer().log(optimization::Optimizer::LogType::Error) << "Worker responded with wrong id. Expected " << this->args.solution.id() << " but got " << static_cast<size_t>(r.id()) << optimization::Optimizer::Logger::End();

			debug_worker << "Got response,  but for wrong request..." << endl;
		}
		
		++failures;
		onFailed(timeout);
	}
	else
	{
		debug_worker << "Got response, worker success " << "(" << r.id() << ")" << endl;

		this->args.fitness.clear();
		this->args.response = r;
		
		for (int i = 0; i < r.fitness_size(); ++i)
			this->args.fitness[r.fitness(i).name()] = r.fitness(i).value();

		failures = 0;
		onSuccess(this->args);
	}

	return false;
}
