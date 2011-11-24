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


unsigned int PPP::string_to_sid(std::string sid)
{
	unsigned int res;

	if(sid_map.find(sid) == sid_map.end()) {
		std::ofstream getsid_file_w("/proc/piga/getsid");
		if (getsid_file_w.is_open()) {
			getsid_file_w << sid << std::endl;
			getsid_file_w.close();
		} else {
			std::cerr << "PPP: Can't open file /proc/piga/getsid to write the sid " << sid << std::endl;
			set_invalid_context(true);
			++error_count;
			if(error_count > 10) {
				std::cerr << "[+] PPP: Too many errors. Exiting" << std::endl;
				exit(EXIT_FAILURE);
			}
			return -1;
		}

		std::string line;
		std::ifstream getsid_file_r("/proc/piga/getsid");
		if (getsid_file_r.is_open()) {
			getline (getsid_file_r, line);
// 			std::cout << "PPP: answer:" << line << std::endl;
			getsid_file_r.close();
		} else {
			std::cerr << "PPP: Can't open file /proc/piga/getsid to read the answer for " << sid << std::endl;
			set_invalid_context(true);
			++error_count;
			if(error_count > 10) {
				std::cerr << "[+] PPP: Too many errors. Exiting" << std::endl;
				exit(EXIT_FAILURE);
			}
			return -1;
		}

		res  = strtoul(line.c_str(), NULL, 10);
		sid_map[sid] = res;
		stat_sid_map[sid] = 1;
	} else {
		res = sid_map[sid];
		stat_sid_map[sid]++;
	}

	if(res == 0) {
		// FIXME Add a #define for this, and a runtime option if defined
		if(verbose) {
			std::cerr << "PPP: This sid is 0, so there is an error. Maybe the context you've used isn't available with the currently loaded policy: " << sid << std::endl;
		}
		set_invalid_context(true);
	}

	return res;
}


int PPP::set_sequence(unsigned int s_len, unsigned int l_len, struct sequence * s, struct link * l)
{
// 	unsigned int i = 0;
// 	printf("len: %d, ", s->length);
// 	for(i = 0; i < s->length; ++i) {
// 		printf("%d -(%d %d)-> %d ; ",
// 		       s->link_list[i].cs, s->link_list[i].tclass,
// 		       s->link_list[i].requested, s->link_list[i].cc);
// 	}
// 	printf("\n");
        return syscall(__NR_sys_piga_add_sequence, s_len, l_len, s, l);
// 	return 0;

// 	unsigned long p;
// 	std::ofstream load_s("/proc/piga/load_sequence");
// 	if (load_s.is_open()) {
// 		p = (unsigned long) s;
// 		load_s << p << std::endl;
// 		load_s.close();
// 	} else {
// 		std::cerr << "PPP: Can't open file /proc/piga/load_sequence to write the s pointer." << std::endl;
// 		return;
// 	}
// 	std::ofstream load_l("/proc/piga/load_link");
// 	if (load_l.is_open()) {
// 		p = (unsigned long) l;
// 		load_l << p << std::endl;
// 		load_l.close();
// 	} else {
// 		std::cerr << "PPP: Can't open file /proc/piga/load_link to write the l pointer." << std::endl;
// 		return;
// 	}
}
