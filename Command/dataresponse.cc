#include "command.ih"

void Command::Data::response(Client &client, command::CommandType type, bool status, string const &data) 
{
	command::Response response = createResponse(type, status, data);
	
	string serialized;
	
	if (Messages::create(response, serialized))
	{
		client.write(serialized);
	}
}
