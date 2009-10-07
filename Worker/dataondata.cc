#include "worker.ih"

bool Worker::Data::onData(os::FileDescriptor::DataArgs &args) 
{
	vector<task::Response> responses;
	vector<task::Response>::iterator iter;
	
	Messages::extract(args, responses);
	
	if (responses.begin() == responses.end())
	{
		return false;
	}
	
	// Take the last response
	response = responses[responses.size() - 1];
	
	if  (response.status() == task::Response::Challenge)
	{
		// We are being challenged
		string challenge = response.challenge();
		onChallenge(challenge);
		return false;
	}
	
	// Check status
	if (response.status() != task::Response::Success)
	{
		if (!response.has_failure())
		{
			setFailure(task::Response::Failure::Unknown);
		}
	}
	else if (task.task().id() != response.id())
	{
		// Response for the wrong task
		setFailure(task::Response::Failure::WrongRequest);
	}

	working = false;
	onResponse(response);

	return false;
}
