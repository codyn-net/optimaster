/*
 *  dataondata.cc - This file is part of optimaster
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

#include "worker.ih"

bool Worker::Data::onData(os::FileDescriptor::DataArgs &args) 
{
	vector<task::Response> responses;
	vector<task::Response>::iterator iter;
	
	Messages::extract(args, responses);
	
	if (responses.begin() == responses.end())
	{
		return false;
	}
	
	// Take the last response
	response = responses[responses.size() - 1];
	
	if  (response.status() == task::Response::Challenge)
	{
		// We are being challenged
		string challenge = response.challenge();
		onChallenge(challenge);
		return false;
	}
	
	// Check status
	if (response.status() != task::Response::Success)
	{
		if (!response.has_failure())
		{
			setFailure(task::Response::Failure::Unknown);
		}
	}
	else if (task.task().id() != response.id())
	{
		// Response for the wrong task
		setFailure(task::Response::Failure::WrongRequest);
	}

	working = false;
	onResponse(response);

	return false;
}
