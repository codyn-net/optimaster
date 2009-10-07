#include "task.ih"

void Task::overtake(Task &other) 
{
	other.d_data->overtake -= d_data->overtake;
	
	if (d_data->nextInSequence)
	{
		d_data->nextInSequence->sequenceDecrease(d_data->overtake);
		d_data->nextInSequence.clear();
	}
}
