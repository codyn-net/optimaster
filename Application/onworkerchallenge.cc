#include "application.ih"

void Application::onWorkerChallenge(string &challenge, Worker worker)
{
	// Do some calculation with the job token, and write the response
	string response = hashToken(worker.job(), challenge);

	optimization::messages::worker::Communication message;
	optimization::messages::worker::Token &token = *message.mutable_token();
	
	message.set_type(optimization::messages::worker::Communication::CommunicationToken);
	
	token.set_id(worker.solution().id());
	token.set_response(response);
	
	string serialized;
	
	if (optimization::Messages::create(message, serialized))
	{
		worker.write(serialized);
	}
}
