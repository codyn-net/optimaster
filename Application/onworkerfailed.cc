#include "application.ih"

void Application::onWorkerFailed(optimization::messages::worker::Response::Failure &failure, Worker worker) 
{
	/* Add solution back to the job queue to be run again */
	debug_master << "Worker failed (" << worker.solution().id() << ") " << failureError(failure) << ": " << failureMessage(failure) << endl;
	
	worker.job().optimizer().log(optimization::Optimizer::LogType::Error) << "Worker failed for " << worker.solution().id() << " => " << failureError(failure) << ": " << failureMessage(failure) << optimization::Optimizer::Logger::End();
	
	if (worker.job().failures() >= 5)
	{
		debug_worker << "Job " << worker.job().name() << " failed too many times, removing job" << endl;
		worker.job().optimizer().log(optimization::Optimizer::LogType::Error) << "Job failed too many times" << optimization::Optimizer::Logger::End();
		
		removeJob(worker.job());
	}
	else
	{
		d_jobQueue.queue(worker.job(), worker.solution());
	}
}
