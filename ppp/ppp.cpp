/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Timothée Ravier timothee.romain.ravier@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "ppp.h"

#include <cstdlib>
#include <cstdio>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>

#ifdef __x86_64
#define __NR_sys_piga_add_sequence 307
#define __NR_sys_piga_get_sid 308
#endif /* __x86_64 */


extern int verbose;

int error_count = 0;


namespace PPP
{
	static bool invalid_context = false;

	std::map< std::string, unsigned int > sid_map;
	std::map< std::string, unsigned int > stat_sid_map;
	std::map< std::string, unsigned int > stat_tclass_map;
	std::map< std::string, unsigned int > stat_requested_map;
// 	std::map< std::string, unsigned int > stat_seq_map;

	unsigned int loaded = 0;
	unsigned int ignored = 0;
}


void PPP::set_invalid_context(bool b)
{
	invalid_context = b;
}


bool PPP::get_invalid_context(void)
{
	return invalid_context;
}


unsigned int PPP::get_ignored(void)
{
	return ignored;
}


unsigned int PPP::get_loaded(void)
{
	return loaded;
}


void PPP::incr_ignored(void)
{
	ignored++;
}


void PPP::incr_loaded(void)
{
	loaded++;
}


std::map< std::string, unsigned int > PPP::get_stat_sid(void)
{
	return stat_sid_map;
}


std::map< std::string, unsigned int > PPP::get_stat_tclass(void)
{
	return stat_tclass_map;
}


std::map< std::string, unsigned int > PPP::get_stat_requested(void)
{
	return stat_requested_map;
}


std::map< std::string, unsigned int > PPP::get_sid_map(void)
{
	return sid_map;
}


void PPP::stat_perm(std::string tclass, std::string requested)
{
	if(requested == ""){
		stat_tclass_map[tclass]++;
	} else {
		stat_requested_map[requested]++;
	}
}


unsigned int PPP::string_to_sid(std::string scontext)
{
	int ret;
	unsigned int sid;

	if(sid_map.find(scontext) == sid_map.end()) {
		ret = syscall(__NR_sys_piga_get_sid, scontext.c_str(), scontext.length(), &sid);
		if (ret != 0) {
			std::cerr << "PPP: Error in get_sid syscall, with scontext: " << scontext << std::endl;
			set_invalid_context(true);
			++error_count;
			if(error_count > 10) {
				std::cerr << "[+] PPP: Too many errors. Exiting" << std::endl;
				exit(EXIT_FAILURE);
			}
			return 0;
		}
		sid_map[scontext] = sid;
		stat_sid_map[scontext] = 1;
	} else {
		sid = sid_map[scontext];
		stat_sid_map[scontext]++;
	}

	if(sid == 0) {
		// FIXME Add a #define for this, and a runtime option if defined
		if(verbose) {
			std::cerr << "PPP: Error: This scontext corresponding sid is 0. Maybe the context used isn't available in the currently loaded SELinux policy: " << scontext << std::endl;
		}
		set_invalid_context(true);
	}

	return sid;
}


int PPP::set_sequence(unsigned int s_len, unsigned int l_len, struct sequence * s, struct link * l)
{
	unsigned int i = 0, j = 0, off = 0, link_list_len = 0;
// 	printf("len: %d, ", s->length);
// 	for(i = 0; i < s->length; ++i) {
// 		printf("%d -(%d %d)-> %d ; ",
// 		       s->link_list[i].cs, s->link_list[i].tclass,
// 		       s->link_list[i].requested, s->link_list[i].cc);
// 	}

	if (verbose) {
		for(i = 0; i < s_len; ++i) {
			printf("len: %u, pos: %u", s[i].length, s[i].current_position);
			off = s[i].link_offset;
			link_list_len = s[i].length;
			for(j = 0; j < link_list_len; ++j) {
				printf(" ; %u-(%u,%u)->%u",
				l[off + j].cs, l[off + j].tclass,
				l[off + j].requested, l[off + j].cc);
			}
			printf("\n");
		}
	}

        return syscall(__NR_sys_piga_add_sequence, s_len, l_len, s, l);
}
