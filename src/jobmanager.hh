/*
 * optimizermanager.hh
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

#ifndef __OPTIMASTER_JOB_MANAGER_H__
#define __OPTIMASTER_JOB_MANAGER_H__

#include <jessevdk/network/network.hh>
#include "job.hh"

namespace optimaster
{
	class JobManager
	{
		std::map<int, Job> d_jobs;
		jessevdk::network::TcpServer d_server;

		public:
			/* Constructor/destructor */
			JobManager();
			~JobManager();

			/* Public functions */
			void Set(std::string const &host, std::string const &port);

			bool Listen();
			operator bool() const;

			bool Find(size_t id, Job &job);

			std::vector<Job> Jobs() const;

			/**
			* @brief OnAdded signal
			 *
			 * Signal emitted when job is added to the manager
			 */
			jessevdk::base::signals::Signal<Job> OnAdded;

			/**
			* @brief OnRemoved signal
			 *
			 * Signal emitted when job is removed from the manager
			 */
			jessevdk::base::signals::Signal<Job> OnRemoved;
		private:
			/* Private functions */
			void OnNewConnection(jessevdk::network::Client &client);
			void OnJobClosed(Communicator &communicator);
	};
}

#endif /* __OPTIMASTER_JOB_MANAGER_H__ */
