#include "command.ih"

command::Response Command::Data::createResponse(command::CommandType type, bool status, string const &data) const
{
	command::Response ret;
	
	ret.set_type(type);
	ret.set_status(status);
	ret.set_message(data);
	
	return ret;
}
