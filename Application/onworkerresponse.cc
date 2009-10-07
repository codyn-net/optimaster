#include "application.ih"

void Application::onWorkerResponse(task::Response &response, Worker worker)
{
	// TODO
	// If the response is a success, relay it to the optimizer connection
	
	// Accept N number of failures before relaying the fail back to the optimizer
	/*
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
	*/
}
