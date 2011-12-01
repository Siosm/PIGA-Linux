#include "include/piga.h"

#include <linux/string.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>

#include "../av_permissions.h"
#include "../flask.h"
#include "../include/avc_ss.h"
#include "../include/security.h"

#define AVC_BUFFER_SIZE 64

#define PIGA_ALLOW 0
#define PIGA_DENY -EACCES


// static int MAX_SID = 1000; // FIXME : 65536 ?

// static struct link *** links; // link[tclass][ssid][tsid]
// static struct list_head ** sequences; // (struct listhead *)[ssid]

// Sequence are stored here until I find a satisfying way to store them
// static struct sequence ** sequences = NULL;
// static unsigned int sequences_size = 0;
// static unsigned int sequence_max = 0;


static unsigned int s_len = 0;
static unsigned int l_len = 0;
static struct sequence * seq = NULL;
static struct link * link = NULL;


static int piga_status_enabled = 0;
static int piga_audit_only_mode = 0;


u32 piga_seq_get_cs(struct sequence * s)
{
	if (s != NULL) {
		return link[s->link_offset + s->current_position].cs;
	}
	printk(KERN_ERR "PIGA: piga_seq_get_cs: s is NULL");

	return 0;
}


u32 piga_seq_get_cc(struct sequence * s)
{
	if (s != NULL) {
		return link[s->link_offset + s->current_position].cc;
	}
	printk(KERN_ERR "PIGA: piga_seq_get_cc: s is NULL");

	return 0;
}


u16 piga_seq_get_tclass(struct sequence * s)
{
	if (s != NULL) {
		return link[s->link_offset + s->current_position].tclass;
	}
	printk(KERN_ERR "PIGA: piga_seq_get_tclass: s is NULL");

	return 0;
}


u32 piga_seq_get_requested(struct sequence * s)
{
	if (s != NULL) {
		return link[s->link_offset + s->current_position].requested;
	}
	printk(KERN_ERR "PIGA: piga_seq_get_requested: s is NULL");

	return 0;
}


bool piga_seq_end(struct sequence * s)
{
	if (s != NULL) {
		return s->current_position + 1 == s->length;
	}
	printk(KERN_ERR "PIGA: piga_seq_end: s is NULL");

	return 0;
}


// TODO Fix locks, concurrency ...
bool piga_seq_next(struct sequence * s)
{
	printk(KERN_ERR "PIGA: piga_seq_next: called!");
	if (s != NULL) {
		s->current_position++;
		if (unlikely(s->current_position == s->length)) {
			s->current_position--;
			return false;
		}
		return true;
	}
	printk(KERN_ERR "PIGA: piga_seq_next: s is NULL");

	return false;
}


struct sequence * piga_get_sequence_at(u32 ssid, u32 tsid, u16 tclass)
{
	return seq;
}


bool piga_move_sequence(u32 ssid, u32 tsid, u16 tclass, u32 ssid2, u32 tsid2, u16 tclass2)
{
	return true;
}


void piga_list_sequence(void)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int off = 0;

	for(i = 0; i < s_len; ++i) {
		printk(KERN_INFO "PIGA: len: %u, pos: %u", seq[i].length,
		       seq[i].current_position);
		off = seq[i].link_offset;
		for(j = 0; j < l_len; ++j) {
			printk(KERN_INFO "%u-(%u,%u)->%u",
			       link[off + j].cs, link[off + j].tclass,
			       link[off + j].requested, link[off + j].cc);
		}
	}
}


int piga_set_pol(unsigned int s_l, unsigned int l_l,
		 struct sequence * s, struct link * l)
{
	if(seq != NULL) {
		vfree(seq);
	}
	if(link != NULL) {
		vfree(link);
	}
	seq = s;
	link = l;
	s_len = s_l;
	l_len = l_l;

	return 0;
}


// int piga_add_sequence(struct sequence * s)
// {
// // 	printk(KERN_INFO "PIGA: Adding sequence");
//
// 	if (s == NULL) {
// 		printk(KERN_ERR "PIGA: Can't add a NULL sequence!");
// 	}
//
// 	if (sequences_size == sequence_max) {
// 		printk(KERN_INFO "PIGA: Can't add sequence, no space left");
// 		return -1;
// 	}
//
// // 	printk(KERN_INFO "PIGA: len: %u, pos: %u", s->length, s->current_position);
//
// 	sequences[sequence_max] = s;
// 	s->link_list = NULL;
// 	++sequence_max;
//
// 	return 0;
// }


// int piga_set_link(struct link * l)
// {
// 	struct sequence * s;
// // 	unsigned int j = 0;
// 	struct link * lk;
// // 	printk(KERN_ERR "PIGA: Adding link_list");
//
// 	sequences[sequence_max - 1]->link_list = l;
//
// 	s = sequences[sequence_max - 1];
//
// // 	printk(KERN_INFO "PIGA: len: %u, pos: %u", s->length, s->current_position);
// 	lk = l;
// // 	for(j = 0; j < s->length; ++j) {
// // 		l = lk +j;
// // 		printk(KERN_INFO "%u-(%u,%u)->%u", l->cs, l->tclass, l->requested, l->cc);
// // 	}
//
// 	return 0;
// }


