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
 * @param priority The task priority
 * @param task The task
 *
 * Constructor.
 *
 * Create a new task object.
 * \fn Task::Task(size_t id, double priority, optimization::messages::task::Task const &task)
 */
Task::Task(size_t id, double priority, task::Task const &task) 
{
	d_data = new Data();
	addPrivateData(d_data);
	
	d_data->id = id;
	d_data->priority = priority;
	d_data->task = task;

	d_data->overtake = 0;
	d_data->failures = 0;
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
 * @brief Checks whether the task can overtake some other task (const).
 * @param other the other task
 *
 * Checks whether the current task has enough 'overtake credits' to overtake
 * the other task. This essentially checks if the task has priority over
 * the other task.
 *
 * @return: true if the task can overtake the other task, false otherwise
 *
 */
bool
Task::CanOvertake(Task const &other) const
{
	return Overtake() > other.Overtake();
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
 * @brief Overtake another task.
 * @param other the other task
 *
 * Overtake another task. This decreases the overtake credits of the other
 * task and sequence decreases the overtake of this task.
 *
 */
void
Task::Overtake(Task &other) 
{
	other.d_data->overtake -= d_data->overtake;
	
	if (d_data->nextInSequence)
	{
		d_data->nextInSequence->SequenceDecrease(d_data->overtake);
		d_data->nextInSequence.clear();
	}
}

/**
 * @brief Sequence task after this task.
 * @param other the other task
 *
 * Sequence the other task after the task.
 *
 */
void
Task::Sequence(Task &other) 
{
	d_data->nextInSequence = other;
	other.d_data->overtake += d_data->overtake;
}

/**
 * @brief Reset sequence.
 *
 * Reset the task sequence.
 *
 */
void
Task::Sequence()
{
	d_data->nextInSequence.clear();
}

/**
 * @brief Decrease the sequence.
 * @param overtake overtake to decrease
 *
 * Decreases by overtake, and propagates to next in sequence.
 *
 */
void
Task::SequenceDecrease(double overtake)
{
	d_data->overtake -= overtake;

	if (d_data->nextInSequence)
	{
		d_data->nextInSequence->SequenceDecrease(d_data->overtake);
	}
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
 * @brief Get task overtak (const).
 *
 * Get the task overtak.
 *
 * @return: the task overtake
 *
 */
double
Task::Overtake() const
{
	return d_data->overtake;
}

/**
 * @brief Set the task overtake.
 * @param overtake the task overtake
 *
 * Set the task overtake.
 *
 */
void
Task::SetOvertake(double overtake)
{
	d_data->overtake = overtake;
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

/**
 * @brief Get the task priority (const).
 *
 * Get the task priority.
 *
 * @return: the task priority
 *
 */
double
Task::Priority() const
{
	return d_data->priority;
}
