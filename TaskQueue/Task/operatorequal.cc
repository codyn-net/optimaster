#include "task.ih"

bool Task::operator==(size_t id) const
{
	return d_data->id == id;	
}
