#include "optimizer.ih"

void Optimizer::Data::onClientData(os::FileDescriptor::DataArgs args)
{
	vector<task::Batch> messages;
	Messages::extract(args, messages);
	
	vector<task::Batch>::iterator iter;
	
	for (iter = messages.begin(); iter != messages.end(); ++iter)
	{
		task::Batch &batch = *iter;
		onBatch(batch);
	}
}
