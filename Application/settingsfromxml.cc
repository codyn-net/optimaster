#include "application.ih"

void Application::settingsFromXml(base::Properties &properties, xmlpp::Element *element) 
{
	xmlpp::Node::NodeList settings = element->get_children("setting");
	xmlpp::Node::NodeList::iterator iter;
	
	for (iter = settings.begin(); iter != settings.end(); ++iter)
	{
		xmlpp::Element *setting;
		
		try
		{
			setting = dynamic_cast<xmlpp::Element *>(*iter);
		}
		catch (bad_cast)
		{
			continue;
		}

		xmlpp::Attribute *attr = setting->get_attribute("name");
		
		if (!attr || !setting->has_child_text())
			continue;
		
		string name = attr->get_value();
		
		base::PropertyBase &property = properties.property(name);
		
		if (!property.serializable())
			continue;

		string value = setting->get_child_text()->get_content();
		
		try
		{
			property = value;
		}
		catch (exception e)
		{
			cerr << "Could not set setting " << name << " = '" << value << "', bad value" << endl;
		}
	}
}
