/*
 * communicator.cc
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

#include "communicator.hh"

using namespace std;
using namespace optimaster;
using namespace network;
using namespace optimization;
using namespace optimization::messages;
using namespace base;
using namespace base::signals;

/**
 * @class optimaster::Communicator
 * @brief Abstract class used for communication to worker and optimizer.
 * @see optimaster::Optimizer
 * @see optimaster::Worker
 */

/**
 * @class optimaster::Communicator::CommunicationArgs
 * @brief Communication signal arguments.
 */

/**
 * @brief Create CommunicationArgs object.
 * @param source the source object
 * @param communication the communication object
 *
 * Constructor.
 *
 * Creates a new CommunicationArgs object for use in the onCommunication signal.
 * \fn Communicator::CommunicationArgs::CommunicationArgs(Communicator &source, optimization::messages::task::Communication &communication)
 */
Communicator::CommunicationArgs::CommunicationArgs(Communicator        &source,
                                                   task::Communication &communication)
:
	Source(source),
	Communication(communication)
{
}

/**
 * @brief Default communicator constructor.
 *
 * Constructor.
 *
 * This creates an empty communicator object.
 *
 */
Communicator::Communicator()
{
}

/**
 * @brief Communicator shell constructor.
 * @param data the communicator data
 *
 * Constructor.
 *
 * Creates a communicator object shell around the communicator data.
 *
 */
Communicator::Communicator(Data *data)
:
	d_data(data)
{
	// Empty shell around data
}

Communicator::Data::~Data()
{
	client.onClosed().remove(*this, &Data::OnClosed);
	client.onData().remove(*this, &Data::OnData);
}

/**
 * @brief Set communicator data.
 * @param data the communicator data
 * @param client the communicator client
 *
 * Set the communicator data.
 *
 */
void
Communicator::Set(Data *data, network::Client const &client)
{
	d_data = data;
	d_data->client = client;
	
	d_data->id = static_cast<size_t>(client.fd());

	d_data->client.onData().add(*d_data, &Data::OnData);
	d_data->client.onClosed().add(*d_data, &Data::OnClosed);
}

/**
 * @brief Communicator id (const).
 *
 * Get the communicator id.
 *
 * @return: the communicator id
 *
 */
size_t
Communicator::Id() const
{
	return d_data->id;
}

/**
 * @brief Operator equal.
 * @param id the id to compare
 *
 * Operator equal comparing communicator ids.
 *
 * @return true if the id is the same, false otherwise
 *
 */
bool
Communicator::operator==(size_t id) const
{
	return d_data->id == id;
}

/**
 * @brief Operator equal.
 * @param other the communicator to compare with
 *
 * Operator equal comparing communicator ids.
 *
 * @return true if the two communicators are equal, false otherwise
 *
 */
bool
Communicator::operator==(Communicator const &other) const
{
	return d_data->id == other.d_data->id;
}

/**
 * @brief Communication received signal.
 * 
 * Get the signal object which is fired when a communication message has been
 * received.
 *
 * @return: the signal object
 *
 */
Signal<Communicator::CommunicationArgs> &
Communicator::OnCommunication()
{
	return d_data->onCommunication;
}

/**
 * @brief Connection closed signal.
 * 
 * Get the signal object which is fired when the connection
 * is closed.
 *
 * @return: the signal object
 *
 */
Signal<Communicator> &
Communicator::OnClosed()
{
	return d_data->onClosed;
}

/**
 * @brief Data received callback.
 * @param args The received data argument
 * 
 * Callback called when data has been received from the worker.
 *
 */
void
Communicator::Data::OnData(os::FileDescriptor::DataArgs &args)
{
	vector<task::Communication> communications;
	vector<task::Communication>::iterator iter;

	Messages::Extract(args, communications);
	Communicator shell(this);

	for (iter = communications.begin(); iter != communications.end(); ++iter)
	{
		CommunicationArgs args(shell, *iter);
		onCommunication(args);
	}
}

/**
 * @brief Connection closed callback.
 * @param fd The connection file descriptor
 * 
 * Callback called when the connection with the worker is closed.
 *
 */
void
Communicator::Data::OnClosed(int fd)
{
	Communicator shell(this);
	
	// Keep it ALIVE!
	ref();
	onClosed(shell);
	unref();
}

/**
 * @brief Get the client connection.
 *
 * The client connection.
 *
 * @return: the client connection
 *
 */
Client &
Communicator::Client()
{
	return d_data->client;
}

/**
 * @brief Get the client connection (const).
 *
 * The client connection.
 *
 * @return: the client connection
 *
 */
Client const &
Communicator::Client() const
{
	return d_data->client;
}
