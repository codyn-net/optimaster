/*
 *  insert.cc - This file is part of optimaster
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

bool TaskQueue::insert(taskqueue::Task &task) 
{
	// Return true if task is inserted at the end of the queue
	list<taskqueue::Task>::reverse_iterator iter;
	
	for (iter = d_data->queue.rbegin(); iter != d_data->queue.rend(); ++iter)
	{
		if (!task.canOvertake(*iter))
			break;
	}
	
	if (iter == d_data->queue.rbegin())
	{
		d_data->queue.push_back(task);
		return true;
	}
	else
	{
		--iter;

		task.overtake(*iter);
		d_data->queue.insert(iter.base(), task);
		return false;
	}
}
