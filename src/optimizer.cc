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

#include <optimization/Messages/messages.hh>

using namespace std;
using namespace optimaster;
using namespace network;
using namespace optimization::messages;
using namespace optimization;

Optimizer::Optimizer(network::Client &client)
{
	d_data = new Data();
	addPrivateData(d_data);

	Set(d_data, client);
}

Optimizer::Optimizer()
{
}

void
Optimizer::Data::OnWorkerDeactivated(Worker &worker)
{
	// Remove worker from list of active workers
	vector<Worker>::iterator iter;
	iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);
	
	activeWorkers.erase(iter, activeWorkers.end());
	
	worker.OnDeactivated().remove(*this, &Optimizer::Data::OnWorkerDeactivated);
}

void
Optimizer::Add(Worker &worker)
{
	d_data->activeWorkers.push_back(worker);
	worker.OnDeactivated().add(*d_data, &Optimizer::Data::OnWorkerDeactivated);
}

vector<Worker> &
Optimizer::ActiveWorkers()
{
	return d_data->activeWorkers;
}
