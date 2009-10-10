/*
 *  onnewconnection.cc - This file is part of optimaster
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

#include "optimizermanager.ih"

void OptimizerManager::onNewConnection(Client &client)
{
	client.onClosed().add(*this, &OptimizerManager::onConnectionClosed);
	
	Optimizer optimizer(client);
	
	optimizer.onBatch().addData(*this, &OptimizerManager::onOptimizerBatch, client.fd(), 0);
	d_optimizers[client.fd()] = optimizer;
	
	if (Debug::enabled(Debug::Domain::Master))
	{
		debug_master << "New optimizer connected: " << optimizer.id() << " (" << client.address().host(true) << ":" << client.address().port(true) << ")" << endl;
	}
}
