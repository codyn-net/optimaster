#include "jobqueue.ih"

bool JobQueue::insert(Item &item) 
{
	/* Return true if item is inserted at the end of the queue */
	list<Item>::reverse_iterator iter;
	
	for (iter = d_data->queue.rbegin(); iter != d_data->queue.rend(); ++iter)
	{
		if (!item.canOvertake(*iter))
			break;
	}
	
	if (iter == d_data->queue.rbegin())
	{
		d_data->queue.push_back(item);
		return true;
	}
	else
	{
		--iter;

		item.overtake(*iter);
		d_data->queue.insert(iter.base(), item);
		return false;
	}
}
