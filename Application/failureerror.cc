#include "application.ih"

string Application::failureError(worker::Response::Failure const &failure) const
{
	switch (failure.type())
	{
		case worker::Response::Failure::Timeout:
			return "Timeout";
		break;
		case worker::Response::Failure::DispatcherNotFound:
			return "Dispatcher not found";
		break;
		case worker::Response::Failure::NoResponse:
			return "No response";
		break;
		case worker::Response::Failure::Dispatcher:
			return "Dispatcher problem";
		break;
		case worker::Response::Failure::Unknown:
			return "Unknown error";
		break;
		case worker::Response::Failure::WrongRequest:
			return "Wrong request/response";
		break;
		case worker::Response::Failure::Disconnected:
			return "Worker got disconnected";
		break;
	}
	
	return "General error";
}
