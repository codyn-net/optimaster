/*
 * worker.cc
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

#include "worker.hh"
#include <optimization/Messages/messages.hh>

#include <algorithm>

using namespace std;
using namespace optimaster;
using namespace network;
using namespace optimization;
using namespace optimization::messages;
using namespace base;
using namespace base::signals;

/** \brief Create stub worker.
 * 
 * Constructor.
 *
 * Creates an empty stub worker object. You cannot use this object since it
 * will not have any data assigned to it. It's purpose is to declare a worker
 * object for later assignment (for instance in functions with a return value
 * argument).
 *
 */
Worker::Worker()
{
}

/** \brief Create worker at address.
 * @param info Address specification for the worker
 * 
 * Constructor.
 *
 * Creates a new worker on a specific network address.
 * \fn Worker::Worker(network::AddressInfo &info)
 */
Worker::Worker(AddressInfo &info)
{
	d_data = new Data();
	addPrivateData(d_data);

	Set(d_data, network::Client(info));
}

/** \brief Get the current worker task.
 * 
 * Get the task currently being executed by the worker.
 *
 * @return: the current task
 *
 */
Task &
Worker::ActiveTask()
{
	return d_data->task;
}

/** \brief Get the current worker task (const).
 * 
 * Get the task currently being executed by the worker.
 *
 * @return: the current task
 *
 */
Task const &
Worker::ActiveTask() const
{
	return d_data->task;
}

/** \brief Signal emitted when worker is activated.
 *
 * When the worker is activated with a certain task, this signal is emitted.
 *
 * @return: the signal object
 *
 */
Signal<Worker> &
Worker::OnActivated()
{
	return d_data->onActivated;
}


/** \brief Signal emitted when worker is deactivated.
 *
 * This signal is emitted when the worker is deactivated.
 *
 * @return: the signal object
 *
 */
Signal<Worker> &
Worker::OnDeactivated()
{
	return d_data->onDeactivated;
}

/** \brief Send task to the worker.
 * @param task The task to send to the worker
 * 
 * Send a task to the worker to be executed.
 *
 * @return: true if the task was send or false otherwise
 *
 */
bool
Worker::Activate(Task &task) 
{
	if (d_data->active)
	{
		return false;
	}

	d_data->task = task;

	task::Communication communication;
	communication.set_type(task::Communication::CommunicationTask);

	*communication.mutable_task() = task.Message();

	return Send(communication);
}

/** \brief Deactivate worker.
 *
 * Deactivate the worker. This makes the worker idle and ready for a new task.
 *
 */
void
Worker::Deactivate()
{
	if (!d_data->active)
	{
		return;
	}

	d_data->active = false;
	d_data->onDeactivated(*this);
}

/** \brief Cancel active task.
 *
 * Cancel the task the worker is currently working on. This will send a
 * TaskCancelled message to the worker before deactivating it.
 *
 * @return: true if the cancellation was send, false otherwise
 *
 */
bool
Worker::Cancel()
{
	if (!d_data->active)
	{
		return false;
	}
	
	task::Communication communication;
	communication.set_type(task::Communication::CommunicationCancel);
	
	communication.mutable_cancel()->set_id(d_data->task.Message().id());

	if (Send(communication))
	{
		Deactivate();
		return true;
	}

	return true;
}

/** \brief Get whether worker is active (const).
 *
 * Returns whether the worker is currently working on a task.
 *
 * @return: true if the worker is active or false otherwise
 *
 */
bool
Worker::Active() const
{
	return d_data->active;
}
