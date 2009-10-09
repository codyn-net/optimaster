#include "task.ih"

void Task::failed()
{
	++d_data->failures;
}
