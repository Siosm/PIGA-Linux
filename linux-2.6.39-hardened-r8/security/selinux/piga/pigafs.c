#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h>

#include "include/piga.h"


static struct kobject *pigafs_kobj;


static ssize_t piga_status_read(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	int result;

	if (*piga_status() == 0) {
		result = sprintf(buf, "Disabled\n");
	} else {
		result = sprintf(buf, "Enabled\n");
	}

	return result;
}


static ssize_t piga_status_write(struct kobject *kobj, struct kobj_attribute *attr,
				 const char *buf, size_t count)
{
	sscanf(buf, "%d", piga_status());
	return count;
}


static struct kobj_attribute piga_status_attribute =
	__ATTR(status, 0600, piga_status_read, piga_status_write);


static ssize_t piga_audit_only_mode_read(struct kobject *kobj,
					 struct kobj_attribute *attr, char *buf)
{
	int result;

	if (*piga_audit() == 0) {
		result = sprintf(buf, "Disabled: Full check\n");
	} else {
		result = sprintf(buf, "Enabled: SELinux audited interactions only\n");
	}

	return result;
}


static ssize_t piga_audit_only_mode_write(struct kobject *kobj, struct kobj_attribute *attr,
					  const char *buf, size_t count)
{
	sscanf(buf, "%d", piga_audit());
	return count;
}


static struct kobj_attribute piga_audit_only_mode_attribute =
	__ATTR(mode, 0600, piga_audit_only_mode_read, piga_audit_only_mode_write);


static struct attribute *piga_attrs[] = {
	&piga_audit_only_mode_attribute.attr,
	&piga_status_attribute.attr,
	NULL,
};


static struct attribute_group piga_attr_group = {
	.attrs = piga_attrs,
};


int init_piga_fs()
{
	int retval;

// 	init_piga_procfs();

	pigafs_kobj = kobject_create_and_add("piga", fs_kobj);
	if (!pigafs_kobj) {
		printk(KERN_ERR "PIGA: Can't create piga fs.");
		return -ENOMEM;
	}

	retval = sysfs_create_group(pigafs_kobj, &piga_attr_group);
	if (retval) {
		kobject_put(pigafs_kobj);
	}

	return retval;
}
