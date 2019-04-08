#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
volatile int ticketCount=1000;
void *SaleThread(void *arg)
{
	int num,temp;
	num=1000;
	int i;
	for(i=0;i<num;i++)
	{
		if(i % 200 ==0)
			printf("%dsold,%drest\n",i,ticketCount);
		temp=ticketCount;
		//放弃CPU，强制切换到另外一个进程
		pthread_yield();
		temp=temp-1;
		ticketCount-=1;
		pthread_yield();
		ticketCount=temp;
	}
	return NULL;
}

void *RefundThread(void *arg)
{
	int num,temp;
	num=1000;
	int i;
	for(i=0;i<num;i++)
	{
		if(i % 200 ==0)
			printf("%dreturn，%drest\n",i,ticketCount);
		temp=ticketCount;
		pthread_yield();
		temp=temp+1;
		ticketCount+=1;
		pthread_yield();
		ticketCount=temp;
		pthread_yield();
	}
	return NULL;
}
int main(int argc,char *argv[])
{
	printf("start：%dtickets\n",ticketCount);
	pthread_t p1,p2;
	pthread_create(&p1,NULL,SaleThread,argv[1]);
	pthread_create(&p2,NULL,RefundThread,argv[2]);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("end：%dtickets\n",ticketCount);
	return 0;

}
