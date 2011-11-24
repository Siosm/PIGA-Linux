#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/kernel.h>

#include "include/procfs.h"


static u32 sid = 0;


/**
 * FIXME This does not look correct for all of those functions :
 * We have to make sure we're handling parameters and stuff correctly,
 * especially regarding strict checking of input strings
**/


int piga_proc_status_read(char *buffer, char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data)
{
	int result;

	if (offset > 0) {
		result  = 0; /* we have finished reading, return 0 */
	} else if (*piga_status() == 0) {
		result = sprintf(buffer, "Disabled\n"); /* fill the buffer, return the buffer size */
	} else {
		result = sprintf(buffer, "Enabled\n");
	}

	return result;
}


int piga_proc_status_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char tmp[count + 1];

	if (copy_from_user(tmp, buffer, count)) {
		return -EFAULT;
	}

	tmp[count] = '\0';

	if (*tmp == '0') {
		*piga_status() = false;
		printk(KERN_INFO "PIGA: Status: Disabled");
	} else if (*tmp == '1') {
// 		piga_add_sequence("locale_t, read, sysadm_t, write, tmp_t, read, sysadm_t");
		*piga_status() = true;
		printk(KERN_INFO "PIGA: Status: Enabled");
	}

	return count;
}


int piga_proc_mode_read(char *buffer, char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data)
{
	int result;

	if (offset > 0) {
		result  = 0; /* we have finished reading, return 0 */
	} else if (*piga_audit_only_mode() == 0) {
		result = sprintf(buffer, "Disabled: Full check\n"); /* fill the buffer, return the buffer size */
	} else {
		result = sprintf(buffer, "Enabled: Audit only\n");
	}

	return result;
}


int piga_proc_mode_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char tmp[count + 1];

	if (copy_from_user(tmp, buffer, count)) {
		return -EFAULT;
	}

	tmp[count] = '\0';

	if (*tmp == '0') {
		*piga_audit_only_mode() = false;
		printk(KERN_INFO "PIGA: Audit only mode : off");
	} else if (*tmp == '1') {
		*piga_audit_only_mode() = true;
		printk(KERN_INFO "PIGA: Audit only mode : on");
	}

	return count;
}


int piga_proc_getsid_read(char *buffer, char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data)
{
	int result;

	if (offset > 0) {
		result  = 0; /* we have finished reading, return 0 */
	} else {
		result = sprintf(buffer, "%u\n", sid); /* fill the buffer, return the buffer size */
		sid = 0;
	}

	return result;
}


int piga_proc_getsid_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	int ret;
	char tmp[count + 1];

	if (copy_from_user(tmp, buffer, count)) {
		return -EFAULT;
	}

	tmp[count] = '\0';
	tmp[count - 1] = '\0';

	ret = security_context_to_sid(tmp, strlen(tmp), &sid);
	if(ret != 0) {
		printk(KERN_INFO "PIGA: an error occured sid: %s, u32: %ud, ret: %d", tmp, sid, ret);
		sid = 0;
	}

	return count;
}


int piga_proc_load_sequence_read(char *buffer, char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data)
{
	int result;

	if (offset > 0) {
		result  = 0; /* we have finished reading, return 0 */
		*eof = 1;
	} else {
		result = sprintf(buffer, "PIGA: policy: TODO\n"); /* fill the buffer, return the buffer size */
	}

	return result;
}


int piga_proc_load_sequence_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char tmp[count + 1];
	struct sequence * s = NULL;
	unsigned long int stmp = 0;

	if (copy_from_user(tmp, buffer, count)) {
		return -EFAULT;
	}

	tmp[count] = '\0';
// 	tmp[count - 1] = '\0';

	s = vmalloc(sizeof(struct sequence));
	if (s == NULL) {
		printk(KERN_INFO "PIGA: can't alloc mem sequence");
		return count;
	}

	stmp = simple_strtoul(tmp, NULL, 10);
	printk(KERN_INFO "PIGA: Loading %lu", stmp);
	if (copy_from_user(s, (const void *) stmp, sizeof(struct sequence))) {
		return -EFAULT;
	}

// 	piga_add_sequence(s);

	return count;
}


