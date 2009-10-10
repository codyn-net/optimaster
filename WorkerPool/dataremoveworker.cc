/*
 *  dataremoveworker.cc - This file is part of optimaster
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

void WorkerPool::Data::removeWorker(Worker &worker) 
{
	// Remove from active list
	deque<Worker>::iterator iter = std::remove(activeWorkers.begin(), activeWorkers.end(), worker);
	activeWorkers.erase(iter, activeWorkers.end());
	
	// Remove from idle list
	iter = std::remove(idleWorkers.begin(), idleWorkers.end(), worker);
	idleWorkers.erase(iter, idleWorkers.end());

	// Remove from all workers
	map<string, Worker>::iterator miter;
	
	for (miter = allWorkers.begin(); miter != allWorkers.end(); ++miter)
	{
		if (miter->second == worker)
		{
			allWorkers.erase(miter);
			break;
		}
	}

	worker.onClosed().removeData(*this, &WorkerPool::Data::onWorkerClosed, worker);
	worker.onResponse().removeData(*this, &WorkerPool::Data::onWorkerResponse, worker);

	onWorkerRemoved(worker);
	worker.close();
}
