/*
 * task.hh
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

#ifndef __OPTIMASTER_TASK_H__
#define __OPTIMASTER_TASK_H__

#include <optimization/Messages/messages.hh>

namespace optimaster
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

			size_t Id() const;
			double Priority() const;
			double Overtake() const;

			bool CanOvertake(Task const &other) const;
			void Overtake(Task &other);
			void SetOvertake(double overtake);

			void Sequence(Task &other);
			void Sequence();
			
			void Failed();
			size_t Failures();
			
			bool operator==(size_t id) const;
			
			optimization::messages::task::Task &Message();
			optimization::messages::task::Task const &Message() const;
			
			virtual Task *clone() const;
		private:
			void SequenceDecrease(double overtake);
	};
}

#endif /* __OPTIMASTER_TASK_H__ */

