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

#include <optimization/messages.hh>

namespace optimaster
{
	class Task : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			optimization::messages::task::Task task;

			size_t group;
			size_t failures;

			Glib::TimeVal started;
			double lastRunTime;
		};
		
		Data *d_data;
		
		public:
			Task();
			Task(size_t group, optimization::messages::task::Task const &task);

			size_t Id() const;
			size_t Group() const;

			void Failed();
			size_t Failures();

			void Begin();
			void End();

			double LastRunTime() const;

			optimization::messages::task::Task &Message();
			optimization::messages::task::Task const &Message() const;

			virtual Task *clone() const;
	};
}

#endif /* __OPTIMASTER_TASK_H__ */

