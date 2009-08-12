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
