#include "item.ih"

void JobQueue::Item::overtake(Item &other) 
{
	other.d_data->overtake -= d_data->overtake;
	
	if (d_data->nextInSequence)
	{
		d_data->nextInSequence->sequenceDecrease(d_data->overtake);
		d_data->nextInSequence.clear();
	}
}
