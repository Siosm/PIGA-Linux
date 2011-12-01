#include <linux/pid.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/linkage.h>
#include <linux/kernel.h>

#include "include/piga.h"


/**
 * Load a PIGA policy into the kernel
 **/
asmlinkage long sys_piga_add_sequence(unsigned int __user s_len, unsigned int __user l_len,
				      struct sequence __user * s, struct link __user * l)
{
	struct sequence * seq;
	struct link * link;
	unsigned int i = 0, j = 0, off = 0, link_list_len = 0;

	if ((s == NULL) || (l == NULL) || (s_len <=0) || (l_len <= 0)) {
// 		printk(KERN_ERR "PIGA: Wrong syscall arguments");
		return -EFAULT;
	}

	seq = vmalloc(sizeof(struct sequence) * s_len);
	if (seq == NULL) {
		printk(KERN_ERR "PIGA: Can't alloc mem for sequences");
		return -ENOMEM;
	}
	if (copy_from_user(seq, s, sizeof(struct sequence) * s_len)) {
		printk(KERN_ERR "PIGA: Can't copy sequence from userspace");
		vfree(seq);
		return -EFAULT;
	}

	link = vmalloc(sizeof(struct link) * l_len);
	if (link == NULL) {
		printk(KERN_ERR "PIGA: Can't alloc mem for links");
		vfree(seq);
		return -ENOMEM;
	}
	if (copy_from_user(link, l, sizeof(struct link) * l_len)) {
		printk(KERN_ERR "PIGA: Can't copy links from userspace");
		vfree(link);
		vfree(seq);
		return -EFAULT;
	}

	printk(KERN_INFO "PIGA: Listing added sequences");
	for(i = 0; i < s_len; ++i) {
		if (seq[i].length == 0) {
			printk(KERN_ERR "PIGA: This sequence isn't valid. Stopping");
			return -1;
		}
		printk(KERN_INFO "len: %u, pos: %u", seq[i].length, seq[i].current_position);
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


asmlinkage long sys_piga_get_sid(char __user * scontext, unsigned int __user len, unsigned int __user * sid)
{
	char * scon;
	int ret;
	u32 tmp_sid;

	if ((sid == NULL) || (scontext == NULL) || (len <= 0)) {
// 		printk(KERN_ERR "PIGA: Wrong syscall arguments");
		return -EFAULT;
	}

	scon = vmalloc(sizeof(char) * (len + 1));
	if (scon == NULL) {
		return -ENOMEM;
	}
	if (copy_from_user(scon, scontext, sizeof(char) * len)) {
		printk(KERN_ERR "PIGA: Can't copy security context from userspace");
		vfree(scon);
		return -EFAULT;
	}
	scon[len] = '\0';

	ret = security_context_to_sid(scon, len, &tmp_sid);
	if(ret != 0) {
		printk(KERN_INFO "PIGA: An error occured sid: %s, u32: %ud, ret: %d", scon, *sid, ret);
	}
	vfree(scon);

	if (copy_to_user(sid, &tmp_sid, sizeof(int))) {
		printk(KERN_ERR "PIGA: Can't copy sid to userspace");
		return -EFAULT;
	}

	return 0;
}
