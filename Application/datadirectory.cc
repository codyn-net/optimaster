#include "application.ih"

string Application::dataDirectory() const
{
	string path = optimaster::Config::instance().dataDirectory;
	
	if (!FileSystem::isAbsolute(path))
	{
		path = FileSystem::workingDirectory() + "/" + path;
	}
	
	/* Try to make it all */
	FileSystem::mkdirs(path);

	return path;
}
