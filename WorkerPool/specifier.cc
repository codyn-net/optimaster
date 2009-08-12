#include "workerpool.ih"

string WorkerPool::specifier(string const &protocol, string const &host, string const &port) const
{
	return protocol + "://" + host + ":" + port;
}
