#include "optimizer.ih"

base::Cloneable<optimization::Optimizer> Optimizer::lookup(string const &name)
{
	manager::Optimizer info;
	
	if (create(name, info))
	{
		Cloneable<optimization::Optimizer> ret = info.optimizer();
		ret->setName(name);
		
		return ret;
	}
	
	return Cloneable<optimization::Optimizer>();
}
