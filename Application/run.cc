#include "application.ih"

void Application::run(Glib::RefPtr<Glib::MainLoop> loop) 
{
	os::Signals::install();
	os::Signals::onInterrupt.addData(*this, &Application::onInterrupt, loop);
	
	if (d_discovery.isMulticast())
	{
		sendWakeup();
	}
	
	d_mainLoop = loop;
	loop->run();
	
	// Close off workers
	d_workerPool.close();
	
	// Remove all jobs
	vector<Job> all = jobs();

	for (vector<Job>::iterator iter = all.begin(); iter != all.end(); ++iter)
	{
		removeJob(*iter);
	}
	
	if (d_idleWork)
	{
		d_idleWork.disconnect();
	}
}
