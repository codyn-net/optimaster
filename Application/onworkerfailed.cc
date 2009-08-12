#include "application.ih"

void Application::onWorkerFailed(bool timeout, Worker worker) 
{
	/* Add solution back to the job queue to be run again */
	debug_master << "Worker failed (" << worker.solution().id() << ")"  << (timeout ? " [timeout]" : "") << endl;
	
	worker.job().optimizer().log(optimization::Optimizer::LogType::Error) << "Worker failed for " << worker.solution().id() << (timeout ? " (timeout)" : "") << optimization::Optimizer::Logger::End();
	
	d_jobQueue.queue(worker.job(), worker.solution());
}
