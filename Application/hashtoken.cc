#include "application.ih"

string Application::hashToken(Job const &job, string const &challenge)
{
	string const &token = job.token();
	
	if (token == "")
	{
		return "";
	}
	
	gcry_md_hd_t hd;
	
	if (gcry_md_open(&hd, GCRY_MD_SHA1, 0) != GPG_ERR_NO_ERROR)
	{
		return "";
	}
	
	gcry_md_write(hd, token.c_str(), token.size());
	gcry_md_write(hd, challenge.c_str(), challenge.size());
	
	uint8_t *ptr = gcry_md_read(hd, GCRY_MD_SHA1);
	
	if (!ptr)
	{
		gcry_md_close(hd);
		return "";
	}
	
	stringstream s;
	
	for (size_t i = 0; i < gcry_md_get_algo_dlen(GCRY_MD_SHA1); ++i)
	{
		s << hex << ptr[i];
	}
	
	gcry_md_close(hd);
	return s.str();
}
