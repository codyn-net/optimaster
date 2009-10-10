/*
 *  config.hh - This file is part of optimaster
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
			bool acceptAnonymous;

			Glib::ustring discoveryNamespace;
			Glib::ustring discoveryAddress;
			Glib::ustring listenAddress;
			
			size_t maxTaskFailures;
			
			/* Constructor/destructor */
			static Config &initialize(std::string const &filename);
			static Config &instance();
		
			/* Public functions */
		private:
			/* Private functions */
			Config();
	};
}

#endif /* __OPTIMASTER_CONFIG_H__ */
