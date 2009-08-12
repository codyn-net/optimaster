#include "job.ih"

Job::Data::~Data()
{
	if (optimizer)
	{
		optimizer->clear();
	}
}
