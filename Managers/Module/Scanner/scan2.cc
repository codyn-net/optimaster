#include "scanner.ih"

void Scanner::scan(string const &directory) 
{
	debug_modules << "Scanning for modules in `" << directory << "`" << endl;

	if (!FileSystem::directoryExists(directory))
	{
		debug_modules << "Not a valid directory `" << directory << "`" << endl;
		return;
	}
	
	DIR *d = opendir(directory.c_str());
	
	if (!d)
	{
		debug_modules << "Could not open directory `" << directory << "`: " << strerror(errno) << endl;
		return;
	}
	
	struct dirent *dent;
	
	while ((dent = readdir(d)))
	{
		String s(directory + "/" + dent->d_name);
		
		if (!FileSystem::fileExists(s))
		{
			continue;
		}
		
		if (!String(dent->d_name).startsWith(os::Module::prefix) || !s.endsWith(os::Module::suffix))
		{
			debug_modules << "File not validly formatted: " << s << endl;
		}
		else
		{
			addModule(s);
		}
	}
	
	closedir(d);	
}
