/*
 *  queue2.cc - This file is part of optimaster
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

void TaskQueue::queue(taskqueue::Task &task)
{
	updatePrioritySum(task.priority());
	
	task.setOvertake(d_data->prioritySum / task.priority());
	task.sequence();
	
	bool same = (d_data->queue.begin() == d_data->queue.end() || d_data->queue.back().id() != task.id());
	
	if (insert(task) && same)
	{
		list<taskqueue::Task>::iterator iter = d_data->queue.end();
		
		// task was inserted at the end, so we go 2 back to get the
		// previously last item and squence it with the new item
		--iter;
		--iter;
		
		if (iter != d_data->queue.end())
		{
			iter->sequence(task);
		}
	}
}
