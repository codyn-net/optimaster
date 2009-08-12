#include "worker.ih"

Worker::Worker(AddressInfo &info) 
:
	Client(info)
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->failures = 0;

	onData().add(*d_data, &Data::onData);
}
