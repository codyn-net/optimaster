#include "task.ih"

Task *Task::clone() const
{
	return new Task(*this);
}
