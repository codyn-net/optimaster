/*
 *  queue1.cc - This file is part of optimaster
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

#include "taskqueue.ih"

void TaskQueue::queue(size_t id, task::Batch &tasks) 
{
	if (tasks.tasks_size() == 0)
	{
		return;
	}
	
	Cloneable<taskqueue::Task> prev;
	updatePrioritySum(tasks.tasks_size() * tasks.priority());
	
	for (size_t idx = 0; idx < tasks.tasks_size(); ++idx)
	{
		taskqueue::Task task(id, tasks.priority(), tasks.tasks(idx));
		task.setOvertake(d_data->prioritySum / tasks.priority());
		
		if (insert(task))
		{
			if (prev)
			{
				prev->sequence(task);
			}
			
			prev = task;
		}
	}
}
