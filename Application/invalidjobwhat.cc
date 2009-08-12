#include "application.ih"

char const *Application::InvalidJob::what() const throw()
{
	return d_message.c_str();
}
