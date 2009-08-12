#include "application.ih"

void Application::parseBoundaries(optimization::Boundaries &boundaries, xmlpp::Element *child)
{
	xmlpp::Node::NodeList children = child->get_children("boundary");
	xmlpp::Node::NodeList::iterator iter;

	for (iter = children.begin(); iter != children.end(); ++iter)
	{
		xmlpp::Element *boundary;

		try
		{
			boundary = dynamic_cast<xmlpp::Element *>(*iter);
		}
		catch (bad_cast)
		{
			continue;
		}
	
		string name;

		double min;
		double max;
		xmlpp::Attribute *attr;

		if (!(attr = boundary->get_attribute("name")))
			continue;

		name = attr->get_value();
	
		if (name == "" || boundaries.includes(name))
			continue;
	
		if (!(attr = boundary->get_attribute("min")))
			continue;

		{
			stringstream s;
			s << attr->get_value();
		
			if (!(s >> min))
				continue;
		}
	
		if (!(attr = boundary->get_attribute("max")))
			continue;

		{
			stringstream s;
			s << attr->get_value();
		
			if (!(s >> max))
				continue;
		}
	
		if (min > max)
		{
			double tmp = min;
			min = max;
			max = tmp;
		}
	
		boundaries.add(name, min, max);
	}
}
