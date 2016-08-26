#include <unistd.h>
#include <linux/mm.h>//get_free_page
#include <linux/sched.h> //get_base
#include <linux/kernel.h>//printk
//#include <string.h>
#include <asm/segment.h>
#define MAX_SHMARRAY_NUM 80
struct shm_Node{
	unsigned long FreePageAddr;
	char name[20];
};
struct shm_Node ShmNodeArray[MAX_SHMARRAY_NUM]={0};//There is no more than 80 share memory pages
int sys_shmget(const char *name,unsigned long size,int shmflag)
{
	printk("sys_shmget start\n");
	int i=0;
	int LastEmpty = 0;
	char UserMemoryByte[2]={'\0','\0'};
	UserMemoryByte[0] = get_fs_byte(name);
	printk("sys_shmget:name:%s\n",UserMemoryByte);
	for(i=1;i<MAX_SHMARRAY_NUM;i++)
	{
		if(ShmNodeArray[i].name[0] =='\0')
		{
			LastEmpty = i;
			continue;
		}
		if(!strcmp(ShmNodeArray[i].name,UserMemoryByte))
		{
			printk("sys_shmget:find the share memory\n");
			return i;
		}
	}
	//Not found
	if(0 == LastEmpty)
	{
		//The array is full
		return -1;
	}
	//Add new share memory
	strcpy(ShmNodeArray[LastEmpty].name ,UserMemoryByte);
	ShmNodeArray[LastEmpty].FreePageAddr = get_free_page();
	printk("sys_shmget:add new share memory:name:%s,Free Memory Page addr:%d\n",\
		ShmNodeArray[LastEmpty].name ,ShmNodeArray[LastEmpty].FreePageAddr );
	return LastEmpty;	
}
unsigned long sys_shmat(int shmid,const long shmaddr,int shmflg)
{
	unsigned long VirtualAddr = 0,data_base=0,Put_page_ret=0;
	
	if(shmid >= MAX_SHMARRAY_NUM || shmid <= 0)
	{
		printk("sys_shmat:shmid:%d is out of range \n",shmid);
	}
	
	data_base = get_base(current -> ldt[1]);
	VirtualAddr = data_base + 0x4000000 - 132*1024;
	
	Put_page_ret = put_page(ShmNodeArray[shmid].FreePageAddr,VirtualAddr);
	printk("sys_shmat:put_page return:%d\n",Put_page_ret);
	return 0x4000000 - 132*1024;
}