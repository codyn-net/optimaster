#include "task.ih"

void Task::sequence(Task &other) 
{
	d_data->nextInSequence = other;
	other.d_data->overtake += d_data->overtake;
}
