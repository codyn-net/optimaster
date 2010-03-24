/*
 * job.cc
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

#include "job.hh"
#include "config.hh"

#include <numeric>
#include <optimization/messages.hh>

using namespace std;
using namespace optimaster;
using namespace jessevdk::network;
using namespace optimization::messages;
using namespace optimization;

/**
 * @class optimaster::Job
 * @brief Class representing a job connection.
 */

/**
 * @brief Constructor with client.
 * @param client the client
 *
 * Constructor.
 *
 * Construct an job object for a certain clien.
 *
 */
Job::Job(jessevdk::network::Client &client)
{
	d_data = new Data();
	AddPrivateData(d_data);

	d_data->priority = 1;
	d_data->timeout = -1;

	Set(d_data, client);
}

/**
 * @brief Default constructor.
 *
 * Constructor.
 *
 * Default constructor. This does not result in a valid job object.
 *
 */
Job::Job()
{
}

/**
 * @brief Callback called when worker is deactivated.
 * @param worker the worker
 *
 * Called when a worker that was executing a task for the job was 
 * deactivated.
 *
 */
void
Job::Data::OnWorkerDeactivated(Worker &worker)
{
	// Remove worker from list of active workers
	vector<Worker>::iterator iter;
	iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);

	activeWorkers.erase(iter, activeWorkers.end());

	worker.OnDeactivated().Remove(*this, &Job::Data::OnWorkerDeactivated);
	
	Config &config = Config::Instance();
	while (lastRunTimes.size() > config.RunTimeEstimation - 1)
	{
		lastRunTimes.pop_front();
	}
	
	lastRunTimes.push_back(worker.ActiveTask().LastRunTime());
}

/**
 * @brief Add worker activated for the job.
 * @param worker the worker
 *
 * This should be called when a worker is activated for a task of the 
 * job. The job keeps track of which workers are executing tasks
 * belonging to the job.
 *
 */
void
Job::Add(Worker &worker)
{
	d_data->activeWorkers.push_back(worker);
	worker.OnDeactivated().Add(*d_data, &Job::Data::OnWorkerDeactivated);
}

/**
 * @brief Get active workers.
 *
 * Get all the workers currently executing tasks for the job.
 *
 * @return: the active workers
 *
 */
vector<Worker> &
Job::ActiveWorkers()
{
	return d_data->activeWorkers;
}

/** \brief Get average task run time.
 * 
 * Returns the average run time of tasks from this job.
 *
 * @return: Average run time in seconds
 *
 */

double
Job::AverageRunTime() const
{
	size_t num = d_data->lastRunTimes.size();
	
	if (num == 0)
	{
		return 0;
	}
	
	double sum = std::accumulate(d_data->lastRunTimes.begin(), d_data->lastRunTimes.end(), 0.0);
	return sum / num;
}

double
Job::Priority() const
{
	return d_data->priority;
}

double
Job::Timeout() const
{
	return d_data->timeout;
}

void
Job::SetPriority(double priority)
{
	d_data->priority = priority;
}

void
Job::SetTimeout(double timeout)
{
	d_data->timeout = timeout;
}

string const &
Job::User() const
{
	return d_data->user;
}

void
Job::SetUser(string const &user)
{
	d_data->user = user;
}
