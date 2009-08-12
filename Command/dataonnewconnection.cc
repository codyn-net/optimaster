#include "command.ih"

void Command::Data::onNewConnection(Client &client) 
{
	client.onData().addData(*this, &Data::onData, client);
}
