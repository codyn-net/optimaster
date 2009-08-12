#include "application.ih"

string Application::dataDirectory() const
{
	string path;
	
	if (!Environment::variable("OPTIMIZATION_DATA_PATH", path))
	{
		path = "data";
	}
	
	if (!FileSystem::isAbsolute(path))
	{
		path = FileSystem::workingDirectory() + "/" + path;
	}
	
	/* Try to make it all */
	
	FileSystem::mkdirs(path);
	return path;
}