// unsigned int piga_get_length(void)
// {
// 	return sequences[sequence_max - 1]->length;
// }


/*int piga_add_sequence(char * seq)
{
	struct sequence * s;
// 	struct list_head * l;
	int ret = 0;

	if (seq == NULL) {
		printk(KERN_ERR "PIGA: piga_remove_sequence: seq is NULL");
		return -1;
	}

	s = vmalloc(sizeof(struct sequence));
	s->seq_string = vmalloc(sizeof(char) * strlen(seq) + 1);
	s->current_position = 0;

	s->length = 3;
	s->link_list = vmalloc(sizeof(struct link) * s->length);

	if (s == NULL || s->seq_string == NULL || s->link_list == NULL) {
		printk(KERN_ERR "PIGA: not enough memory to store the sequence: %s", seq);
		if (s->link_list != NULL) vfree(s->link_list);
		if (s->seq_string != NULL) vfree(s->seq_string);
		if (s != NULL) vfree(s);
		return -ENOMEM;
	}

	// Here we should parse 'seq' and create the sequence
	// passwd_t, read, sysadm_t, write, tmp_t, read, sysadm_t
	ret += security_context_to_sid("root:sysadm_r:sysadm_t", 22, &(s->link_list[0].cs));
	ret += security_context_to_sid("root:sysadm_r:sysadm_t", 22, &(s->link_list[1].cs));
	ret += security_context_to_sid("root:sysadm_r:sysadm_t", 22, &(s->link_list[2].cs));
	ret += security_context_to_sid("system_u:object_r:locale_t", 26, &(s->link_list[0].cc));
	ret += security_context_to_sid("root:object_r:user_tmp_t", 24, &(s->link_list[1].cc));
	ret += security_context_to_sid("root:object_r:user_tmp_t", 24, &(s->link_list[2].cc));
	// We should use "../flask.h" stuff here :
	s->link_list[0].tclass = SECCLASS_FILE;
	s->link_list[1].tclass = SECCLASS_FILE;
	s->link_list[2].tclass = SECCLASS_FILE;
	// We should use "../av_permissions.h" here :
	s->link_list[0].requested = FILE__READ;
	s->link_list[1].requested = FILE__WRITE;
	s->link_list[2].requested = FILE__READ;

	printk(KERN_INFO "PIGA: Found match : %s, %d", "root:sysadm_r:sysadm_t", s->link_list[0].cs);
	printk(KERN_INFO "PIGA: Found match : %s, %d", "root:sysadm_r:sysadm_t", s->link_list[1].cs);
	printk(KERN_INFO "PIGA: Found match : %s, %d", "root:sysadm_r:sysadm_t", s->link_list[2].cs);

	printk(KERN_INFO "PIGA: Found match : %s, %d", "system_u:object_r:locale_t", s->link_list[0].cc);
	printk(KERN_INFO "PIGA: Found match : %s, %d", "root:object_r:user_tmp_t", s->link_list[1].cc);
	printk(KERN_INFO "PIGA: Found match : %s, %d", "root:object_r:user_tmp_t", s->link_list[2].cc);

	printk(KERN_INFO "PIGA: Found match : %s, %d", "read", s->link_list[0].tclass);
	printk(KERN_INFO "PIGA: Found match : %s, %d", "write", s->link_list[1].tclass);
	printk(KERN_INFO "PIGA: Found match : %s, %d", "read", s->link_list[2].tclass);

	if (ret != 0) {
		// FIXME
		printk(KERN_ERR "PIGA: can't find the sid for this context...");
		vfree(s->seq_string);
		vfree(s->link_list);
		vfree(s);
		return -ENOMEM;
	}

	strncpy(s->seq_string, seq, strlen(seq) + 1);

	if (sequence_max < 20) {
		sequences[sequence_max] = s;
		++sequence_max;
	} else {
		printk(KERN_ERR "PIGA: to many sequences for now!");
	}

// 	INIT_LIST_HEAD(&s->list);
//
// 	if (sequences == NULL){
// 		sequences = kmalloc(sizeof(struct list_head) * MAX_SID, GFP_NOFS);
// 		if (sequences == NULL) {
// 			printk(KERN_ERR "PIGA: can't allocate mem for sequences (MAX_TCLASS).");
// 			kfree(s->link_list);
// 			kfree(s);
// 			return -ENOMEM;
// 		}
// 	}
// 	l = sequences[s->link_list[0].cs];
// 	if (l == NULL) {
// 		l = kmalloc(sizeof(struct list_head), GFP_NOFS);
// 		if (l == NULL) {
// 			printk(KERN_ERR "PIGA: can't allocate mem for sequence list.");
// 			kfree(s->link_list);
// 			kfree(s);
// 			return -ENOMEM;
// 		}
// 		l->prev = l;
// 		l->next = l;
// // 		*l = LIST_HEAD_INIT(sid); // FIXME check this
// 	}

// 	list_add(&s->list, l);
	printk(KERN_INFO "PIGA: New sequence added: %s", seq);

	return 0;
}*/


