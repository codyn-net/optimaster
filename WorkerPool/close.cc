/*
 *  close.cc - This file is part of optimaster
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

void WorkerPool::close()
{
	deque<Worker>::iterator iter;
	deque<Worker> cpc = d_data->activeWorkers;
	
	// Close connections to all the active workers first
	for (iter = cpc.begin(); iter != cpc.end(); ++iter)
	{
		iter->close();
	}
	
	cpc.clear();
	
	// REmove all the workers that are still left
	map<string, Worker> cp = d_data->allWorkers;
	map<string, Worker>::iterator it;
	
	for (it = cp.begin(); it != cp.end(); ++it)
	{
		d_data->removeWorker(it->second);
	}
	
	// Clear them all	
	d_data->activeWorkers.clear();
	d_data->idleWorkers.clear();
	d_data->allWorkers.clear();
}
