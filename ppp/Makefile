# for debug, use :
# LEX = /usr/bin/flex -d
# CFLAGS = -g -Wall -Wextra
# YACC = /usr/bin/yacc -d --verbose --debug

# for release, use :
LEX = /usr/bin/flex -f
CFLAGS = -O2 -Wall -Wextra -Wpacked -Wpacked
YACC = /usr/bin/yacc -d

CC = /usr/bin/gcc
CXX = /usr/bin/g++

LDLIBS = -lfl -lselinux

all: ppp

ppp: clean
	$(LEX) ppp.l
	mv -f lex.yy.c ppp.lex.c
	$(YACC) ppp.y
	mv -f y.tab.c ppp.yacc.c
	mv -f y.tab.h ppp.yacc.h
	$(CC) $(CFLAGS) -c ppp.lex.c -o ppp.lex.o $(LDLIBS)
	$(CXX) $(CFLAGS) -o ppp main.cpp ppp.lex.o ppp.yacc.c piga.c ppp.cpp $(LDLIBS)

clean:
	rm -f ppp ppp.lex.c ppp.lex.o ppp.yacc.c ppp.yacc.h ppp.yacc.o piga.o ppp.o y.output *~
