#include "application.ih"

void Application::onWorkerSuccess(Worker::SuccessArgs &args)
{
	// Check if job has been removed in the mean time
	if (d_jobs.find(args.job.name()) == d_jobs.end())
	{
		return;
	}
	
	// Update solution fitness
	args.solution.fitness().update(args.fitness);
	args.job.optimizer().customResponse(args.solution, args.response);
	
	debug_master << "Worker successfully finished for " << args.job.name() << "(" << args.solution.id() << ", " << String::join(args.solution.parameters().values(), ",") << "): " << String::join(args.solution.fitness().values(), ", ") << " => " << args.solution.fitness().value() << endl;
	
	/* See if all solutions for the job are done */
	if (d_jobQueue.inQueue(args.job) == 0 && d_workerPool.active(args.job) == 0)
	{
		newIteration(args.job);
	}
}
