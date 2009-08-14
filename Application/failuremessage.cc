#include "application.ih"

string Application::failureMessage(optimization::messages::worker::Response::Failure const &failure) const
{
	return failure.has_message() ? failure.message() : "";
}
