Before printing process information in `/var/process.log`, we have two things to do.

1. Create a standard output which in number 3, and the output location is `/var/process/log`. So we can call the function

	fprintk(3, "%ld\t%c\t%ld\n", current->pid, 'R', jiffies); 
to echo something we want into this file.

2. As we seen, there is 5 parameters in function `fprintk`

	* standard output number.
	* string
	* pid
	* status, like N J R W E
	* jiffies, this is a parameter which save how long the OS has runned.

What is jiffies? it's a parameter which binding to the Clock. every 10ms, jiffies plus 1, so we can use jiffies to get the time since OS started.
