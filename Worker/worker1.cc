#include "worker.ih"

Worker::Worker(AddressInfo &info) 
:
	Client(info)
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->working = false;

	onData().add(*d_data, &Data::onData);
}
