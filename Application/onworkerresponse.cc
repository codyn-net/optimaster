/*
 *  onworkerresponse.cc - This file is part of optimaster
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

#include "application.ih"

void Application::onWorkerResponse(task::Response &response, Worker worker)
{
	taskqueue::Task &task = worker.task();
	Optimizer optimizer;
	
	if (!d_optimizerManager.find(task.id(), optimizer))
	{
		return;
	}
	
	if (response.status() == task::Response::Success)
	{
		// Simply relay to optimizer
		if (Debug::enabled(Debug::Domain::Worker))
		{
			debug_worker << "Worker success (" << worker.address().host(true) << ":" << worker.address().port(true) << ") for (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
		}

		optimizer.response(response);
	}
	else
	{
		// Check how many times this task failed
		task.failed();
		
		if (task.failures() >= Config::instance().maxTaskFailures)
		{
			if (Debug::enabled(Debug::Domain::Worker))
			{
				debug_worker << "Task failed to many times (" << worker.address().host(true) << ":" << worker.address().port(true) << ") for (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
			}

			// Relay failure to optimizer
			optimizer.response(response);
		}
		else
		{
			if (Debug::enabled(Debug::Domain::Worker))
			{
				debug_worker << "Task failed, rescheduling (" << worker.address().host(true) << ":" << worker.address().port(true) << ") for (" << worker.task().id() << ", " << worker.task().task().id() << ")" << endl;
			}
		
			// Reschedule task
			d_taskQueue.queue(task);
		}
	}
}
