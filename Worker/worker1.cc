#include "worker.ih"

Worker::Worker(AddressInfo &info) 
:
	Client(info)
{
	d_data = new Data();
	addPrivateData(d_data);
	
	onData().add(*d_data, &Data::onData);
}
