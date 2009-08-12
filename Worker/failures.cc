#include "worker.ih"

size_t Worker::failures() const
{
	return d_data->failures;
}
