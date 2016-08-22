#define __LIBRARY__

#include <unistd.h>
#include <errno.h>
#include <asm/segment.h>

static char myname[24] = {0};

int sys_iam(const char* name){
    int i= 0;
    while(get_fs_byte(name + i)!='\0')
	i++;
    if(i>23){
        return -EINVAL;
    }
    printk("[%d]\n",i);
    i = 0;
    while(myname[i] = get_fs_byte(name + i)!='\0'){
        i++;
    }
    return i;
}

int sys_whoami(char* name, unsigned int size){
    int i = 0;
    while(myname[i] != '\0')
        i++;
    if(size<i)
        return -1;
    i = 0;
    while(myname[i] != '\0'){
        put_fs_byte(myname[i],(name+i));
        i++;
    }
    return i;
}

