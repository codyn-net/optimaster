/*
 * optimizer.hh
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

#ifndef __OPTIMASTER_OPTIMIZER_H__
#define __OPTIMASTER_OPTIMIZER_H__

#include <network/network.hh>
#include <optimization/Messages/messages.hh>
#include <map>
#include <vector>

#include "worker.hh"

namespace optimaster
{
	class Optimizer : public Communicator
	{
		public:
			/* Constructor/destructor */
			Optimizer();
			Optimizer(network::Client &client);
			
			void Add(Worker &worker);
			std::vector<Worker> &ActiveWorkers();
		private:
			struct Data : public Communicator::Data
			{
				std::vector<Worker> activeWorkers;
				void OnWorkerDeactivated(Worker &worker);
			};

			Data *d_data;
	};
}

#endif /* __MASTER_OPTIMIZER_H__ */
