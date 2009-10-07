#include "worker.ih"

void Worker::send(taskqueue::Task &task) 
{
	d_data->task = task;
	
	task::Communication communication;
	communication.set_type(task::Communication::CommunicationRequest);
	
	*communication.mutable_task() = task.task();
	string serialized;
	
	if (Messages::create(communication, serialized))
	{
		// Send task to worker
		d_data->working = true;
		write(serialized);
	}
	else
	{
		d_data->setFailure(task::Response::Failure::Unknown);
		d_data->onResponse(d_data->response);
	}
}
