#### This is lab2 is a System Call lab.

#### The following is the step of the OS system call.

1. The user programm will include a header which named `unistd.h`, and the user programm should use a macro `_syscall`.

After doing this, this macro will make a function of this system call, and this function will call 0x80 interuption.

2. The interuption will use IDT, we can find the syscall function by using the selector, the syscall has a mapping with selector by using IDT.

3. Find the real worked function, and it will work.

files:
/include/unistd.h
/kernel/system_call.S
/include/linux/sys.h
/kernel/who.c

