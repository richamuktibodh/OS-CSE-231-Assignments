
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/sched/signal.h>
#include <linux/sched.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richa");
MODULE_DESCRIPTION("A Simple Hello World module");

static short int pidval = 1;

module_param(pidval, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

static int __init hello_init(void)
{
    struct task_struct *taskstrut;
    pr_info("%s: In init\n", __func__);
    

	if((taskstrut = get_pid_task(find_get_pid(pidval),PIDTYPE_PID))){
        	pr_info("Process: %s\t PID:[%d]\t GID:[%d]\t Userid: [%d]\n", taskstrut->comm, pidval,taskstrut->cred->gid.val,
			taskstrut->cred->uid.val);
	}

	return 0;
    
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);