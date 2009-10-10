/*
 *  add.cc - This file is part of optimaster
 *
 *  Copyright (C) 2009 - Jesse van den Kieboom
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#include "workerpool.ih"

Worker WorkerPool::add(string const &connection) 
{
	/* Check if already registered */
	if (d_data->allWorkers.find(connection) != d_data->allWorkers.end())
	{
		debug_worker << "Worker is already registered: " << connection << endl;
		return Worker();
	}
	
	debug_worker << "Adding worker: " << connection << endl;

	/* Connect to worker */
	Worker worker(Client::resolve<Worker>(AddressInfo::Parse(connection)));
	
	if (worker)
	{
		debug_worker << "Ready for work: " << connection << endl;
	
		d_data->allWorkers[connection] = worker;
		d_data->idleWorkers.push_back(worker);
		
		worker.onClosed().addData(*d_data, &WorkerPool::Data::onWorkerClosed, worker);
		worker.onResponse().addData(*d_data, &WorkerPool::Data::onWorkerResponse, worker);
		
		d_data->onWorkerIdle();
	}
	else
	{
		debug_worker << "Could not connect to worker: " << connection << endl;
	}
	
	return worker;
}