int piga_proc_load_link_read(char *buffer, char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data)
{
	int result;

	if (offset > 0) {
		result  = 0; /* we have finished reading, return 0 */
		*eof = 1;
	} else {
		result = sprintf(buffer, "PIGA: policy: TODO\n"); /* fill the buffer, return the buffer size */
	}

	return result;
}


int piga_proc_load_link_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char tmp[count + 1];
	struct link * l = NULL;
	unsigned long int ltmp = 0;

	if (copy_from_user(tmp, buffer, count)) {
		return -EFAULT;
	}

	tmp[count] = '\0';
// 	tmp[count - 1] = '\0';

// 	l = vmalloc(piga_get_length() * sizeof(struct link));
	if (l == NULL) {
		printk(KERN_INFO "PIGA: can't alloc mem sequence");
		return count;
	}

	ltmp = simple_strtoul(tmp, NULL, 10);
	printk(KERN_INFO "PIGA: Loading %lu", ltmp);
	if (copy_from_user(l, (const void *) ltmp, sizeof(struct sequence))) {
		return -EFAULT;
	}

// 	piga_set_link(l);

	return count;
}


int piga_proc_policy_read(char *buffer, char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data)
{
	int result;

	if (offset > 0) {
		result  = 0; /* we have finished reading, return 0 */
		*eof = 1;
	} else {
		piga_list_sequence();
		result = sprintf(buffer, "PIGA: policy: See dmesg output\n"); /* fill the buffer, return the buffer size */
	}

	return result;
}


int piga_proc_policy_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	return count;
}


