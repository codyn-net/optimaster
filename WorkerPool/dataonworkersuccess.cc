#include "workerpool.ih"

void WorkerPool::Data::onWorkerSuccess(Worker::SuccessArgs &args, Worker worker) 
{
	workerFinished(worker);
}
