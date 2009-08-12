#include "item.ih"

void JobQueue::Item::sequence(JobQueue::Item &other) 
{
	d_data->nextInSequence = other;
	other.d_data->overtake += d_data->overtake;
}
