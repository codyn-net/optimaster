/*
 * task.cc
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

#include "task.hh"

using namespace optimaster;
using namespace optimization::messages;

/**
 * @class optimaster::Task
 * @brief Class containing a single task description.
 */

/**
 * @brief Task constructor.
 * @param id The task group id
 * @param task The task
 *
 * Constructor.
 *
 * Create a new task object.
 * \fn Task::Task(size_t id, optimization::messages::task::Task const &task)
 */
Task::Task(size_t id, task::Task const &task) 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->id = id;
	d_data->task = task;
	d_data->failures = 0;
	d_data->lastRunTime = 0;
}

/**
 * @brief Empty task constructor.
 *
 * Constructor.
 *
 * Create new task object. This object created by this constructor is not valid.
 *
 */
Task::Task()
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
Task *
Task::clone() const
{
	return new Task(*this);
}

/**
 * @brief Increase task failure.
 *
 * Indicates a task failure. The task keeps track of the number of failures.
 *
 */
void
Task::Failed()
{
	++d_data->failures;
}

/**
 * @brief Number of failures.
 *
 * The number of times tasks have failed.
 *
 * @return: the number of failures
 *
 */
size_t
Task::Failures()
{
	return d_data->failures;
}

/**
 * @brief Task equality.
 *
 * Test for task id
 *
 * @return: true of the task matches the id
 *
 */
bool
Task::operator==(size_t id) const
{
	return d_data->id == id;
}

/**
 * @brief Get task message associated to the task.
 *
 * Get the task message associated to this task.
 *
 * @return: the task message
 *
 */
optimization::messages::task::Task &
Task::Message()
{
	return d_data->task;
}

/**
 * @brief Get task message associated to the task (const).
 *
 * Get the task message associated to this task.
 *
 * @return: the task message
 *
 */
optimization::messages::task::Task const &
Task::Message() const
{
	return d_data->task;
}

/**
 * @brief Get the task id (const).
 *
 * Get the task id.
 *
 * @return: the task id
 *
 */
size_t
Task::Id() const
{
	return d_data->id;
}

void
Task::Begin()
{
	d_data->started.assign_current_time();
	d_data->lastRunTime = 0;
}

void
Task::End()
{
	Glib::TimeVal tv;
	tv.assign_current_time();
	
	d_data->lastRunTime = (tv - d_data->started).as_double();
}

/**
 * @brief Get the time to run the last task (const).
 *
 * Returns the time it took to run the last task in seconds.
 *
 * @return: the last run time in seconds
 *
 */
double
Task::LastRunTime() const
{
	return d_data->lastRunTime;
}
