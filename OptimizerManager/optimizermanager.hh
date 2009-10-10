/*
 *  optimizermanager.hh - This file is part of optimaster
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

#ifndef __MASTER_OPTIMIZER_MANAGER_H__
#define __MASTER_OPTIMIZER_MANAGER_H__

#include <network/network.hh>
#include "Optimizer/optimizer.hh"

namespace optimaster
{
	class OptimizerManager
	{
		std::map<int, Optimizer> d_optimizers;
		network::TcpServer d_server;

		public:
			struct BatchArgs
			{
				Optimizer optimizer;
				optimization::messages::task::Batch batch;
			};

			/* Constructor/destructor */
			OptimizerManager();
			~OptimizerManager();
		
			/* Public functions */
			void set(std::string const &host, std::string const &port);
			void listen();
			
			bool find(size_t id, Optimizer &optimizer);
			
			base::signals::Signal<BatchArgs> onBatch;
			base::signals::Signal<Optimizer> onOptimizerClosed;
		private:
			/* Private functions */
			void onNewConnection(network::Client &client);
			void onConnectionClosed(int fd);
			
			bool fromDescriptor(int fd, Optimizer &optimizer);
			bool onOptimizerBatch(optimization::messages::task::Batch &batch, int fd);
	};
}

#endif /* __MASTER_OPTIMIZER_MANAGER_H__ */
