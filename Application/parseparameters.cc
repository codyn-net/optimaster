#include "application.ih"

void Application::parseParameters(optimization::Parameters &parameters, optimization::Boundaries &boundaries, xmlpp::Element *child) 
{
	xmlpp::Node::NodeList children = child->get_children("parameter");
	xmlpp::Node::NodeList::iterator iter;

	for (iter = children.begin(); iter != children.end(); ++iter)
	{
		xmlpp::Element *parameter;

		try
		{
			parameter = dynamic_cast<xmlpp::Element *>(*iter);
		}
		catch (bad_cast)
		{
			continue;
		}
	
		string name;
		xmlpp::Attribute *attr;

		if (!(attr = parameter->get_attribute("name")))
		{
			continue;
		}

		name = attr->get_value();
	
		if (name == "" || parameters.includes(name))
		{
			continue;
		}
		
		optimization::Parameters::value_type &param = parameters.add(name);
	
		if ((attr = parameter->get_attribute("boundary")))
		{
			string boundary = attr->get_value();
			
			if (boundaries.includes(boundary))
			{
				optimization::Boundaries::value_type &t = boundaries[boundary];
				
				param.boundary = t;
			}
		}
	}
}
