#ifndef __MASTER_MANAGERS_MODULE_H__
#define __MASTER_MANAGERS_MODULE_H__

#include "Scanner/scanner.hh"

#include <base/base.hh>
#include <os/os.hh>

#include <string>

namespace optimaster
{
namespace managers
{
	template <typename Base, typename Info>
	class Module : public base::Object, public Scanner
	{
		public:
			/* Constructor/destructor */
			Module();
		
			/* Public functions */
			bool create(std::string const &name, Base &base);
			void remove(Base &base);
			
			typedef Base base_type;
			typedef Info info_type;
		protected:
			virtual void addModule(std::string const &filename);
		private:
			struct Data : public base::Object::PrivateData
			{
				/* Instance data */
				struct Module
				{
					Info info;
					os::Module module;
				};
			
				std::map<std::string, Module> infos;
				std::vector<Base> bases;
			};

			Data *d_data;
	};

	template <typename Base, typename Info>
	Module<Base, Info>::Module()
	{
		d_data = new Data();
		addPrivateData(d_data);
	}
	
	template <typename Base, typename Info>
	bool Module<Base, Info>::create(std::string const &name, Base &base)
	{
		typename std::map<std::string, typename Module<Base, Info>::Data::Module>::iterator iter;
		iter = d_data->infos.find(name);
	
		if (iter == d_data->infos.end())
			return false;
	
		base = Base(iter->second.info);
		d_data->bases.push_back(base);

		return true;
	}
	
	template <typename Base, typename Info>
	void Module<Base, Info>::remove(Base &base)
	{
		typename std::vector<Base>::iterator iter;
	
		for (iter = d_data->bases.begin(); iter != d_data->bases.end(); ++iter)
		{
			if (*iter == base)
			{
				d_data->bases.erase(iter);
				break;
			}
		}
	}
	
	template <typename Base, typename Info>
	void Module<Base, Info>::addModule(std::string const &filename)
	{
		os::Module module(filename);
	
		debug_modules << "Adding module: " << filename << std::endl;
	
		if (!module)
		{
			debug_modules << "Could not open module `" << filename << "`: " << module.error() << std::endl;
			return;
		}	
	
		typename Info::Loader loader = module.lookup<typename Info::Loader>(Info::loader);
	
		if (!loader)
		{
			debug_modules << "Could not lookup symbol `" << Info::loader << "` in `" << filename << "`" << std::endl;
			return;
		}
	
		typename Data::Module mod;
		loader(&mod.info);
	
		if (mod.info.name == "")
		{
			debug_modules << "Invalid module name in `" << filename << "`" << std::endl;
			return;
		}

		if (d_data->infos.find(mod.info.name) != d_data->infos.end())
		{
			debug_modules << "Module `" << mod.info.name << "` is already registered" << std::endl;
			return;
		}
	
		if (!mod.info.constructor)
		{
			debug_modules << "Module constructor not available in `" << filename << "`" << std::endl;
			return;
		}
	
		mod.module = module;
		d_data->infos[mod.info.name] = mod;
	
		debug_modules << "Added module `" << mod.info.name << "` from `" << filename << "`" << std::endl;
	}
}
}

#endif /* __MASTER_MANAGERS_MODULE_H__ */
