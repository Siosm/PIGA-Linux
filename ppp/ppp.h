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


#ifndef PPP_H
#define PPP_H

#include <map>
#include <string>

#include "piga.h"

namespace PPP
{
	void set_invalid_context(bool b);
	bool get_invalid_context(void);

	unsigned int string_to_sid(std::string sid);

	int set_sequence(unsigned int s_len, unsigned int l_len, sequence * s, struct link * l);

	std::map< std::string, unsigned int > get_stat_sid(void);
	std::map< std::string, unsigned int > get_sid_map(void);
	std::map< std::string, unsigned int > get_stat_tclass(void);
	std::map< std::string, unsigned int > get_stat_requested(void);

	void stat_perm(std::string tclass, std::string requested);

	unsigned int get_ignored(void);
	void incr_ignored(void);
	void incr_loaded(void);

	unsigned int get_loaded(void);
}


#endif // PPP_H
