/*
 * worker.hh
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

#ifndef __OPTIMASTER_WORKER_H__
#define __OPTIMASTER_WORKER_H__

#include <jessevdk/network/network.hh>
#include <optimization/optimization.hh>

#include "communicator.hh"
#include "taskqueue.hh"

namespace optimaster
{
	class Worker : public Communicator
	{
		public:
			/* Constructor/destructor */
			Worker();
			Worker(jessevdk::network::AddressInfo &info);

			/* Public functions */
			bool Activate(Task &task, double timeout);
			void Deactivate();

			bool Cancel();
			bool Active() const;
			
			Task const &ActiveTask() const;
			Task &ActiveTask();

			std::map<size_t, double> const &TotalActiveTime() const;
			void ResetTotalActiveTime();

			jessevdk::base::signals::Signal<Worker> &OnActivated();
			jessevdk::base::signals::Signal<Worker> &OnDeactivated();
			jessevdk::base::signals::Signal<Worker> &OnTimeout();

			Glib::Timer &IdleTime();
		private:
			/* Private functions */
			struct Data : public Communicator::Data
			{
				/* Instance data */
				bool active;

				jessevdk::base::signals::Signal<Worker> onActivated;
				jessevdk::base::signals::Signal<Worker> onDeactivated;
				jessevdk::base::signals::Signal<Worker> onTimeout;

				Task task;
				Glib::Timer idleTime;

				std::map<size_t, double> totalActiveTime;

				sigc::connection timeout;
				bool OnTimeout();
			};

			Worker(Data *data);
			Data *d_data;
	};
}

#endif /* __OPTIMASTER_WORKER_H__ */
