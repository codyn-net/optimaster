#include "optimizer.ih"

void Optimizer::response(optimization::messages::task::Response &response)
{
	string serialized;
	
	if (Messages::create(response, serialized))
	{
		d_data->client.write(serialized);
	}
}
