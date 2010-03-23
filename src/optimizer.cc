/*
 * optimizer.cc
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

#include "optimizer.hh"
#include "config.hh"

#include <numeric>
#include <optimization/messages.hh>

using namespace std;
using namespace optimaster;
using namespace jessevdk::network;
using namespace optimization::messages;
using namespace optimization;

/**
 * @class optimaster::Optimizer
 * @brief Class representing a optimizer connection.
 */

/**
 * @brief Constructor with client.
 * @param client the client
 *
 * Constructor.
 *
 * Construct an optimizer object for a certain clien.
 *
 */
Optimizer::Optimizer(jessevdk::network::Client &client)
{
	d_data = new Data();
	AddPrivateData(d_data);

	Set(d_data, client);
}

/**
 * @brief Default constructor.
 *
 * Constructor.
 *
 * Default constructor. This does not result in a valid optimizer object.
 *
 */
Optimizer::Optimizer()
{
}

/**
 * @brief Callback called when worker is deactivated.
 * @param worker the worker
 *
 * Called when a worker that was executing a task for the optimizer was 
 * deactivated.
 *
 */
void
Optimizer::Data::OnWorkerDeactivated(Worker &worker)
{
	// Remove worker from list of active workers
	vector<Worker>::iterator iter;
	iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);

	activeWorkers.erase(iter, activeWorkers.end());

	worker.OnDeactivated().Remove(*this, &Optimizer::Data::OnWorkerDeactivated);
	
	Config &config = Config::Instance();
	while (lastRunTimes.size() > config.RunTimeEstimation - 1)
	{
		lastRunTimes.pop_front();
	}
	
	lastRunTimes.push_back(worker.ActiveTask().LastRunTime());
}

/**
 * @brief Add worker activated for the optimizer.
 * @param worker the worker
 *
 * This should be called when a worker is activated for a task of the 
 * optimizer. The optimizer keeps track of which workers are executing tasks
 * belonging to the optimizer.
 *
 */
void
Optimizer::Add(Worker &worker)
{
	d_data->activeWorkers.push_back(worker);
	worker.OnDeactivated().Add(*d_data, &Optimizer::Data::OnWorkerDeactivated);
}

/**
 * @brief Get active workers.
 *
 * Get all the workers currently executing tasks for the optimizer.
 *
 * @return: the active workers
 *
 */
vector<Worker> &
Optimizer::ActiveWorkers()
{
	return d_data->activeWorkers;
}

/** \brief Get average task run time.
 * 
 * Returns the average run time of tasks from this optimizer.
 *
 * @return: Average run time in seconds
 *
 */

double
Optimizer::AverageRunTime() const
{
	size_t num = d_data->lastRunTimes.size();
	
	if (num == 0)
	{
		return 0;
	}
	
	double sum = std::accumulate(d_data->lastRunTimes.begin(), d_data->lastRunTimes.end(), 0.0);
	return sum / num;
}
