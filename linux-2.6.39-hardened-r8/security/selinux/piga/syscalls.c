/**
 * Some syscalls to easily give stuff to the kernel
 **/


#include <linux/pid.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/linkage.h>
#include <linux/kernel.h>

#include "include/piga.h"


/**
 *
 **/
asmlinkage long sys_piga_add_sequence(unsigned int s_len, unsigned int l_len,
				      struct sequence __user * s,
				      struct link __user * l)
{
	struct sequence * seq;
	struct link * link;
	unsigned int i = 0, j = 0, off = 0, link_list_len = 0;

	if ((s == NULL) || (l == NULL) || (s_len <=0) || (l_len <= 0)) {
		printk(KERN_ERR "PIGA: Check syscall parameters!");
		return -EFAULT;
	}

	seq = vmalloc(sizeof(struct sequence) * s_len);
	if (seq == NULL) {
		printk(KERN_ERR "PIGA: can't alloc mem for sequences");
		return -ENOMEM;
	}
	if (copy_from_user(seq, s, sizeof(struct sequence) * s_len)) {
		printk(KERN_ERR "PIGA: can't copy sequence from userspace");
		vfree(seq);
		return -EFAULT;
	}

	link = vmalloc(sizeof(struct link) * l_len);
	if (link == NULL) {
		printk(KERN_ERR "PIGA: can't alloc mem for links");
		vfree(seq);
		return -ENOMEM;
	}
	if (copy_from_user(link, l, sizeof(struct link) * l_len)) {
		printk(KERN_ERR "PIGA: can't copy links from userspace");
		vfree(link);
		vfree(seq);
		return -EFAULT;
	}

	for(i = 0; i < s_len; ++i) {
		printk(KERN_INFO "PIGA: len: %u, pos: %u", seq[i].length,
		       seq[i].current_position);
		off = seq[i].link_offset;
		link_list_len = seq[i].length;
		for(j = 0; j < link_list_len; ++j) {
			printk(KERN_INFO "%u-(%u,%u)->%u",
			       link[off + j].cs, link[off + j].tclass,
			       link[off + j].requested, link[off + j].cc);
		}
	}

	return piga_set_pol(s_len, l_len, seq, link);
}
