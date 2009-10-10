/*
 *  enableenvironment.cc - This file is part of optimaster
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

struct DebugInfo
{
	char const *name;
	int flag;
};

void Application::enableEnvironment() 
{
	static DebugInfo infos[] = {
		{"DEBUG_BASE", base::Debug::Domain::Base},
		{"DEBUG_NETWORK", base::Debug::Domain::Network},
		{"DEBUG_DATA", base::Debug::Domain::Data},
		{"DEBUG_OS", base::Debug::Domain::OS},
		{"DEBUG_OPTIMIZATION", base::Debug::Domain::Optimization},
		{"DEBUG_MODULES", base::Debug::Domain::Modules},
		{"DEBUG_WORKER", base::Debug::Domain::Worker},	
		{"DEBUG_MASTER", base::Debug::Domain::Master},
	};
	
	for (size_t i = 0; i < sizeof(infos) / sizeof(DebugInfo); ++i)
	{
		if (Environment::variable(infos[i].name))
			base::Debug::enable(infos[i].flag);
	}
}
