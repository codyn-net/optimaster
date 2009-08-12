#ifndef __MASTER_DISPATCHER_H__
#define __MASTER_DISPATCHER_H__

#include <optimization/optimization.hh>

namespace master
{
	class Dispatcher : public optimization::Settings
	{
		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::string name;
		};

		Data *d_data;

		public:
			/* Constructor/destructor */
			Dispatcher(std::string const &name = "");
		
			std::string const &name() const;
			void setName(std::string const &name);
		private:
			/* Private functions */
	};
	
	inline std::string const &Dispatcher::name() const
	{
		return d_data->name;
	}
	
	inline void Dispatcher::setName(std::string const &name)
	{
		d_data->name = name;
	}
}

#endif /* __MASTER_DISPATCHER_H__ */
