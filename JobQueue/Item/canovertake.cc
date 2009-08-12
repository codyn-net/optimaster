#include "item.ih"

bool JobQueue::Item::canOvertake(Item const &other) const
{
	return overtake() > other.overtake();
}
