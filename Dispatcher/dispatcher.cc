#include "dispatcher.ih"

Dispatcher::Dispatcher(string const &name) 
{
	d_data = new Data();
	addPrivateData(d_data);

	d_data->name = name;
}
