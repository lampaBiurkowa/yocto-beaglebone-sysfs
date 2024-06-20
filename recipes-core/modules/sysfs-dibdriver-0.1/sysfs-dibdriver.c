#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

static struct kobject *my_kobject;
static int int_data;
static char str_data[256] = {};
const char* FOLDER_NAME = "dibsysfs";

static ssize_t int_data_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", int_data);
}

static ssize_t int_data_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &int_data);
    return count;
}

static ssize_t str_data_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", str_data);
}

static ssize_t str_data_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    strncpy(str_data, buf, sizeof(str_data) - 1);
    str_data[sizeof(str_data) - 1] = '\0';
    return count;
}

static struct kobj_attribute int_data_attribute = __ATTR(int_data, 0664, int_data_show, int_data_store);
static struct kobj_attribute str_data_attribute = __ATTR(str_data, 0664, str_data_show, str_data_store);

static int try_create_sysfs_file(struct kobject *kobj, struct attribute *attr) {
    int ret = sysfs_create_file(kobj, attr);
    if (ret) {
        printk("failed to create the %s file in /sys/kernel/%s\n", attr -> name, FOLDER_NAME);
    }

    return ret;
}

static int __init mymodule_init(void)
{
    int ret = 0;
    my_kobject = kobject_create_and_add(FOLDER_NAME, kernel_kobj);
    if (!my_kobject)
        return -ENOMEM;

    ret = try_create_sysfs_file(my_kobject, &int_data_attribute.attr);
    if (ret >= 0) {
        ret = try_create_sysfs_file(my_kobject, &str_data_attribute.attr);
    }

    return ret;
}

static void __exit mymodule_exit(void)
{
    printk("dibsyfs exit\n");
    kobject_put(my_kobject);
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lampaBiurkowa");
MODULE_DESCRIPTION("simple sysfs demonstration");