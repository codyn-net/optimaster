#include "application.ih"

bool Application::onGreeting(optimization::Discovery::Info &info) 
{
	string protocol;
	string host = info.host;
	string port;
	
	AddressInfo::Split(info.connection, protocol, host, port);
	
	if (host == "" || host == "0.0.0.0")
	{
		host = info.host;
	}

	registerWorker(protocol + "://" + host + ":" + port);
	return false;
}
