#ifndef __OPTIMASTER_DEBUG_H__
#define __OPTIMASTER_DEBUG_H__

#include <optimization/debug.hh>

#define debug_optimaster_out(domain) (base::Debug::out(optimaster::Debug::Domain::domain, #domain,  __FILE__, __FUNCTION__, __LINE__))

#define debug_scheduler (debug_optimaster_out(Scheduler))

namespace optimaster
{
	class Debug : public optimization::Debug
	{
		public:
			struct Domain
			{
				enum Values
				{
					Scheduler = optimization::Debug::Domain::User << 1,
				};
			};
	};
}

#endif /* __OPTIMASTER_DEBUG_H__ */

