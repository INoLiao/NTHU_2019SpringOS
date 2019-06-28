//////////////////////////////////////////////
// Author: I-No Liao                        //
// Date of update: 2019/03/13               //
// Description: OS HW1 Kernel Linked List   //
//////////////////////////////////////////////

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

// variables
#define maxStrLength 10

// linked list node
struct student {
    char studentID[maxStrLength];
    char day[maxStrLength];
    char month[maxStrLength];
    char year[maxStrLength];
    struct list_head list;
};

// global variable
static LIST_HEAD(studentList);

struct student* addStudent(char *studentID, char *day, char *month, char *year) {
    struct student *node = kmalloc(sizeof(struct student), GFP_KERNEL);

    // check memory allocation
    if (node == NULL) {
        return NULL;
    }

    // check parameter validity
    if (strlen(studentID) > maxStrLength || strlen(day) > maxStrLength || strlen(month) > maxStrLength || strlen(year) > maxStrLength) {
        return NULL;
    }

    // write values
    strcpy(node->studentID, studentID);
    strcpy(node->day, day);
    strcpy(node->month, month);
    strcpy(node->year, year);
    
    return node;
}

// load kernel module
int sample_init(void) {
    printk(KERN_INFO "----- Loading Module -----\n");

    // INIT_LIST_HEAD(&studentList);
    struct student *node = NULL;

    // adding student nodes
    node = addStudent("00001", "01", "01", "2001");
    if (node == NULL) {
        printk(KERN_INFO ">> adding node failed\n");
    } else {
        list_add_tail(&node->list, &studentList);
    }

    node = addStudent("00002", "02", "02", "2002");
    if (node == NULL) {
        printk(KERN_INFO ">> adding node failed\n");
    } else {
        list_add_tail(&node->list, &studentList);
    }

    node = addStudent("00003", "03", "03", "2003");
    if (node == NULL) {
        printk(KERN_INFO ">> adding node failed\n");
    } else {
        list_add_tail(&node->list, &studentList);
    }

    node = addStudent("00004", "04", "04", "2004");
    if (node == NULL) {
        printk(KERN_INFO ">> adding node failed\n");
    } else {
        list_add_tail(&node->list, &studentList);
    }

    node = addStudent("00005", "05", "05", "2005");
    if (node == NULL) {
        printk(KERN_INFO ">> adding node failed\n");
    } else {
        list_add_tail(&node->list, &studentList);
    }

    // traverse student linked list
    list_for_each_entry(node, &studentList, list) {
        printk(KERN_INFO "studentID: %s, birthday: %s-%s-%s | address: %p\n", node->studentID, node->day, node->month, node->year, node);
    }

    return 0;
}

// remove kernel module
void sample_exit(void) {
    printk(KERN_INFO "----- Removing Module -----\n");

    struct student *node, *next;

    // delete student nodes from head
    list_for_each_entry_safe(node, next, &studentList, list) {
        printk(KERN_INFO "Freeing node w/ studentID: %s | address: %p\n", node->studentID, node);
        list_del(&node->list);
        kfree(node);
    }

    // check deletion correctness
    if (list_empty(&studentList)) {
        printk(KERN_INFO ">> Deletion completed\n");
    } else {
        printk(KERN_INFO ">> Incomplete delection\n");
    }

    return;
}

// macros for registering module entry and exit points
module_init(sample_init);
module_exit(sample_exit);

MODULE_LICENSE("GPL");                      // license
MODULE_DESCRIPTION("Linked List Module");   // module description
MODULE_AUTHOR("I-No Liao");                 // author name
