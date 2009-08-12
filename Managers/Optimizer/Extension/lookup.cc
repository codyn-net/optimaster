#include "extension.ih"

base::Cloneable<optimization::Optimizer::Extension> Optimizer::Extension::lookup(string const &name) 
{
	manager::Optimizer::Extension info;
	
	if (create(name, info))
	{
		Cloneable<optimization::Optimizer::Extension> ret = info.extension();
		ret->setName(name);
		
		debug_master << "Found and created extension " << name << endl;
		
		return ret;
	}

	return Cloneable<optimization::Optimizer::Extension>();	
}
