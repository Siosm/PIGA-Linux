#ifndef __PIGA_H__
#define __PIGA_H__

#ifdef __cplusplus
extern "C" {
#endif


struct sequence {
	unsigned int length;
	unsigned int current_position;
	unsigned int link_offset;
// 	struct link * link_list;
	char * seq_string;
};


struct link {
	unsigned int cs;
	unsigned int cc;
	unsigned short tclass;
	unsigned int requested;
};


void piga_seq_print(struct sequence * s, struct link * l);

void piga_seq_add_link(struct sequence * s, struct link * l);

void piga_seq_init(struct sequence * s);

void piga_seq_clear(struct sequence * s);

void piga_link_clear(struct link * l);

unsigned int string_to_sid(char * sid);


#ifdef __cplusplus
}
#endif

#endif /* __PIGA_H__ */
