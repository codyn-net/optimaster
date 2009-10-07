#include "task.ih"

void Task::sequenceDecrease(double overtake)
{
	d_data->overtake -= overtake;

	if (d_data->nextInSequence)
	{
		d_data->nextInSequence->sequenceDecrease(d_data->overtake);
	}
}
