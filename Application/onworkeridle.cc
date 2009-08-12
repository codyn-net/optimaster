#include "application.ih"

void Application::onIdleWork() 
{
	if (!d_idleWork)
	{
		d_idleWork = Glib::signal_idle().connect(sigc::mem_fun(*this, &Application::onDispatch));
	}
}
