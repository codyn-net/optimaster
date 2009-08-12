#include "application.ih"

bool Application::getFirstChild(xmlpp::Node *node, string const &name, xmlpp::Element *&child) 
{
	xmlpp::Node::NodeList children = node->get_children(name);
	
	if (children.size() == 0)
		return false;
	
	xmlpp::Node *n = children.front();
	
	try
	{
		child = dynamic_cast<xmlpp::Element *>(n);
	}
	catch (std::bad_cast &e)
	{
		return false;
	}
	return true;	
}
