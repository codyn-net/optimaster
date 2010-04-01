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
#include <jessevdk/network/network.hh>

using namespace std;
using namespace jessevdk::base;
using namespace optimaster;
using namespace optimization::messages;
using namespace jessevdk::network;

/**
 * @class optimaster::WorkerManager
 * @brief Class managing worker connections.
 */

/**
 * @brief Add new worker to the manager.
 * @param connection the connection string describing the worker location
 * @param worker worker return value
 *
 * Add a new worker to the manager. Usually the worker notifies itself through
 * automatic worker discovery to the master notifying a certain connection
 * string which is then added to the worker manager.
 *
 * This function will immediately try to connect to the worker and will return
 * false if the connection could not be made.
 *
 * @return true if the worker was registered, false otherwise
 *
 */
bool
WorkerManager::Add(std::string const &connection, Worker &worker) 
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

		if (info.Protocol() == Socket::Level::Tcp)
		{
			worker.Client().SetOption(Socket::Level::Tcp, Socket::Options::NoDelay);
		}

		// Add worker to the relevant maps and queues
		d_workersHash[connection] = worker;
		d_workers[worker.Id()] = worker;
		d_idleWorkers.push_back(worker);

		// Notification of worker closing
		worker.OnClosed().Add(*this, &WorkerManager::OnWorkerClosed);

		// Activation notification
		worker.OnActivated().Add(*this, &WorkerManager::OnWorkerActivated);
		worker.OnDeactivated().Add(*this, &WorkerManager::OnWorkerDeactivated);

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

WorkerManager::~WorkerManager()
{
	map<size_t, Worker> workers = d_workers;
	map<size_t, Worker>::iterator iter;
	
	for (iter = workers.begin(); iter != workers.end(); ++iter)
	{
		RemoveWorker(iter->second);
	}
}

/**
 * @brief Add new worker to the manager.
 * @param connection the connection describing the worker location
 *
 * Add a new worker to the manager.
 *
 * @return: true if the worker was added successfully, false otherwise
 *
 */
bool
WorkerManager::Add(std::string const &connection)
{
	Worker worker;

	return Add(connection, worker);
}

/**
 * @brief Callback for closing worker connection.
 * @param fd the worker id
 *
 * This callback is called when the connection with the worker has closed.
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

	if (Debug::Enabled(optimization::Debug::Domain::Worker))
	{
		debug_worker << "Worker disconnected: "
		             << worker.Client().Address().Host(true)
		             << ":"
		             << worker.Client().Address().Port(true)
		             << endl;
	}

	// Actually remove the worker
	RemoveWorker(worker);
}

/**
 * @brief Remove worker from the manager.
 * @param worker the worker to remove
 *
 * This function removes the worker from the manager, properly unregistering
 * any of the connected signals and removing it from all the managed queues and
 * maps.
 *
 *
 */
void
WorkerManager::RemoveWorker(Worker &worker)
{
	// Remove from active list
	d_activeWorkers.erase(worker.Id());

	deque<Worker>::iterator iter = remove(d_idleWorkers.begin(), d_idleWorkers.end(), worker);
	d_idleWorkers.erase(iter, d_idleWorkers.end());

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
	worker.OnActivated().Remove(*this, &WorkerManager::OnWorkerActivated);
	worker.OnDeactivated().Remove(*this, &WorkerManager::OnWorkerDeactivated);

	worker.OnClosed().Remove(*this, &WorkerManager::OnWorkerClosed);

	// Emit the signal
	OnRemoved(worker);

	if (worker.Active())
	{
		worker.Deactivate();
	}

	// And finally close the worker
	worker.Client().Close();
}

/**
 * @brief Find a worker given a worker id.
 * @param id the worker id
 * @param worker a return value for the worker if found
 *
 * Find a worker with the specified id.
 *
 *
 * @return true if the worker with id could be found, false otherwise
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

/**
 * @brief Check whether there are idle workers.
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

	worker = d_idleWorkers.front();
	d_idleWorkers.pop_front();
	d_idleWorkers.push_back(worker);

	return true;
}

/**
 * @brief Callback called when worker is activated.
 * @param worker the worker that is activated
 *
 * Called when worker is activated.
 *
 */
void
WorkerManager::OnWorkerActivated(Worker &worker)
{
	d_activeWorkers[worker.Id()] = worker;

	deque<Worker>::iterator iter = remove(d_idleWorkers.begin(), d_idleWorkers.end(), worker);
	d_idleWorkers.erase(iter, d_idleWorkers.end());
}

/**
 * @brief Callback called when worker is deactivated.
 * @param worker the worker that is deactivated
 *
 * Called when worker is deactivated.
 *
 */
void
WorkerManager::OnWorkerDeactivated(Worker &worker)
{
	d_activeWorkers.erase(worker.Id());

	d_idleWorkers.push_back(worker);
	OnNotifyAvailable();
}

size_t
WorkerManager::Size() const
{
	return d_workers.size();
}

size_t
WorkerManager::Active() const
{
	return d_activeWorkers.size();
}

double
WorkerManager::ResetIdleTime()
{
	deque<Worker>::iterator iter;
	double ret;

	for (iter = d_idleWorkers.begin(); iter != d_idleWorkers.end(); ++iter)
	{
		ret += iter->IdleTime().elapsed();
		iter->IdleTime().start();
	}

	return ret;
}
