/*
 *  hashtoken.cc - This file is part of optimaster
 *
 *  Copyright (C) 2009 - Jesse van den Kieboom
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#include "application.ih"

/*string Application::hashToken(Job const &job, string const &challenge)
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
		s << hex << setw(2) << setfill('0') << static_cast<unsigned>(ptr[i]);
	}
	
	gcry_md_close(hd);
	return s.str();
}*/
