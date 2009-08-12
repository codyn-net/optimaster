#include "application.ih"

bool Application::onInterrupt(Glib::RefPtr<Glib::MainLoop> loop) 
{
	loop->quit();
	return true;
}
