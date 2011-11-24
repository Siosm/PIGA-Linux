#ifndef __PIGA_H__
#define __PIGA_H__

#include <linux/capability.h>
#include <linux/spinlock.h>
#include <linux/lsm_audit.h>
#include <linux/list.h>

#include "procfs.h"

#include "../../include/avc.h"

struct sequence {
	unsigned int length;
	unsigned int current_position;
	unsigned int link_offset;
	char * seq_string;

// 	struct list_head list;
};

struct link {
	u32 cs;
	u32 cc;
	u16 tclass;
	u32 requested;

// 	struct list_head list;
	// sequence list ?
};

/**
 * Sequence internals related functions
 * Current position related informations
**/
u32 piga_seq_get_cs(struct sequence *);
u32 piga_seq_get_cc(struct sequence *);
u16 piga_seq_get_tclass(struct sequence *);
u32 piga_seq_get_requested(struct sequence *);

/**
 * Informations about the sequence itself
 * returns true if this sequence can't go further
**/
bool piga_seq_end(struct sequence *);

/**
 * Action on the sequence : moves the token
 * returns true if this sequence has moved
**/
bool piga_seq_next(struct sequence *);

/** To get all sequences related to an action **/
struct sequence * piga_get_sequence_at(u32, u32, u16);

/** To add/delete/move sequences **/
// int piga_add_sequence(struct sequence * s);
// int piga_set_link(struct link * l);
int piga_set_pol(unsigned int s_l, unsigned int l_l, struct sequence * s, struct link * l);
unsigned int piga_get_length(void);
bool piga_remove_sequence(char*);
bool piga_move_sequence(u32, u32, u16, u32, u32, u16);

/** Compare two sequences: -x if 1 < 2 ; 0 if 1 == 2 ; +x if 1 > 2 **/
int piga_compare(u16 tclass1, u32 requested1, u16 tclass2, u32 requested2);

/** Easily display sequences **/
void piga_list_sequence(void);


bool * piga_status(void);
bool * piga_audit_only_mode(void);

/** Initialize PIGA related stuff **/
int init_piga(void);

/** Main PIGA function called after SELinux decision **/
int piga_has_perm(u32 ssid, u32 tsid, u16 tclass, u32 requested, struct common_audit_data *auditdata, int rc, struct av_decision * avd);

#endif /* __PIGA_H__ */
