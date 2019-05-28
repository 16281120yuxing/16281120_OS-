// main2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<stdlib.h>
#include<time.h>
#define N 20      //�����ڴ�ߴ�
using namespace std;

int P;  
int const blockCount=3 ;//�ڴ��е��������
int count = 0;
int block[blockCount];
int const PageCount=15;//�ܵ�ҳ����
int Page[PageCount];
int state[blockCount];//clock�û��㷨�У��ڴ��е�ÿ��ҳ��Ŷ�Ӧ��״̬
int state2[blockCount][2];// ��ά���飬��һ�е�һ��Ϊ����λ����һ�еĵڶ���Ϊ�޸�λ
double lost= 0.0;

void generate_list(int *list,int e,int m,int t)
{
	int i,j=0,q=P,r;
	srand((unsigned)time(NULL));
	while(j<e)
	{
		for(i=j;i<j+m;i++)
		{
			if(i==e)
				break;
			list[i]=(q+rand()%e)%N;   //��֤�������ڴ��ҳ����
		}
		j=i;
		r=rand()%100;
		if(r<t)
			q=rand()%N;
		else 
			q=(q+1)%N;
	}
}

//��������Ƿ��޸ĵ������prop(0����100),prop/100�ĸ���Ϊ���޸�
void generate_modify(int *mo,int e,int prop)
{
	int i,t;
	for(i=0;i<e;i++)
	{
		t=rand()%100;
		if(t>prop)
			mo[i]=0;
		else
			mo[i]=1;
	}
}


//���ҳ���Ƿ����ڴ���
bool inblock(int num)
{
	for(int i=0; i<blockCount;i++)
	{
		if(block[i] == Page[num])
		{
			state[i] = 1;
			return true;
		}
	}
	return false;
}

//�ж�ҳ���Ƿ��Ѿ����޸�
bool change()
{
	if((rand()%2+1) == 1 )
	{
		printf("��ҳ�汻�޸�!\n");
		return true;
	}
	else
		return false;
}

//���ڸĽ���clock�û��㷨�����ҳ���Ƿ����ڴ��в��ѷ���λ���޸�λ��1
bool inblock2(int num)
{
	for(int i=0;i<blockCount;i++){
		if(block[i] == Page[num]){
			if(change()){
				state2[i][0] = 1;
				state2[i][1] = 1;
			}
			else{
				state2[i][0] = 1;
			}
			return true;
		}
	}
	return false;
}

//���ڸĽ���clock�û��㷨���ж��ڴ��еڼ�����Ҫ���û�
int whichpage(){

	int j;

	for(j=0;j<blockCount;j++)
	{
        if(state2[j][0] == 0&&state2[j][1] == 0)
		{
			return j;
		}
	}
	for(j=0;j<blockCount;j++ )
	{
        if(state2[j][0] == 0&&state2[j][1] == 1)
		{
			return j;
		}
		state2[j][0] = 0 ;
	}
	for(j=0;j<blockCount;j++ )
	{
		state2[j][0] =0 ;
	}
	return whichpage();
}


//��Clock�û��㷨
void CLOCK(int num)
{
	int j;

	if(inblock(num))
	{
		printf("����!\n");
		lost++;
		for(int i=0;i<blockCount;i++)
        printf("�����%d#������:%d\n",i,block [i]);
	}
	else
		if(count == blockCount)
		{
			//lost++;
			for(j=0;j<blockCount; )
			{
				if(state[j] == 0)
				{
					break;
				}
				else{
					state[j] = 0;
				}
				j++;
				j = j%3;
			}
			block[j] = Page[num];
			state[j] = 1;
			for(int i=0;i<blockCount;i++)
            printf("�����%d#������:%d\n",i,block[i]);
		}
		else{
			block[count] = Page[num];
			count++;
			for(int i=0;i<blockCount;i++)
            printf("�����%d#������:%d\n",i,block[i]);
		}
}

//�Ľ���clock�û��㷨
void LCLOCK(int num)
{
	int j;

	if(inblock2(num))
	{
		printf("����!\n");
        lost++;
		for(int i=0;i<blockCount;i++)
        printf("�����%d#������:%d\n",i,block[i]);
	}
	else
		if(count == blockCount)
		{
	     	//lost++;
			j =  whichpage();
			block[j] = Page[num];
			state2[j][0] = 1;
			for(int i=0;i<blockCount;i++)
            printf("�����%d#������:%d\n",i,block[i]);
		}
		else{
			block[count] = Page[num];
			count++;
			for(int i=0;i<blockCount;i++)
            printf("�����%d#������:%d\n",i,block[i]);
		}
}

int main()
{
    int a[N];
	int mo[N];
    int A=10;
    int e,m,prop,t,j;
	printf("ҳ������Ϊ��");
	generate_list(a, e,m,t);
	generate_modify(mo,e,prop);

    for(int i = 0;i<PageCount;i++)
	{

	    Page[i] =rand()%9 + 1;
		printf("%d ",Page[i]);	
	}
	char ch ;
	printf("\n");
	printf("\t\t1 ��Clock��̭�㷨\n");
	printf("\t\t2 �Ľ���Clock��̭�㷨\n");
	printf("�������㷨���:\t\n");
    while(1){	
    scanf("%c",&ch);
	 switch(ch){
     case '1':{
			lost=0;
			count=0;
			for(int m=0;m<blockCount;m++)
			{
				state[m] = 0;
				}
				for(int j=0;j<blockCount;j++)
				{
					block[j]=0;
				}
				for(int i=0;i<PageCount;i++)
				{
                    printf("����Page[%d]\n",i);
                    CLOCK(i);
	            }
			 printf("ҳ����ʴ���: %d\nȱҳ����: %0.lf\n",PageCount,PageCount-lost);
			 printf("ȱҳ��Ϊ:%0.001f\n",(PageCount-lost)/PageCount);
             printf("\n�������㷨���:\t");  
    }break;
    case '2':{
            lost = 0;
			count = 0;
			for(int m = 0; m < blockCount; m++)
			{
                for(int n = 0; n < 2;n++)
				state2[m][n] = 0;
			}
			for(int j = 0; j < blockCount; j++)
			{
                block[j] = 0;
			}
			for(int i = 0; i < PageCount; i++)
			{
                printf("����Page[%d]\n",i);
                LCLOCK(i);
            }
			printf("ҳ����ʴ���: %d\nȱҳ����: %0.lf\n",PageCount,PageCount-lost);
			printf("ȱҳ��Ϊ:%0.001f\n",(PageCount-lost)/PageCount);
            printf("\n�������㷨���:\t");   
    }break;
	case '3':{
		exit(0);
			 }
	}
}
  return 0;
}


