#ifndef __MASTER_MANAGERS_OPTIMIZER_H__
#define __MASTER_MANAGERS_OPTIMIZER_H__

#include <base/base.hh>
#include <optimization/Optimizer/optimizer.hh>

#include "Managers/Module/module.hh"
#include "Managers/Optimizer/Extension/extension.hh"

#include <string>

namespace master
{
namespace manager
{
	class Optimizer
	{
		base::Cloneable<optimization::Optimizer> d_optimizer;
		
		public:
			class Extension
			{
				base::Cloneable<optimization::Optimizer::Extension> d_extension;
		
				public:
					Extension();
					Extension(optimization::Optimizer::Module::Extension &info);

					optimization::Optimizer::Extension &extension();
			};
	
			Optimizer();
			Optimizer(optimization::Optimizer::Module &info);

			optimization::Optimizer &optimizer();
	};
	
	inline Optimizer::Extension::Extension()
	{
	}

	inline Optimizer::Extension::Extension(optimization::Optimizer::Module::Extension &info)
	{
		d_extension = info.constructor();
	}
		
	inline optimization::Optimizer::Extension &Optimizer::Extension::extension()
	{
		return d_extension;
	}
	
	inline Optimizer::Optimizer()
	{
	}

	inline Optimizer::Optimizer(optimization::Optimizer::Module &info)
	{
		d_optimizer = info.constructor();
	}
		
	inline optimization::Optimizer &Optimizer::optimizer()
	{
		return d_optimizer;
	}
};

namespace managers
{
	class Optimizer : public Module<manager::Optimizer, optimization::Optimizer::Module>
	{
		public:
			class Extension : public Module<manager::Optimizer::Extension, optimization::Optimizer::Module::Extension>
			{
				public:
					Extension();
					
					base::Cloneable<optimization::Optimizer::Extension> lookup(std::string const &name);
				protected:
					virtual std::string searchEnvironmentPath() const;
					virtual std::string searchDirectory() const;
			};
			
			/* Constructor/destructor */
			Optimizer();
			
			base::Cloneable<optimization::Optimizer> lookup(std::string const &name);
		protected:
			virtual std::string searchEnvironmentPath() const;
			virtual std::string searchDirectory() const;
		private:
	};
}
}

#endif /* __MASTER_MANAGERS__OPTIMIZER_H__ */
