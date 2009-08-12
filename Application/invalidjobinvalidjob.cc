#include "application.ih"

Application::InvalidJob::InvalidJob(string const &message)
:
	d_message(message)
{
}
