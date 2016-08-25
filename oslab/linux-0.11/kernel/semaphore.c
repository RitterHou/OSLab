#include <unistd.h>
#include <linux/sched.h>
//#include <string.h>
#include <asm/system.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#define MAX_SEMARRAY_NUM 80
#define sem_t int

struct Sem_wait_list{
    struct tast_struct *TaskNode;
    char name[20];
    int  value;
};

struct Sem_wait_list SemArray[MAX_SEMARRAY_NUM]={0};//Max number of semaphony is 80
sem_t  sys_sem_open(const char *name,unsigned int value)
{
    int i=0;
    int LastEmpty = 0;
    char UserMemoryByte[2]={'\0','\0'};
    UserMemoryByte[0] = get_fs_byte(name);
    printk("sys_sem_open:name:%s,value:%d\n",UserMemoryByte ,value);
    for(i=1;i<MAX_SEMARRAY_NUM;i++)
    {
        if(SemArray[i].name[0] =='\0')
        {
            LastEmpty = i;
            continue;
        }
        if(!strcmp(SemArray[i].name,UserMemoryByte))
        {
            printk("sys_sem_open:find the sem\n");
            return i;
        }
    }
    //Not found
    if(0 == LastEmpty)
    {
        //The array is full
        return -1;
    }
    //Add new semaphony
    strcpy(SemArray[LastEmpty].name ,UserMemoryByte);
    SemArray[LastEmpty].value = value;
    printk("sys_sem_open:add new sem:index:%d,value:%d,name%s\n",LastEmpty,value,name);
    return LastEmpty;
}
int sys_sem_wait(sem_t sem)
{
    cli();
    if(sem >=  MAX_SEMARRAY_NUM || sem<= 0 )
    {
        printk("sys_sem_wait:sem:%d is out of range\n",sem);
        sti();
        return -1;
    }
    if( SemArray[sem].name[0] == '\0' )
    {
        //This semophony does'n exist
        sti();
        return -1;
    }
    printk("sys_sem_wait:sem:%d,pid%d\n",sem,sys_getpid());
    while( 0 >= SemArray[sem].value )
    {
        sleep_on( &(SemArray[sem].TaskNode) );
    }
    printk("sys_sem_wait:procedure pass,value:%d,sem:%d,pid%d\n",SemArray[sem].value,sem,sys_getpid());
    
    //Catch the symaphony 
    --SemArray[sem].value;
    
    sti();
}
int sys_sem_post(sem_t sem)
{
    cli();
    if( SemArray[sem].name[0] == '\0' )
    {
        //This semophony does'n exist
        sti();
        return -1;
    }
    ++SemArray[sem].value;
    wake_up(  &(SemArray[sem].TaskNode) );      
    sti();
}
int sys_sem_unlink(const char *name)
{
    
    char UserMemoryByte[2]={'\0','\0'};
    UserMemoryByte[0] = get_fs_byte(name);
    int i=0;
    for(i=1;i<MAX_SEMARRAY_NUM;i++)
    {
        if( !strcmp(SemArray[i].name,UserMemoryByte) )
        {
            SemArray[i].name[0] = '\0';
            SemArray[i].TaskNode = 0;
            SemArray[i].value=0;
            return 0;
        }
    }
}