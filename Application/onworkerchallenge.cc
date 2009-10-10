/*
 *  onworkerchallenge.cc - This file is part of optimaster
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

void Application::onWorkerChallenge(string &challenge, Worker worker)
{
	// TODO: Do some calculation with the job token, and write the response
	/*string response = hashToken(worker.job(), challenge);

	optimization::messages::worker::Communication message;
	optimization::messages::worker::Token &token = *message.mutable_token();
	
	message.set_type(optimization::messages::worker::Communication::CommunicationToken);
	
	token.set_id(worker.solution().id());
	token.set_response(response);
	
	string serialized;
	
	if (optimization::Messages::create(message, serialized))
	{
		worker.write(serialized);
	}*/
}
