/*
 *  optimizer.hh - This file is part of optimaster
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

#ifndef __MASTER_OPTIMIZER_H__
#define __MASTER_OPTIMIZER_H__

#include <network/network.hh>
#include <optimization/Messages/messages.hh>

namespace optimaster
{
	class Optimizer : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			network::Client client;
			
			size_t id;

			base::signals::Signal<optimization::messages::task::Batch> onBatch;
			void onClientData(os::FileDescriptor::DataArgs args);
		};
		
		Data *d_data;
		
		public:
			/* Constructor/destructor */
			Optimizer();
			Optimizer(network::Client &client);
			
			network::Client &client();
		
			/* Public functions */
			size_t id();
			
			bool operator==(size_t id) const;
			void response(optimization::messages::task::Response &response);
			
			base::signals::Signal<optimization::messages::task::Batch> &onBatch();
		private:
			/* Private functions */
	};
}

#endif /* __MASTER_OPTIMIZER_H__ */
