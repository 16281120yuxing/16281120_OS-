#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>
int main()
{
	sem_t *P1_signal,*P2_signal,*P3_signal,*P23_signal;
	//主函数中的进程是P1
	pid_t p2,p3,p4; 
	//创建信号量
	P1_signal=sem_open("P1_signal",O_CREAT,0666,1);
	P2_signal=sem_open("P2_signal",O_CREAT,0666,0);
	P3_signal=sem_open("P3_signal",O_CREAT,0666,0);
	sem_wait(P1_signal);
	p2=fork();//创建进程P2
	if(p2==0)
	{
		sem_wait(P1_signal); //等待p1
		printf("I am the process P2!\n");
		sem_post(P2_signal);
		sem_post(P1_signal);
	}
	if(p2>0)
	{
		p3=fork();
		if(p3==0)
		{
			sem_wait(P1_signal);
			printf("I am the process P3!\n");
			sem_post(P3_signal);
			sem_post(P1_signal);
		}
		if(p3>0)
		{
			printf("I am the process P1!\n");
			sem_post(P1_signal);
			p4=fork();
			if(p4==0)
			{
				sem_wait(P2_signal);
				sem_wait(P3_signal);
				sem_wait(P1_signal);
				printf("I am the process P4!\n");
				sem_post(P2_signal);
				sem_post(P3_signal);
				sem_post(P1_signal);
			}
		}
	}
	sem_close(P1_signal);
	sem_close(P23_signal);
	sem_close(P2_signal);
	sem_close(P3_signal);
	sem_unlink("P1_signal");
	sem_unlink("P23_signal");
	sem_unlink("P2_signal");
	sem_unlink("P3_signal");
	sleep(0.5);
	return 0;


}
