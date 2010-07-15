/*
 * config.cc
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

#include "config.hh"

#include <optimization/constants.hh>
#include <jessevdk/os/os.hh>

using namespace std;
using namespace optimaster;
using namespace jessevdk::os;

Config *Config::s_instance = 0;
size_t Config::ProtocolVersion = 2;

/**
 * @class optimaster::Config
 * @brief Config class
 */

/**
 * @brief Default constructor.
 *
 * Constructor.
 *
 * Default constructor.
 *
 */
Config::Config()
{
	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	MaxTaskFailures = 5;
	DiscoveryAddress = s.str();

	string user;
	if (Environment::Variable("USER", user))
	{
		DiscoveryNamespace = user;
	}

	s.str("");
	s << ":" << optimization::Constants::MasterPort;
	ListenAddress = s.str();

	s.str("");
	s << ":" << optimization::Constants::CommandPort;
	CommandAddress = s.str();

	RunTimeEstimation = 10;
	LogInterval = 15;

	Register("discovery namespace", DiscoveryNamespace);
	Register("discovery address", DiscoveryAddress);
	Register("listen address", ListenAddress);
	Register("max task failures", MaxTaskFailures);
	Register("runtime estimation", RunTimeEstimation);
	Register("command password", CommandPassword);
	Register("log storage", LogStorage);
	Register("log interval", LogInterval);
	Register("command address", CommandAddress);
}

/**
 * @brief Initialize config singleton.
 * @param filename file to load configuration from
 *
 * Initialize the configuration singleton by loading configuration from a file.
 *
 * @return the configuration object
 * \fn Config &Config::Initialize(std::string const &filename)
 */
Config &
Config::Initialize(string const &filename)
{
	if (!s_instance)
	{
		s_instance = new Config();
	}

	s_instance->Read(filename);
	return *s_instance;
}

/**
 * @brief Get config object instance.
 *
 * Get the config object instance. Make sure the config object is initialized.
 *
 * @return: the configuration object
 *
 */
Config &
Config::Instance()
{
	return *s_instance;
}
