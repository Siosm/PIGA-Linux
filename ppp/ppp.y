%{
#include <stdio.h>

#include <selinux/selinux.h>

#include <vector>

#include "piga.h"
#include "ppp.h"

extern unsigned int linenum;
extern int verbose;

extern struct sequence * s;
extern struct link * l;

extern unsigned int curr_seq;
extern unsigned int curr_link;

extern std::map< std::string, unsigned int > tclass_map;

std::vector< std::string > request_list;

extern "C" {
	int yyparse(void);
	int yylex(void);
	int yywrap()
	{
		return 1;
	}
}

extern int yyerror (char *);
%}

%union
{
	unsigned int sid;
	char * str;
}

%token <str> TYPE
%token <str> BOOL
%token <str> SID
%token EOL ENDF AVPERMSTART AVPERMEND
%token <str> WORD

%%

wholefile:	file ENDF { return 0; }
	|	file EOL ENDF { return 0; }
;

file:		line { ; }
	|	file EOL line { ; }
;

line:		TYPE '-' BOOL ':' signature {
/* 	printf("YACC: line: %s,%s,%s\n", $<str>1, $<str>3, $<str>5); */
	if(PPP::get_invalid_context() == true) {
		if(verbose)
			fprintf (stderr, "PPP: Invalid context. Skipping\n");
		PPP::incr_ignored();
		PPP::set_invalid_context(false);
	} else {
		if(verbose) {
			piga_seq_print(s, l);
		}
		PPP::incr_loaded();
/* 		if((PPP::set_sequence(s) != 0) && verbose) { */
/* 			fprintf (stdderr, "PPP: Can't give this one to the kernel. Skipping\n"); */
/* 		} */
	}
	// TODO Deal with this stuff!
/* 	piga_seq_clear(s); */
	curr_link += s[curr_seq].length;
	curr_seq++;
	s[curr_seq].link_offset = curr_link;
}
	|	BOOL ':' signature {
/* 	printf("YACC: line: %s,%s\n", $<str>1, $<str>3); */
	if(PPP::get_invalid_context() == true) {
		if(verbose)
			fprintf (stderr, "PPP: Invalid context. Skipping\n");
		PPP::incr_ignored();
		PPP::set_invalid_context(false);
	} else {
		if(verbose) {
			piga_seq_print(s, l);
		}
		PPP::incr_loaded();
/* 		if((PPP::set_sequence(s) != 0) && verbose) { */
/* 			fprintf (stdderr, "PPP: Can't give this one to the kernel. Skipping\n"); */
/* 		} */
	}
	// TODO Deal with this stuff!
/* 	piga_seq_clear(s); */
	curr_link += s[curr_seq].length;
	curr_seq++;
	s[curr_seq].link_offset = curr_link;
};

signature:	transition { ;
/* 	printf("YACC: trans: %s\n", $<str>1); */
}
	|	signature ';' transition { ;
/* 	printf("YACC: trans: %s, sign: %s\n", $<str>1, $<str>2); */
};

transition:	SID AVPERMSTART vector AVPERMEND SID {
/* 	printf("YACC: sid: %s, vec: %s, sid: %s\n", $<str>1, $<str>3, $<str>5); */
	l[curr_link].cs = PPP::string_to_sid($<str>1);
	l[curr_link].cc = PPP::string_to_sid($<str>5);
	piga_seq_add_link(s, l);
/* 	piga_link_clear(l); */
};

vector:		WORD '{' request '}' {
/* 	printf("YACC: vector: %s: %s\n", $<str>1, $<str>3); */
	l[curr_link].tclass = string_to_security_class($<str>1);
	tclass_map[$<str>1] = l[curr_link].tclass;

	// Do the stuff with the temporary request list
	l[curr_link].requested = 0;
	std::vector< std::string >::const_iterator it;
	for(it = request_list.begin(); it < request_list.end(); ++it) {
/* 		printf("YACC: adding request: %s\n", (*it).c_str()); */
		l[curr_link].requested |= string_to_av_perm(l[curr_link].tclass, (*it).c_str());
		PPP::stat_perm($<str>1, *it);
	}
	PPP::stat_perm($<str>1, "");
	request_list.clear();
};
//string_to_security_class(name) string_to_av_perm(tclass, name)

request:	WORD {
/* 	printf("YACC: request: %s\n", $<str>1); */
	request_list.push_back($<str>1);
}
	|	request WORD {
/* 	printf("YACC: request: %s\n", $<str>1); */
	request_list.push_back($<str>1);
};

%%

/* request:	REQ */
