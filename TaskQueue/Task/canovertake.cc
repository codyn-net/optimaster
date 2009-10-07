#include "task.ih"

bool Task::canOvertake(Task const &other) const
{
	return overtake() > other.overtake();
}
