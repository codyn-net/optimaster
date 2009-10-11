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

Task::Task()
{	
}

bool
Task::CanOvertake(Task const &other) const
{
	return Overtake() > other.Overtake();
}

Task *Task::clone() const
{
	return new Task(*this);
}

void
Task::Failed()
{
	++d_data->failures;
}

size_t
Task::Failures()
{
	return d_data->failures;
}

bool
Task::operator==(size_t id) const
{
	return d_data->id == id;	
}

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

void
Task::Sequence(Task &other) 
{
	d_data->nextInSequence = other;
	other.d_data->overtake += d_data->overtake;
}

void
Task::Sequence()
{
	d_data->nextInSequence.clear();
}

void
Task::SequenceDecrease(double overtake)
{
	d_data->overtake -= overtake;

	if (d_data->nextInSequence)
	{
		d_data->nextInSequence->SequenceDecrease(d_data->overtake);
	}
}

optimization::messages::task::Task &
Task::Message()
{
	return d_data->task;
}

optimization::messages::task::Task const &
Task::Message() const
{
	return d_data->task;
}

double
Task::Overtake() const
{
	return d_data->overtake;
}

void
Task::SetOvertake(double overtake)
{
	d_data->overtake = overtake;
}

size_t
Task::Id() const
{
	return d_data->id;
}

double
Task::Priority() const
{
	return d_data->priority;
}
