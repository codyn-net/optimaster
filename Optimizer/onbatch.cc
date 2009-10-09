#include "optimizer.ih"

base::signals::Signal<optimization::messages::task::Batch> &Optimizer::onBatch()
{
	return d_data->onBatch;
}
