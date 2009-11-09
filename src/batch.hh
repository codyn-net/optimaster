/*
 * batch.hh
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

#ifndef __OPTIMASTER_BATCH_H__
#define __OPTIMASTER_BATCH_H__

#include <optimization/messages.hh>
#include "task.hh"

namespace optimaster
{
	class Batch : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			size_t id;
			std::deque<Task> tasks;
			
			double priority;
			double timeout;
			
			double waitTime;
			double bias;
		};
		
		Data *d_data;
		
		public:
			Batch();
			Batch(size_t idx, double bias, optimization::messages::task::Batch const &batch);

			size_t Id() const;

			double Priority() const;
			double Timeout() const;
			
			void Push(Task const &task);
			bool Pop(Task &task);
			
			bool Empty() const;

			bool operator==(size_t id) const;
			bool operator>(Batch const &other) const;

			void Wait();
			void WaitReset();
			double WaitTime() const;

			virtual Batch *clone() const;
	};
}

#endif /* __OPTIMASTER_BATCH_H__ */