int init_piga_procfs(void)
{
	struct proc_dir_entry *piga_proc_dir, *piga_proc_status,
		*piga_proc_mode, *piga_proc_getsid, *piga_proc_load_sequence,
		*piga_proc_load_link, *piga_proc_policy;

	printk(KERN_INFO "PIGA: Creating /proc entries.\n");

	/** Create PIGA directory **/
	piga_proc_dir = proc_mkdir(PROC_PIGA_DIR, NULL);
	if (piga_proc_dir == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/", PROC_PIGA_DIR);
		return -ENOMEM; // FIXME Check this return code
	}

	/** Toogle PIGA status: ON/OFF **/
	piga_proc_status = create_proc_entry(PROC_PIGA_STATUS, 0600, piga_proc_dir);
	if (piga_proc_status == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/%s file", PROC_PIGA_DIR, PROC_PIGA_STATUS);
		remove_proc_entry(PROC_PIGA_DIR, piga_proc_dir);
		return -ENOMEM; // FIXME Check this return code
	}

	piga_proc_status->read_proc    = piga_proc_status_read;
	piga_proc_status->write_proc   = piga_proc_status_write;
	piga_proc_status->mode         = S_IFREG | S_IRUGO;
	piga_proc_status->uid          = 0;
	piga_proc_status->gid          = 0;
	piga_proc_status->size         = 0;

	/**
	 * Toogle PIGA mode:
	 * Complete (check every interactions)
	 * Audit only (check only selinux audited interactions)
	**/
	piga_proc_mode = create_proc_entry(PROC_PIGA_MODE, 0600, piga_proc_dir);
	if (piga_proc_mode == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/%s file", PROC_PIGA_DIR, PROC_PIGA_MODE);
		remove_proc_entry(PROC_PIGA_STATUS, piga_proc_status);
		remove_proc_entry(PROC_PIGA_DIR, piga_proc_dir);
		return -ENOMEM; // FIXME Check this return code
	}

	piga_proc_mode->read_proc    = piga_proc_mode_read;
	piga_proc_mode->write_proc   = piga_proc_mode_write;
	piga_proc_mode->mode         = S_IFREG | S_IRUGO;
	piga_proc_mode->uid          = 0;
	piga_proc_mode->gid          = 0;
	piga_proc_mode->size         = 0;

	/** Used to get sid in userspace **/
	piga_proc_getsid = create_proc_entry(PROC_PIGA_GETSID, 0600, piga_proc_dir);
	if (piga_proc_getsid == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/%s file", PROC_PIGA_DIR, PROC_PIGA_GETSID);
		remove_proc_entry(PROC_PIGA_STATUS, piga_proc_status);
		remove_proc_entry(PROC_PIGA_MODE, piga_proc_mode);
		remove_proc_entry(PROC_PIGA_DIR, piga_proc_dir);
		return -ENOMEM; // FIXME Check this return code
	}

	piga_proc_getsid->read_proc    = piga_proc_getsid_read;
	piga_proc_getsid->write_proc   = piga_proc_getsid_write;
	piga_proc_getsid->mode         = S_IFREG | S_IRUGO;
	piga_proc_getsid->uid          = 0;
	piga_proc_getsid->gid          = 0;
	piga_proc_getsid->size         = 0;

	/** Used to load a sequence **/
	piga_proc_load_sequence = create_proc_entry(PROC_PIGA_LOAD_SEQ, 0200, piga_proc_dir);
	if (piga_proc_load_sequence == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/%s file", PROC_PIGA_DIR, PROC_PIGA_LOAD_SEQ);
		remove_proc_entry(PROC_PIGA_GETSID, piga_proc_getsid);
		remove_proc_entry(PROC_PIGA_STATUS, piga_proc_status);
		remove_proc_entry(PROC_PIGA_MODE, piga_proc_mode);
		remove_proc_entry(PROC_PIGA_DIR, piga_proc_dir);
		return -ENOMEM; // FIXME Check this return code
	}

	piga_proc_load_sequence->read_proc    = piga_proc_load_sequence_read;
	piga_proc_load_sequence->write_proc   = piga_proc_load_sequence_write;
	piga_proc_load_sequence->mode         = S_IFREG | S_IRUGO;
	piga_proc_load_sequence->uid          = 0;
	piga_proc_load_sequence->gid          = 0;
	piga_proc_load_sequence->size         = 0;

	/** Used to load a link for the current sequence **/
	piga_proc_load_link = create_proc_entry(PROC_PIGA_LOAD_LINK, 0200, piga_proc_dir);
	if (piga_proc_load_link == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/%s file", PROC_PIGA_DIR, PROC_PIGA_LOAD_LINK);
		remove_proc_entry(PROC_PIGA_LOAD_SEQ, piga_proc_load_sequence);
		remove_proc_entry(PROC_PIGA_GETSID, piga_proc_getsid);
		remove_proc_entry(PROC_PIGA_STATUS, piga_proc_status);
		remove_proc_entry(PROC_PIGA_MODE, piga_proc_mode);
		remove_proc_entry(PROC_PIGA_DIR, piga_proc_dir);
		return -ENOMEM; // FIXME Check this return code
	}

	piga_proc_load_link->read_proc    = piga_proc_load_link_read;
	piga_proc_load_link->write_proc   = piga_proc_load_link_write;
	piga_proc_load_link->mode         = S_IFREG | S_IRUGO;
	piga_proc_load_link->uid          = 0;
	piga_proc_load_link->gid          = 0;
	piga_proc_load_link->size         = 0;

	/** Used to print the current loaded signatures **/
	piga_proc_policy = create_proc_entry(PROC_PIGA_POLICY, 0400, piga_proc_dir);
	if (piga_proc_policy == NULL) {
		printk(KERN_ERR "PIGA: Failed to create /proc/%s/%s file", PROC_PIGA_DIR, PROC_PIGA_POLICY);
		remove_proc_entry(PROC_PIGA_LOAD_LINK, piga_proc_load_link);
		remove_proc_entry(PROC_PIGA_LOAD_SEQ, piga_proc_load_sequence);
		remove_proc_entry(PROC_PIGA_GETSID, piga_proc_getsid);
		remove_proc_entry(PROC_PIGA_STATUS, piga_proc_status);
		remove_proc_entry(PROC_PIGA_MODE, piga_proc_mode);
		remove_proc_entry(PROC_PIGA_DIR, piga_proc_dir);
		return -ENOMEM; // FIXME Check this return code
	}

	piga_proc_policy->read_proc    = piga_proc_policy_read;
	piga_proc_policy->write_proc   = piga_proc_policy_write;
	piga_proc_policy->mode         = S_IFREG | S_IRUGO;
	piga_proc_policy->uid          = 0;
	piga_proc_policy->gid          = 0;
	piga_proc_policy->size         = 0;

	printk(KERN_INFO "PIGA: Successfully created /proc entries.\n");

	return 0;
}