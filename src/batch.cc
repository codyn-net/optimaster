/*
 * batch.cc
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

#include "batch.hh"

using namespace optimaster;
using namespace optimization::messages;

/**
 * @class optimaster::Batch
 * @brief Class containing a single batch description.
 */

/**
 * @brief Batch constructor.
 * @param id The batch group id
 * @param batch The batch
 *
 * Constructor.
 *
 * Create a new batch object.
 * \fn Batch::Batch(size_t id, optimization::messages::batch::Batch const &batch)
 */
Batch::Batch(size_t id, double bias, task::Batch const &batch) 
{
	d_data = new Data();
	AddPrivateData(d_data);
	
	d_data->id = id;
	d_data->bias = bias > 0 ? bias : 1;

	d_data->timeout = batch.has_timeout() ? batch.timeout() : -1;
	d_data->priority = batch.priority() > 0 ? batch.priority() : 1;
	d_data->waitTime = 0;

	// Create tasks from the batch
	for (size_t i = 0; i < batch.tasks_size(); ++i)
	{
		Push(Task(id, batch.tasks(i)));
	}
}

/**
 * @brief Empty batch constructor.
 *
 * Constructor.
 *
 * Create new batch object. This object created by this constructor is not valid.
 *
 */
Batch::Batch()
{
}

/**
 * @brief Cloneable clone implementation (const).
 *
 * Create clone of the object.
 *
 * @return: a clone of the object
 *
 */
Batch *
Batch::Clone() const
{
	return new Batch(*this);
}

/**
 * @brief Batch equality.
 *
 * Test for batch id
 *
 * @return: true of the batch matches the id
 *
 */
bool
Batch::operator==(size_t id) const
{
	return d_data->id == id;
}

/**
 * @brief Get the batch id (const).
 *
 * Get the batch id.
 *
 * @return: the batch id
 *
 */
size_t
Batch::Id() const
{
	return d_data->id;
}

double
Batch::Priority() const
{
	return d_data->priority;
}

double
Batch::Bias() const
{
	return d_data->bias;
}

double
Batch::Timeout() const
{
	return d_data->timeout;
}

bool
Batch::Pop(Task &task)
{
	if (d_data->tasks.empty())
	{
		return false;
	}
	
	task = d_data->tasks.front();
	d_data->tasks.pop_front();
	
	return true;
}

void
Batch::Push(Task const &task)
{
	d_data->tasks.push_back(task);
}

bool
Batch::Empty() const
{
	return d_data->tasks.empty();
}

void
Batch::Wait()
{
	d_data->waitTime += d_data->priority / d_data->bias;
}

void
Batch::WaitReset()
{
	d_data->waitTime = 0;
}

double
Batch::WaitTime() const
{
	return d_data->waitTime;
}

bool
Batch::operator>(Batch const &other) const
{
	return WaitTime() > other.WaitTime();
}

size_t
Batch::Size() const
{
	return d_data->tasks.size();
}
