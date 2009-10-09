#include "optimizer.ih"

Optimizer::Optimizer(network::Client &client)
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->client = client;
	d_data->client.onData().add(*d_data, &Optimizer::Data::onClientData);
	
	d_data->id = client.fd() >= 0 ? static_cast<size_t>(client.fd()) : 0;
}
