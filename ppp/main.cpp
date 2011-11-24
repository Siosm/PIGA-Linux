#include "ppp.yacc.h"
#include "piga.h"
#include "ppp.h"

#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <fstream>
#include <string>
#include <map>


extern unsigned int linenum;

int verbose = 0;
int stats = 0;

struct sequence * s;
struct link * l;

unsigned int curr_seq = 0;
unsigned int curr_link = 0;

std::map< std::string, unsigned int > tclass_map;

extern "C" {
	int yyparse(void);
	int yylex(void);
}


/* Added because panther doesn't have liby.a installed. */
int yyerror (char *msg) {
	return fprintf (stderr, "YACC: line %d, %s\n", linenum, msg);
}


void help(void){
	printf("Piga Policy Parser and loader, alpha version\n");
	printf("Available options :\n");
	printf("\t -v : verbose output\n");
	printf("\t -s : statistics output\n");
	printf("\t -h : print this help\n");
}


int main(int argc, char ** argv)
{
	int res = 0;
	int opt;

	// Parameter parsing and stuff like that
	while ((opt = getopt(argc, argv, "vhs")) != -1){
		switch (opt){
		case 'v':
			verbose =  1;
			break;
		case 's':
			stats = 1;
			break;
		case 'h':
			help();
			exit(EXIT_SUCCESS);
		case '?':
			if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			abort ();
		}
	}

	// Core PIGA & SELinux checks (versions...)
	// We should check if SELinux is active, and if PIGA is available

	s = (struct sequence *) malloc(sizeof(struct sequence) * 400000);
	l = (struct link *) malloc(sizeof(struct link) * 10000000);

	if((s == NULL) || (l == NULL)) {
		std::cerr << "PPP: Can't allocate enough memory!" << std::endl;
		return -1;
	}

	s[0].link_offset = 0;
// 	piga_seq_init(s);

	std::cout << "[+] PPP: Beggining PIGA policy parsing and loading..." << std::endl;
	res = yyparse();

	if(res != 0) {
		std::cerr << "[+] PPP: Parsing failed!" << std::endl;
		return -1;
	}

	std::cout << "[+] PPP: Policy successfully parsed. Setting up in the kernel..." << std::endl;
	res = PPP::set_sequence(curr_seq, curr_link, s, l);

	if(res == 0) {
		std::cout << "[+] PPP: PIGA policy loadded :" << std::endl;
		std::cout << "[+] \t" << PPP::get_loaded() << " signatures have been loaded" << std::endl;
		std::cout << "[+] \t" << PPP::get_ignored() << " signatures have been ignored" << std::endl;
	} else {
		std::cerr << "[+] PPP: Failed to set up policy!" << std::endl;
		return -1;
	}

	if(stats) {
		std::map< std::string, unsigned int > map;
		std::map< std::string, unsigned int > sid_map;

		std::cout << "[+] Policy stats : '<name> (<uint>): <count>'" << std::endl;
		std::map< std::string, unsigned int>::const_iterator itr;
		map = PPP::get_stat_sid();
		sid_map = PPP::get_sid_map();
		std::cout << "[+] \tSID stats :" << std::endl;
		for (itr = map.begin(); itr != map.end(); itr++) {
			std::cout << "[+] \t\t"<< (*itr).first << " (" << sid_map[(*itr).first] << "): " << (*itr).second << std::endl;
		}

		map = PPP::get_stat_tclass();
		std::cout << "[+] \ttclass stats :" << std::endl;
		for (itr = map.begin(); itr != map.end(); itr++) {
			std::cout << "[+] \t\t"<< (*itr).first << " (" << tclass_map[(*itr).first] << "): " << (*itr).second << std::endl;
		}

		map = PPP::get_stat_requested();
		std::cout << "[+] \trequested stats :" << std::endl;
		for (itr = map.begin(); itr != map.end(); itr++) {
			std::cout << "[+] \t\t"<< (*itr).first << ": " << (*itr).second << std::endl;
		}
	}

	return res;
}
