/*
 * taskqueue.cc
 * This file is part of optimaster
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
 *
 * optimaster is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * optimaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with optimaster; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#include "taskqueue.hh"

using namespace std;
using namespace optimaster;
using namespace base;
using namespace optimization::messages;

TaskQueue::TaskQueue() 
{
	d_prioritySum = 1;
}

bool
TaskQueue::Empty() const
{
	return d_queue.empty();
}

bool
TaskQueue::Insert(Task &task) 
{
	// Return true if task is inserted at the end of the queue
	list<Task>::reverse_iterator iter;

	for (iter = d_queue.rbegin(); iter != d_queue.rend(); ++iter)
	{
		if (!task.CanOvertake(*iter))
			break;
	}

	if (iter == d_queue.rbegin())
	{
		d_queue.push_back(task);
		return true;
	}
	else
	{
		--iter;

		task.Overtake(*iter);
		d_queue.insert(iter.base(), task);
		return false;
	}
}

bool
TaskQueue::Pop(Task &task)
{
	if (d_queue.empty())
	{
		return false;
	}

	task = d_queue.front();
	d_queue.pop_front();

	// TODO: update priority sum?
	return true;
}

void
TaskQueue::Queue(size_t             id,
                 task::Batch const &batch) 
{
	if (batch.tasks_size() == 0)
	{
		return;
	}

	Cloneable<Task> prev;
	UpdatePrioritySum(batch.tasks_size() * batch.priority());
	bool inserted = false;

	for (size_t idx = 0; idx < batch.tasks_size(); ++idx)
	{
		Task task(id, batch.priority(), batch.tasks(idx));
		task.SetOvertake(d_prioritySum / batch.priority());

		if (Insert(task))
		{
			if (prev)
			{
				prev->Sequence(task);
			}

			prev = task;

			inserted = true;
		}
	}

	if (inserted)
	{
		OnNotifyAvailable();
	}
}

void
TaskQueue::Queue(Task &task)
{
	UpdatePrioritySum(task.Priority());

	task.SetOvertake(d_prioritySum / task.Priority());
	task.Sequence();

	bool same = (d_queue.begin() == d_queue.end() || d_queue.back().Id() != task.Id());

	if (Insert(task) && same)
	{
		list<Task>::iterator iter = d_queue.end();

		// task was inserted at the end, so we go 2 back to get the
		// previously last item and squence it with the new item
		--iter;
		--iter;

		if (iter != d_queue.end())
		{
			iter->Sequence(task);
		}

		OnNotifyAvailable();
	}
}

void
TaskQueue::Remove(size_t id)
{
	std::list<Task>::iterator iter;

	iter = std::remove(d_queue.begin(), d_queue.end(), id);

	if (iter != d_queue.end())
	{
		d_queue.erase(iter);
	}

	// TODO: update priority sum
}

void
TaskQueue::UpdatePrioritySum(double num) 
{
	list<Task>::iterator iter;

	double prioritySum = d_prioritySum + num;
	double ratio = prioritySum / d_prioritySum;

	d_prioritySum = prioritySum;

	for (iter = d_queue.begin(); iter != d_queue.end(); ++iter)
	{
		iter->SetOvertake(iter->Overtake() * ratio);
	}
}
