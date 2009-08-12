#include "worker.ih"

void Worker::sendJob(Job &job, optimization::Solution &solution) 
{
	d_data->args.job = job;
	d_data->args.solution = solution;
	
	worker::Communication communication;
	communication.set_type(worker::Communication::CommunicationRequest);

	worker::Request &request = *communication.mutable_request();

	/* Id and dispatcher name */
	request.set_id(solution.id());
	request.set_dispatcher(job.dispatcher().name());
	
	/* Dispatch */
	worker::Request::Dispatch *dispatch = request.mutable_dispatch();

	/* Dispatch job and fitness */
	dispatch->set_job(job.name());
	dispatch->set_fitness(solution.fitness().name());

	/* Optimizer stuff */
	dispatch->set_optimizer(job.optimizer().name());

	optimization::Parameters const &parameters = solution.parameters();
	optimization::Parameters::const_iterator iter;
	
	for (iter = parameters.begin(); iter != parameters.end(); ++iter)
	{
		worker::Request::Dispatch::Parameter *parameter;
		
		parameter = dispatch->add_parameters();

		parameter->set_name((*iter)->name);
		parameter->set_value((*iter)->value);
		parameter->set_min((*iter)->boundary.min);
		parameter->set_max((*iter)->boundary.max);
	}

	/* Dispatch settings */
	optimization::Settings &settings = job.dispatcher();
	vector<string> const &properties = settings.properties();
	
	for (size_t i = 0; i < properties.size(); ++i)
	{
		worker::Request::Dispatch::KeyValue *keyval;
		
		keyval = dispatch->add_settings();
		keyval->set_key(properties[i]);
		keyval->set_value(settings.property(properties[i]));
	}

	/* Custom optimizer things */
	job.optimizer().customRequest(solution, request);
	
	string content;
	
	if (Messages::create(communication, content))
	{
		/* Send request */
		write(content);
		
		if (settings.hasProperty("timeout"))
		{
			string tms = settings.property("timeout");
			stringstream s(tms);
			size_t tm;
			
			if ((s >> tm) && tm > 0)
			{
				d_data->timeout = Glib::signal_timeout().connect(sigc::mem_fun(*d_data, &Data::onTimeout), tm * 1000);
			}
		}
	}
	else
	{
		failed(false);
	}
}
