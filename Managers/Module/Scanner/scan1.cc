#include "scanner.ih"

void Scanner::scan() 
{
	vector<string> paths = Environment::path(searchEnvironmentPath());
	
	for (vector<string>::iterator iter = paths.begin(); iter != paths.end(); ++iter)
	{
		scan(*iter);
	}
	
	string path;

	if (FileSystem::realpath(searchDirectory(), path))
		scan(path);
	else
		debug_modules << "Search path not found: " << searchDirectory() << endl;
}
