/*
 * workermanager.cc
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

#include "workermanager.hh"

#include <algorithm>
#include <network/network.hh>

using namespace std;
using namespace base;
using namespace optimaster;
using namespace optimization::messages;
using namespace network;

/** Add new worker to the manager.
 *
 * Add a new worker to the manager. Usually the worker notifies itself through
 * automatic worker discovery to the master notifying a certain connection
 * string which is then added to the worker manager.
 *
 * This function will immediately try to connect to the worker and will return
 * false if the connection could not be made.
 *
 * @param connection the connection string of the worker
 * @param worker a return value for the added worker
 *
 * @return true if the worker was registered, false otherwise
 *
 */
bool
WorkerManager::Add(string const &connection, Worker &worker) 
{
	// Check if the worker already exists
	if (d_workersHash.find(connection) != d_workersHash.end())
	{
		debug_worker << "Worker is already registered: " << connection << endl;
		return false;
	}

	debug_worker << "Adding worker: " << connection << endl;

	// Try to connect to the worker
	AddressInfo info(AddressInfo::Parse(connection));
	worker = Worker(info);

	if (worker.Client())
	{
		debug_worker << "Ready for work: " << connection << endl;

		// Add worker to the relevant maps and queues
		d_workersHash[connection] = worker;
		d_workers[worker.Id()] = worker;
		d_idleWorkers[worker.Id()] = worker;

		// Notification of worker closing
		worker.OnClosed().add(*this, &WorkerManager::OnWorkerClosed);

		// Emit the Added signal
		OnAdded(worker);
		OnNotifyAvailable();

		return true;
	}
	else
	{
		debug_worker << "Could not connect to worker: " << connection << endl;
		return false;
	}
}

bool
WorkerManager::Add(string const &connection)
{
	Worker worker;

	return Add(connection, worker);
}

/** Callback for closing worker connection.
 *
 * This callback is called when the connection with the worker has closed.
 *
 * @param fd the worker id
 */
void
WorkerManager::OnWorkerClosed(Communicator &communicator)
{
	// Check if the worker is still registered
	Worker worker;

	if (!Find(communicator.Id(), worker))
	{
		return;
	}

	if (Debug::enabled(Debug::Domain::Worker))
	{
		debug_worker << "Worker disconnected: "
		             << worker.Client().address().host(true)
		             << ":"
		             << worker.Client().address().port(true)
		             << endl;
	}

	// Actually remove the worker
	RemoveWorker(worker);
}

/** Remove worker from the manager.
 *
 * This function removes the worker from the manager, properly unregistering
 * any of the connected signals and removing it from all the managed queues and
 * maps.
 *
 * @param worker the worker to remove
 */
void
WorkerManager::RemoveWorker(Worker &worker)
{
	// Remove from active list
	d_activeWorkers.erase(worker.Id());
	d_idleWorkers.erase(worker.Id());
	d_workers.erase(worker.Id());

	// Remove from the hash
	map<string, Worker>::iterator miter;

	for (miter = d_workersHash.begin(); miter != d_workersHash.end(); ++miter)
	{
		if (miter->second == worker)
		{
			d_workersHash.erase(miter);
			break;
		}
	}

	// Remove callbacks on the worker
	worker.OnActivated().remove(*this, &WorkerManager::OnWorkerActivated);
	worker.OnDeactivated().remove(*this, &WorkerManager::OnWorkerDeactivated);

	worker.OnClosed().remove(*this, &WorkerManager::OnWorkerClosed);

	if (worker.Active())
	{
		worker.Deactivate();
	}

	// Emit the signal
	OnRemoved(worker);

	// And finally close the worker
	worker.Client().close();
}

/** Find a worker given a worker id.
 *
 * Find a worker with the specified id.
 *
 * @param id the worker id
 * @param worker a return value for the worker if found
 *
 * @return true if the worker with @id could be found, false otherwise
 */
bool
WorkerManager::Find(size_t id, Worker &worker)
{
	map<size_t, Worker>::iterator found = d_workers.find(id);

	if (found != d_workers.end())
	{
		worker = found->second;
		return true;
	}
	else
	{
		return false;
	}
}

/** Check whether there are idle workers.
 *
 * Check whether there are any workers that are currently idle.
 *
 * @return true if there are idle workers
 */
bool
WorkerManager::Idle(Worker &worker)
{
	if (d_idleWorkers.size() == 0)
	{
		return false;
	}

	worker = d_idleWorkers.begin()->second;
	return true;
}

void
WorkerManager::OnWorkerActivated(Worker &worker)
{
	d_idleWorkers.erase(worker.Id());
	d_activeWorkers[worker.Id()] = worker;
}

void
WorkerManager::OnWorkerDeactivated(Worker &worker)
{
	d_activeWorkers.erase(worker.Id());
	d_idleWorkers[worker.Id()] = worker;
}
