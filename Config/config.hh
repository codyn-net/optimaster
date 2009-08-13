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
			int commandPort;			
			int maxJobs;
			
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
