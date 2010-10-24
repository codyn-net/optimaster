/*
 * jobmanager.cc
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

#include "jobmanager.hh"

#include <optimization/constants.hh>

using namespace std;
using namespace optimaster;
using namespace jessevdk::network;
using namespace optimization;
using namespace jessevdk::base;

/**
 * @class optimaster::JobManager
 * @brief Class managing jobs.
 */

/**
 * @brief Default constructor.
 *
 * Constructor.
 *
 * Create a new job manager. After construction, make sure to call
 * Set to set the host and port to listen on, and listen to start listening
 * for new connections.
 *
 */
JobManager::JobManager()
:
	d_server(Constants::MasterPort)
{
	d_server.OnNewConnection().Add(*this, &JobManager::OnNewConnection);
}

/**
 * @brief Destructor.
 *
 * Destructor.
 *
 */
JobManager::~JobManager()
{
	d_server.OnNewConnection().Remove(*this, &JobManager::OnNewConnection);

	map<int, Job>::iterator iter;

	for (iter = d_jobs.begin(); iter != d_jobs.end(); ++iter)
	{
		iter->second.OnClosed().Remove(*this, &JobManager::OnJobClosed);
	}

	d_jobs.clear();
}

/**
 * @brief Find job given an id.
 * @param id job id
 * @param job job return value
 *
 * Find a connected job by id.
 *
 * @return: true if job could be found, false otherwise
 *
 */
bool
JobManager::Find(size_t     id,
                       Job &job)
{
	map<int, Job>::iterator found;

	found = d_jobs.find(id);

	if (found != d_jobs.end())
	{
		job = found->second;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Start listening for job connections.
 *
 * Start listening for new job connections.
 *
 * @return: true if the manager is now listening, false otherwise
 */
bool
JobManager::Listen()
{
	return d_server.Listen();
}

JobManager::operator bool() const
{
	return d_server;
}

void
JobManager::OnJobClosed(Communicator &communicator)
{
	Job job;

	if (Find(communicator.Id(), job))
	{
		d_jobs.erase(job.Id());
		OnRemoved(job);
	}
}

void
JobManager::OnNewConnection(Client &client)
{
	Job job(client);
	d_jobs[job.Id()] = job;

	job.OnClosed().Add(*this, &JobManager::OnJobClosed);

	if (jessevdk::base::Debug::Enabled(optimization::Debug::Domain::Master))
	{
		debug_master << "New job connected: "
		             << job.Id() << " ("
		             << client.Address().Host(true) << ":"
		             << client.Address().Port(true) << ")" << endl;
	}

	client.SetOption(Socket::Level::Tcp, Socket::Options::NoDelay);

	OnAdded(job);
}

/**
 * @brief Set host and port on which to listen for job connections.
 * @param host the host to listen on
 * @param port the port to listen on
 *
 * Set the host and the port on which to listen for new job connections.
 * \fn JobManager::Set(std::string const &host, std::string const &port)
 */
void
JobManager::Set(string const &host,
                      string const &port)
{
	d_server.Set(host, port);
}

vector<Job>
JobManager::Jobs() const
{
	map<int, Job>::const_iterator iter;
	vector<Job> ret;

	for (iter = d_jobs.begin(); iter != d_jobs.end(); ++iter)
	{
		ret.push_back(iter->second);
	}

	return ret;
}
