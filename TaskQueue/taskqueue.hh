/*
 *  taskqueue.hh - This file is part of optimaster
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

#ifndef __MASTER_TASK_QUEUE_H__
#define __MASTER_TASK_QUEUE_H__

#include <base/base.hh>
#include "Task/task.hh"
#include <list>

namespace optimaster
{
	class TaskQueue : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::list<taskqueue::Task> queue;
			double prioritySum;
		};

		Data *d_data;

		public:
			/* Constructor/destructor */
			TaskQueue();
		
			/* Public functions */
			void queue(size_t id, optimization::messages::task::Batch &tasks);
			void queue(taskqueue::Task &task);

			bool pop(taskqueue::Task &item);
			
			bool empty() const;
			
			void remove(size_t id);
		private:
			/* Private functions */
			bool insert(taskqueue::Task &item);
			void updatePrioritySum(double num);
	};
}

#endif /* __MASTER_TASK_QUEUE_H__ */
