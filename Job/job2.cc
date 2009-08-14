#include "job.ih"

Job::Job(string const &name, optimization::Optimizer &optimizer, optimization::Fitness &fitness, optimization::Parameters &parameters, optimization::Boundaries &boundaries, Dispatcher &dispatcher) 
{
	d_data = new Data();
	addPrivateData(d_data);

	d_data->name = name;
	d_data->optimizer = optimizer;
	d_data->fitness = fitness;
	d_data->parameters = parameters;
	d_data->boundaries = boundaries;
	d_data->dispatcher = dispatcher;
	
	d_data->executionTime = 1;
	d_data->priority = 1;
	d_data->failures = 0;
	
	d_data->optimizer->setup(parameters, boundaries, fitness);
	d_data->optimizer->reset();
}
