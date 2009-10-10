/*
 *  config.cc - This file is part of optimaster
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

#include "config.ih"

Config::Config()
{
	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	acceptAnonymous = true;
	maxTaskFailures = 5;
	discoveryAddress = s.str();
	
	string user;
	if (Environment::variable("USER", user))
	{
		discoveryNamespace = user;
	}
	
	s.str("");
	s << ":" << optimization::Constants::MasterPort;
	listenAddress = s.str();

	registerProperty("accept anonymous", acceptAnonymous);
	registerProperty("discovery namespace", discoveryNamespace);
	registerProperty("discovery address", discoveryAddress);
	registerProperty("listen address", listenAddress);
	registerProperty("max task failures", maxTaskFailures);
}
