#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
volatile int ticketCount=1000;
sem_t *flag=NULL;

//买票
void *SaleThread(void *arg)
{
	int num,temp;
	num=1000;
	for(int i=0;i<num;i++)
	{
		//每200张输出一次
		if(i % 200 ==0)
			printf("%dsold，%drest\n",i,ticketCount);
		sem_wait(flag);
		temp=ticketCount;
		pthread_yield();//放弃CPU，切换到另一进程
		temp=temp-1;
		ticketCount-=1;
		pthread_yield();
		ticketCount=temp;
		pthread_yield();
		sem_post(flag);
	}
	return NULL;
}

//退票
void *RefundThread(void *arg)
{
	int num,temp;
	num=1000;
	for(int i=0;i<num;i++)
	{
		//每200张输出一次
		if(i % 200 ==0)
			printf("return%d，%drest\n",i,ticketCount);
		sem_wait(flag);
		temp=ticketCount;
		pthread_yield();
		temp=temp+1;
		ticketCount+=1;
		pthread_yield();
		ticketCount=temp;
		pthread_yield();
		sem_post(flag);
	}
	return NULL;
}
int main(int argc,char *argv[])
{

	flag=sem_open("flag",O_CREAT,0666,1);
	printf("start：%dtickets\n",ticketCount);
	pthread_t p1,p2;
	pthread_create(&p1,NULL,SaleThread,argv[1]);
	pthread_create(&p2,NULL,RefundThread,argv[2]);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("end：%dtickets\n",ticketCount);
	sem_close(flag);
	sem_unlink("flag");
	return 0;

}
