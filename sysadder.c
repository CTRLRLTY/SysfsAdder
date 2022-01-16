#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/sysfs.h>

static short int add = 0;
static int op_left = 0;
static int op_right = 0;
static int answer = 0;

static int	__init kalkulator_init(void);
static void	__exit kalkulator_exit(void);

static ssize_t	attr_opleft_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf);
static ssize_t	attr_opleft_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t	attr_opright_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf);
static ssize_t	attr_opright_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t	attr_answer_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf);
static ssize_t	attr_answer_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t	attr_add_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf);
static ssize_t	attr_add_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count);

static struct kobject *kobj_kalkulator;

static struct kobj_attribute	attr_add 	= __ATTR(add, 0660, attr_add_show, (void *)attr_add_store);
static struct kobj_attribute	attr_opright	= __ATTR(op_right, 0660, attr_opright_show, (void *)attr_opright_store);
static struct kobj_attribute	attr_opleft	= __ATTR(op_left, 0660, attr_opleft_show, (void *)attr_opleft_store);
static struct kobj_attribute	attr_answer 	= __ATTR(answer, 0660, attr_answer_show, (void *)attr_answer_store);

static struct attribute *attrs[] = {
	&attr_add.attr,
	&attr_opleft.attr,
	&attr_opright.attr,
	&attr_answer.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs
};




static ssize_t attr_add_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", add);
}


static ssize_t attr_add_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%hd", &add);
	answer = (op_left + op_right) * add;
	return count;
}


static ssize_t attr_opleft_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", op_left);
}


static ssize_t attr_opleft_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &op_left);
	return count;
}


static ssize_t attr_opright_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", op_right);
}


static ssize_t attr_opright_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &op_right);
	return count;
}


static ssize_t attr_answer_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", answer);
}


static ssize_t attr_answer_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	// Reset managed devices
	answer = 0;
	add = 0;
	op_left = 0;
	op_right = 0;

	return count;
}


static int 	__init kalkulator_init (void)
{
	int ret = 0;
	pr_info("Initializing kalkulator\n");

	kobj_kalkulator = kobject_create_and_add("kalkulator", NULL);

	if(!kobj_kalkulator) 
	{
		pr_alert("Failed to create kobject\n");
		ret = -ENOMEM;
		goto end;
	}

	ret = sysfs_create_group(kobj_kalkulator, &attr_group);

	if(ret)
	{
		kobject_put(kobj_kalkulator);
	}

end:
	return ret;
}


static void 	__exit kalkulator_exit(void)
{
	pr_info("Deinitializing kalkulator\n");

	sysfs_remove_group(kobj_kalkulator, &attr_group);

	kobject_put(kobj_kalkulator);
}


module_init(kalkulator_init);
module_exit(kalkulator_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Muhammad Raznan <muhammad.raznan@binus.ac.id>");
MODULE_DESCRIPTION("A simple addition and substraction sysfs kernel driver");
