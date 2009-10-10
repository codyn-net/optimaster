/*
 *  task.hh - This file is part of optimaster
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

#ifndef __MASTER_TASK_QUEUE_TASK_H__
#define __MASTER_TAKS_QUEUE_TASK_H__

#include <optimization/Messages/messages.hh>

namespace optimaster
{
namespace taskqueue
{
	class Task : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			optimization::messages::task::Task task;
			base::Cloneable<Task> nextInSequence;

			size_t id;
			double priority;
			double overtake;
			
			size_t failures;
		};
		
		Data *d_data;
		
		public:
			Task();
			Task(size_t idx, double priority, optimization::messages::task::Task const &task);

			size_t id() const;
			double priority() const;
			double overtake() const;

			bool canOvertake(Task const &other) const;
			void overtake(Task &other);
			void setOvertake(double overtake);

			void sequence(Task &other);
			void sequence();
			
			void failed();
			size_t failures();
			
			bool operator==(size_t id) const;
			
			optimization::messages::task::Task &task();
			optimization::messages::task::Task const &task() const;
			
			
			Task *clone() const;
		private:
			void sequenceDecrease(double overtake);
	};
	
	inline optimization::messages::task::Task &Task::task()
	{
		return d_data->task;
	}
	
	inline optimization::messages::task::Task const &Task::task() const
	{
		return d_data->task;
	}
	
	inline double Task::overtake() const
	{
		return d_data->overtake;
	}
	
	inline void Task::setOvertake(double overtake)
	{
		d_data->overtake = overtake;
	}
	
	inline size_t Task::id() const
	{
		return d_data->id;
	}
	
	inline double Task::priority() const
	{
		return d_data->priority;
	}
}
}

#endif /* __MASTER_TASK_QUEUE_TASK_H__ */

