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
#include "debug.hh"

using namespace std;
using namespace optimaster;
using namespace jessevdk::base;
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
:
	d_size(0)
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

		debug_scheduler << "Increasing wait time on " << batch.Id() << ": " << batch.WaitTime() << " (" << batch.Priority() << ", " << batch.Bias() << ")" << endl;
		if ((!maxBatch || batch.WaitTime() > maxWaitTime) && !batch.Empty())
		{
			maxBatch = batch;
			maxWaitTime = batch.WaitTime();
		}
	}

	if (maxBatch)
	{
		debug_scheduler << "Found highest priority batch " << maxBatch->Id() << ": " << maxBatch->WaitTime() << endl;

		maxBatch->WaitReset();
		maxBatch->Pop(task);

		d_running[maxBatch->Id()][task.Id()] = true;
		--d_size;

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
                double             priority,
                double             timeout,
                task::Batch const &batch)
{
	if (batch.tasks_size() == 0)
	{
		return;
	}

	Batch ret;

	if (Lookup(id, ret))
	{
		debug_scheduler << "Scheduled existing batch: [id = " << id << ", bias = " << bias << "]: " << batch.tasks_size() << endl;

		for (size_t i = 0; i < batch.tasks_size(); ++i)
		{
			Task task = Task(id, batch.tasks(i));
			ret.Push(task);
			d_running[id].erase(task.Id());
		}
	}
	else
	{
		debug_scheduler << "Scheduled new batch: [id = " << id << ", bias = " << bias << "]: " << batch.tasks_size() << endl;

		d_batches[id] = Batch(id, bias, priority, timeout, batch);
		d_running[id] = map<size_t, bool>();
	}

	d_size += batch.tasks_size();

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
	
	if (Lookup(task.Group(), batch))
	{
		debug_scheduler << "Pushing single task in batch: [group = " << task.Group() << ", id = " << task.Id() << "]" << endl;
		
		batch.Push(task);
		d_running[batch.Id()].erase(task.Id());

		++d_size;

		OnNotifyAvailable();
	}
	else
	{
		debug_scheduler << "Batch not found while pushing single task: [group = " << task.Group() << ", id = " << task.Id() << "]" << endl;
	}
}

void
TaskQueue::Finished(Task const &task)
{
	Batch batch;
	
	if (Lookup(task.Group(), batch))
	{
		--d_size;

		map<size_t, bool> &mapping = d_running[task.Group()];
		mapping.erase(task.Id());

		if (batch.Empty() && mapping.empty())
		{
			debug_scheduler << "Removing batch after last task finished: [id = " << task.Group() << "]" << endl;
			Remove(batch.Id());
		}
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
		debug_scheduler << "Removing batch: [id = " << id << "]" << endl;

		d_size -= iter->second.Size();

		d_batches.erase(iter);
		d_running.erase(id);
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

size_t
TaskQueue::Size() const
{
	return d_size;
}
