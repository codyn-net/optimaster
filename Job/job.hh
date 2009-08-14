#ifndef __MASTER_JOB_H__
#define __MASTER_JOB_H__

#include <base/base.hh>

#include <optimization/optimization.hh>
#include <base/base.hh>
#include <math/math.hh>

#include "Dispatcher/dispatcher.hh"

#include <string>

namespace optimaster
{
	class Job : public base::Object
	{
		struct Priorities
		{
			enum Values
			{
				Min = 1,
				Max = 5
			};
		};
		
		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::string name;
			std::string user;
			std::string chain;

			base::Cloneable<optimization::Optimizer> optimizer;
			base::Cloneable<optimization::Fitness> fitness;

			optimization::Parameters parameters;
			optimization::Boundaries boundaries;
			Dispatcher dispatcher;
			
			size_t priority;
			double executionTime;
			
			std::string token;
			size_t failures;
			
			~Data();
		};

		Data *d_data;
		
		public:
			/* Constructor/destructor */
			Job();
			Job(std::string const &name, optimization::Optimizer &optimizer, optimization::Fitness &fitness, optimization::Parameters &parameters, optimization::Boundaries &boundaries, Dispatcher &dispatcher);
		
			/* Public functions */
			optimization::Optimizer &optimizer();
			optimization::Optimizer const &optimizer() const;
			
			optimization::Fitness &fitness();
			optimization::Fitness const &fitness() const;
			
			optimization::Boundaries &boundaries();
			optimization::Boundaries const &boundaries() const;

			Dispatcher &dispatcher();
			Dispatcher const &dispatcher() const;
			
			std::string const &name() const;
			
			std::string const &token() const;
			void setToken(std::string const &token);
			
			double priority() const;
			void setPriority(size_t priority);
			
			void setUser(std::string const &user);
			std::string const &user() const;
			
			void setChain(std::string const &chain);
			std::string const &chain() const;
			
			void failed();
			void failed(size_t reset);
			size_t failures() const;
			
			void initializeDatabase();
		private:
			/* Private functions */	
	};
	
	inline optimization::Optimizer &Job::optimizer()
	{
		return d_data->optimizer;
	}
	
	inline optimization::Optimizer const &Job::optimizer() const
	{
		return d_data->optimizer;
	}

	inline optimization::Fitness &Job::fitness()
	{
		return d_data->fitness;
	}
	
	inline optimization::Fitness const &Job::fitness() const
	{
		return d_data->fitness;
	}
	
	inline optimization::Boundaries &Job::boundaries()
	{
		return d_data->boundaries;
	}
	
	inline optimization::Boundaries const &Job::boundaries() const
	{
		return d_data->boundaries;
	}
	
	inline Dispatcher &Job::dispatcher()
	{
		return d_data->dispatcher;
	}
	
	inline Dispatcher const &Job::dispatcher() const
	{
		return d_data->dispatcher;
	}
	
	inline std::string const &Job::name() const
	{
		return d_data->name;
	}
	
	inline double Job::priority() const
	{
		return d_data->priority / d_data->executionTime;
	}

	inline void Job::setPriority(size_t priority)
	{
		d_data->priority = math::min(math::max(priority, (size_t)Priorities::Min), (size_t)Priorities::Max);
	}
	
	inline std::string const &Job::token() const
	{
		return d_data->token;
	}
	
	inline void Job::setToken(std::string const &token)
	{
		d_data->token = token;
	}
	
	inline std::string const &Job::user() const
	{
		return d_data->user;
	}
	
	inline void Job::setUser(std::string const &user)
	{
		d_data->user = user;
	}
	
	inline std::string const &Job::chain() const
	{
		return d_data->chain;
	}
	
	inline void Job::setChain(std::string const &chain)
	{
		d_data->chain = chain;
	}
	
	inline void Job::failed()
	{
		++d_data->failures;
	}
	
	inline void Job::failed(size_t reset)
	{
		d_data->failures = reset;
	}
	
	inline size_t Job::failures() const
	{
		return d_data->failures;
	}
}


#endif /* __MASTER_JOB_H__ */

