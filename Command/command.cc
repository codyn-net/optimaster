#include "command.ih"

Command::Command(Application &application) 
:
	TcpServer(optimization::Constants::CommandPort)
{
	d_data = new Data(application);
	addPrivateData(d_data);

	onNewConnection().add(*d_data, &Data::onNewConnection);
	
	d_data->signalClient = Client::Multicast("225.0.0.56", optimization::Constants::CommandPort, 2);
}
