/*
 *  sendwakeup.cc - This file is part of optimaster
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

#include "application.ih"

void Application::sendWakeup()
{
	// Create new multicast client
	Client client = Client::Multicast(optimization::Constants::DiscoveryGroup, optimization::Constants::DiscoveryPort, 2);

	// Make wakeup discovery message with certain namespace
	optimization::messages::discovery::Discovery disc;
	
	disc.set_type(optimization::messages::discovery::Discovery::TypeWakeup);
	disc.set_namespace_(d_discovery.ns());

	optimization::messages::discovery::Wakeup *wakeup = disc.mutable_wakeup();

	// Set the connection
	wakeup->set_connection(d_discovery.connection());
	string serialized;
	
	if (optimization::Messages::create(disc, serialized))
	{
		client.write(serialized);
	}
}
