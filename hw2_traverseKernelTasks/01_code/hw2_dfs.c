//////////////////////////////////////////////
// Author: I-No Liao                        //
// Date of update: 2019/04/08               //
// Description: OS HW2 System Task Iterator //
//////////////////////////////////////////////

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/sched/signal.h>

// dfs
void dfs(struct task_struct *task) {
    struct list_head *list;
    struct task_struct *child;
    
    // manipulation of current task: print task
    printk(KERN_INFO "pid: %d | process name: %s | state: %d\n", task->pid, task->comm, task->state);

    // visit children of current task 
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);

        // dfs
        dfs(child);
    }

    return;
}

// load kernel module
int task_iterator_init(void) {
    printk(KERN_INFO "----- Loading Module -----\n");

    // iterate tasks in dfs manner
    dfs(&init_task);

    return 0;
}

// remove kernel module
void task_iterator_exit(void) {
    printk(KERN_INFO "----- Removing Module -----\n");
    return;
}

// macros for registering module entry and exit points
module_init(task_iterator_init);
module_exit(task_iterator_exit);

MODULE_LICENSE("GPL");                          // license
MODULE_DESCRIPTION("Iterate tasks in dfs manner");   // module description
MODULE_AUTHOR("I-No Liao");                     // author name
