/*
 * config.hh
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

#ifndef __OPTIMASTER_CONFIG_H__
#define __OPTIMASTER_CONFIG_H__

#include <base/Config/config.hh>
#include <glibmm.h>

namespace optimaster
{
	class Config : public base::Config
	{
		static Config *s_instance;

		public:
			/** Discovery namespace setting. */
			Glib::ustring DiscoveryNamespace;
			
			/** Discovery address setting. */
			Glib::ustring DiscoveryAddress;
			
			/** Address on which to listen. */
			Glib::ustring ListenAddress;
			
			/** Maximum number of allowed task failures. */
			size_t MaxTaskFailures;
			
			/** Number of runs to measure run time estimation */
			size_t RunTimeEstimation;
			
			/* Constructor/destructor */
			static Config &Initialize(std::string const &filename);
			static Config &Instance();
		
			/* Public functions */
		private:
			/* Private functions */
			Config();
	};
}

#endif /* __OPTIMASTER_CONFIG_H__ */