int piga_compare(u16 tclass1, u32 requested1, u16 tclass2, u32 requested2)
{
	if(tclass1 != tclass2) {
		return tclass1 - tclass2;
	} else if(requested1 != requested2) {
		return requested1 - requested2;
	}

	return 0;
}


int * piga_status(void)
{
	return &piga_status_enabled;
}


int * piga_audit(void)
{
	return &piga_audit_only_mode;
}


void print_vector(u32 ssid, u32 tsid, u16 tclass, u32 requested, struct common_audit_data *auditdata, int rc, struct av_decision * avd)
{
	char * tc = NULL;
	char * sc = NULL;
	int perm = 1;
	int sc_l = 0;
	char ** perms = NULL;
	char * perm_dumped = NULL;
	int len = 0;
	int prev_len = 0;
	unsigned int i = 0;

	security_sid_to_context(ssid, &sc, &sc_l);
	security_sid_to_context(tsid, &tc, &sc_l);
	perms = secclass_map[tclass - 1].perms; // FIXME Fix this warning

	perm_dumped = vmalloc(sizeof(char) * AVC_BUFFER_SIZE * 10); // FIXME
	if (perm_dumped == NULL) {
		printk(KERN_ERR "PIGA: not enough memory for the permission buffer");
		return; // this could also be: return -ENOMEM;
	}

	perm = 1;
	i = 0;
	perm_dumped[0] = '\0';
	while (i < (sizeof(requested) * 8)) {
		if ((perm & requested) && perms[i]) {
			prev_len = strlen(perm_dumped);
			len = scnprintf(perm_dumped + prev_len, AVC_BUFFER_SIZE, " %s", perms[i]);
			if ((len > AVC_BUFFER_SIZE) || (len < 0)) {
				printk(KERN_ERR "PIGA: scnprintf problem");
			}
			prev_len += len;
			perm_dumped[prev_len] = '\0';
			requested &= ~perm;
		}
		++i;
		perm <<= 1;
	}
	if (requested) {
		len = scnprintf(perm_dumped + prev_len, AVC_BUFFER_SIZE, " 0x%x", requested);
		if ((len > AVC_BUFFER_SIZE) || (len < 0)) {
				printk(KERN_ERR "PIGA: scnprintf problem");
		}
		prev_len += len;
		perm_dumped[prev_len] = '\0';
	}

	printk(KERN_INFO "PIGA: %s -(%s:%s)-> %s, rc: %d", sc,
	       secclass_map[tclass - 1].name, perm_dumped, tc, rc);
	kfree(tc);
	kfree(sc);
	vfree(perm_dumped);
}


int piga_has_perm(u32 ssid, u32 tsid, u16 tclass, u32 requested, struct common_audit_data *auditdata, int rc, struct av_decision * avd)
{
	struct sequence * seqs = NULL;
	struct sequence * s = NULL;
	unsigned int i = 0;
	u32 denied = 0, audited = 0;

	if (piga_status_enabled) {
		/**
		 * Resolve if SELinux decided to allow or deny or log the syscall.
		 *
		 * For each vector in a PIGA signature/seqeunce, you should add
		 * the corresponding SELinux auditallow rule.
		 * This simple change will save us time as we don't have to check
		 * for this vector "being" in any PIGA signature.
		**/
		if (avd) {
			denied = requested & ~avd->allowed;
			if (piga_audit_only_mode) {
				if (denied) {
					audited = denied;
					if (!(audited & avd->auditdeny))
						return rc;
				} else {
					audited = requested;
					if (!(audited & avd->auditallow))
						return rc;
				}
			}
		}

		rc = PIGA_ALLOW;
		seqs = piga_get_sequence_at(ssid, tsid, tclass);
		for (i = 0; i < s_len; ++i) {
			s = seqs + i;
			if (piga_seq_get_cs(s) == ssid
				&& piga_seq_get_cc(s) == tsid
				&& piga_seq_get_tclass(s) == tclass
				&& (piga_seq_get_requested(s) & requested) > 0) {
				print_vector(ssid, tsid, tclass, requested, auditdata, rc, avd);
				if (piga_seq_end(s) == true) {
					printk(KERN_INFO "PIGA: End of sequence. Access denied\n");
					rc = PIGA_DENY;
				} else {
					piga_seq_next(s);
				}
			}
		}
	}

	return rc;
}


int init_piga()
{
	printk(KERN_DEBUG "PIGA:  Starting in permissive mode\n");
	return 0;
}
