/*
 * optimizermanager.cc
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

#include "optimizermanager.hh"

#include <optimization/constants.hh>

using namespace std;
using namespace optimaster;
using namespace network;
using namespace optimization;
using namespace base;

/** \brief Default constructor.
 *
 * Constructor.
 *
 * Create a new optimizer manager. After construction, make sure to call
 * Set to set the host and port to listen on, and listen to start listening
 * for new connections.
 *
 */
OptimizerManager::OptimizerManager()
:
	d_server(Constants::MasterPort)
{
	d_server.onNewConnection().add(*this, &OptimizerManager::OnNewConnection);
}

/** \brief Destructor.
 *
 * Destructor.
 *
 */
OptimizerManager::~OptimizerManager()
{
	d_server.onNewConnection().remove(*this, &OptimizerManager::OnNewConnection);
	
	map<int, Optimizer>::iterator iter;
	
	for (iter = d_optimizers.begin(); iter != d_optimizers.end(); ++iter)
	{
		iter->second.Client().onClosed().remove(*this, &OptimizerManager::OnConnectionClosed);
	}
	
	d_optimizers.clear();
}

/** \brief Find optimizer given an id.
 * @param id optimizer id
 * @param optimizer optimizer return value
 *
 * Find a connected optimizer by id.
 *
 * @return: true if optimizer could be found, false otherwise
 *
 */
bool
OptimizerManager::Find(size_t     id, 
                       Optimizer &optimizer)
{
	map<int, Optimizer>::iterator found;
	
	found = d_optimizers.find(id);
	
	if (found != d_optimizers.end())
	{
		optimizer = found->second;
		return true;
	}
	else
	{
		return false;
	}
}

/** \brief Start listening for optimizer connections.
 *
 * Start listening for new optimizer connections.
 *
 */
void
OptimizerManager::Listen()
{
	d_server.listen();
}

void
OptimizerManager::OnConnectionClosed(int fd)
{
	Optimizer optimizer;
	size_t id = static_cast<size_t>(fd);

	if (Find(id, optimizer))
	{
		OnRemoved(optimizer);
		d_optimizers.erase(id);
	}
}

void
OptimizerManager::OnNewConnection(Client &client)
{
	client.onClosed().add(*this, &OptimizerManager::OnConnectionClosed);
	
	Optimizer optimizer(client);
	d_optimizers[optimizer.Id()] = optimizer;
	
	if (Debug::enabled(Debug::Domain::Master))
	{
		debug_master << "New optimizer connected: "
		             << optimizer.Id() << " ("
		             << client.address().host(true) << ":"
		             << client.address().port(true) << ")" << endl;
	}
	
	OnAdded(optimizer);
}

/** \brief Set host and port on which to listen for optimizer connections.
 * @param host the host to listen on
 * @param port the port to listen on
 *
 * Set the host and the port on which to listen for new optimizer connections.
 * \fn OptimizerManager::Set(std::string const &host, std::string const &port)
 */
void
OptimizerManager::Set(string const &host,
                      string const &port)
{
	d_server.set(host, port);
}
