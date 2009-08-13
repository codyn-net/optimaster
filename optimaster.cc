#include "Application/application.hh"

#include <glibmm.h>

using namespace optimaster;

int 
main(int argc, char *argv[])
{
	Glib::init();
	Glib::RefPtr<Glib::MainLoop> loop = Glib::MainLoop::create();

	Application app(argc, argv);
	
	app.run(loop);
	
	return 0;
}
