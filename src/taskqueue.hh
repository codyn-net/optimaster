/*
 * taskqueue.hh
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

#ifndef __OPTIMASTER_TASK_QUEUE_H__
#define __OPTIMASTER_TASK_QUEUE_H__

#include <base/base.hh>
#include "task.hh"
#include <list>

namespace optimaster
{
	class TaskQueue
	{
		std::list<Task> d_queue;
		double d_prioritySum;

		public:
			/* Constructor/destructor */
			TaskQueue();

			/* Public functions */
			void Queue(size_t id, optimization::messages::task::Batch const &batch);
			void Queue(Task &task);

			bool Pop(Task &item);

			bool Empty() const;
			void Remove(size_t id);

			/**
 * @brief OnNotifyAvailable signal
			 *
			 * Signal emitted when new tasks are available on the queue
			 */
			base::signals::Signal<> OnNotifyAvailable;
		private:
			/* Private functions */
			bool Insert(Task &item);
			void UpdatePrioritySum(double num);
	};
}

#endif /* __MASTER_TASK_QUEUE_H__ */
