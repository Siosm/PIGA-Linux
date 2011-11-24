#include "piga.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern unsigned int curr_seq;
extern unsigned int curr_link;


void piga_seq_print(struct sequence * s, struct link * l)
{
	unsigned int i = 0;
	printf("len: %d, ", s[curr_seq].length);
	for(i = 0; i < s[curr_seq].length; ++i) {
		printf("%d -(%d %d)-> %d ; ",
		       l[s[curr_seq].link_offset + i].cs, l[s[curr_seq].link_offset + i].tclass,
		       l[s[curr_seq].link_offset + i].requested, l[s[curr_seq].link_offset + i].cc);
	}
	printf("\n");
}


void piga_seq_add_link(struct sequence * s, struct link * l)
{
	s[curr_seq].length++;
	curr_link++;
// 	++(s->length);
// 	if(s->link_list != NULL) {
// 		struct link * tmp = (struct link *) malloc(sizeof(struct link) * s->length);
// 		memcpy(tmp, s->link_list, sizeof(struct link) * (s->length - 1));
// 		free(s->link_list);
// 		s->link_list = tmp;
// 	} else {
// 		s->link_list = (struct link *) malloc(sizeof(struct link));
// 		s->length = 1;
// 	}
// 	s->link_list[s->length - 1] = *l;
}


void piga_seq_init(struct sequence * s)
{
// 	s->link_list = NULL;
// 	s->length = 0;
// 	s->current_position = 0;
// 	s->seq_string = NULL;
}


void piga_seq_clear(struct sequence * s)
{
// 	free(s->link_list);
// 	free(s->seq_string);
// 	s->link_list = NULL;
// 	s->current_position = 0;
// 	s->length = 0;
// 	s->seq_string = NULL;
}


void piga_link_clear(struct link * l)
{
// 	l->cs = 0;
// 	l->cc = 0;
// 	l->tclass = 0;
// 	l->requested = 0;
}
