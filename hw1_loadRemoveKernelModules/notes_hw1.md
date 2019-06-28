# Operating Systems 2019 Spring

## HW1 Notes

### Kernel Module

The following instructions guides us how to initiate and terminate linux kernel using c. 

- Use the make file to compile the c code: sample.c. sample.ko will be generated for kernel initiation and termination.
	
	```bash
	$ make
	```
	
- To load the kernel module,

	```bash
	$ sudo insmod sample.ko
	```

- To check if the kernel module is successfully loaded, 
	
	```bash
	$ sudo lsmod | grep "sample"
	```
	
	- Since there are lots of system modules, directly entering ```lsmod``` will print too many unwanted messages. We exploit ```grep``` to extract interested module. In this example, the interested module is  "sample".
	
- To remove the kernel module,

	```bash
	$ sudo rmmod sample.ko
	```
	
- To check the kernel log buffer,

	```bash
	$ dmesg
	```

- Reference: https://jerrynest.io/how-to-write-a-linux-kernel-module/?fbclid=IwAR1IkA_K2ZJe7D9hoLQHBesnQHPG_ybUmlomnKeoh2kj73LfyedCGsmEpNU

### Linux Kernel Linked List

Use linux kernel list.h to implement the linked list. It is quite important to trace the functions in list.h to thoroughly understand the arguments' meaning. In our application, we use construct a struct containing studentID and birthday. 

The following lists the functions we use in this assignment

- ```LIST_HEAD()```
- ```list_add_tail()```
- ```list_for_each_entry()```
- ```list_for_each_entry_safe()```
- ```list_empty()```


Reference:

- list.h: https://github.com/torvalds/linux/blob/master/include/linux/list.h
- Example: https://github.com/msesmart/OperatingSystem/blob/master/KernelExperiment/p1cyuan.c
- Example: https://github.com/Wal33D/LinuxKernelModule/blob/master/Project5.c
- kernel linked list: https://www.cnblogs.com/muahao/p/7509629.html
- kernel linked list: https://www.cnblogs.com/hwy89289709/p/6754300.html