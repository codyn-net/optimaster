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

/**
 * @class optimaster::TaskQueue
 * @brief Class to queue and schedule tasks.
 */

/**
 * @brief Default task queue constructor.
 *
 * Constructor.
 *
 * Create a new task queue.
 *
 */
TaskQueue::TaskQueue() 
{
}

/**
 * @brief Get whether the queue is empty (const).
 *
 * Get whether the queue is empty.
 *
 * @return: true if the queue is empty, false otherwise
 *
 */
bool
TaskQueue::Empty() const
{
	return d_batches.empty();
}

/**
 * @brief Pop the next task of the queue.
 * @param task the task return value
 *
 * Pop the next task of the queue.
 *
 * @return: true if a task was popped of the queue, false otherwise
 *
 */
bool
TaskQueue::Pop(Task &task)
{
	double maxWaitTime = 0;
	Cloneable<Batch> maxBatch;

	// Find batch with the highest wait time, reset it and tick them
	for (map<size_t, Batch>::iterator iter = d_batches.begin(); iter != d_batches.end(); ++iter)
	{
		Batch &batch = iter->second;
		batch.Wait();

		if (batch.WaitTime() > maxWaitTime && !batch.Empty())
		{
			maxBatch = batch;
			maxWaitTime = batch.WaitTime();
		}
	}

	if (maxBatch)
	{
		maxBatch->WaitReset();
		maxBatch->Pop(task);

		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Queue batch of tasks.
 * @param id the task group id
 * @param bias the bias
 * @param batch a batch of tasks to add
 *
 * Adds a batch of tasks to the task queue.
 * \fn TaskQueue::Queue(size_t id, optimization::messages::task::Batch const &batch)
 */
void
TaskQueue::Push(size_t             id,
                double             bias,
                task::Batch const &batch)
{
	if (batch.tasks_size() == 0)
	{
		return;
	}

	Batch ret;

	if (Lookup(id, ret))
	{
		for (size_t i = 0; i < batch.tasks_size(); ++i)
		{
			ret.Push(Task(id, batch.tasks(i)));
		}
	}
	else
	{
		d_batches[id] = Batch(id, bias, batch);
	}

	OnNotifyAvailable();
}

/**
 * @brief Queue a task.
 * @param task a task
 *
 * Queue a task in the task queue.
 *
 */
void
TaskQueue::Push(Task &task)
{
	Batch batch;
	
	if (Lookup(task.Id(), batch))
	{
		batch.Push(task);
		OnNotifyAvailable();
	}
}

void
TaskQueue::Finished(Task const &task)
{
	Batch batch;
	
	if (Lookup(task.Id(), batch) && batch.Empty())
	{
		Remove(batch.Id());
	}
}

/**
 * @brief Remove task group from the queue.
 * @param id the task group id
 *
 * Removes all tasks in the task group from the queue.
 *
 */
void
TaskQueue::Remove(size_t id)
{
	std::map<size_t, Batch>::iterator iter = d_batches.find(id);

	if (iter != d_batches.end())
	{
		d_batches.erase(iter);
	}
}

bool
TaskQueue::Lookup(size_t id, Batch &batch)
{
	std::map<size_t, Batch>::iterator iter = d_batches.find(id);

	if (iter != d_batches.end())
	{
		batch = iter->second;
		return true;
	}
	
	return false;
}
