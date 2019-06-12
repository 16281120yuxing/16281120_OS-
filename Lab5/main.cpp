#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define NULL 0

typedef struct mdf{//MDF�ṹ��
	char username[20];//�û���
	char filename[20];//�ļ���
	struct mdf *next;
}MDF;

typedef struct ufd{//UFD�ṹ��
	char filename[20];//�ļ���
	int protect;//�ļ�������
	unsigned int length;//�ļ�����
	struct ufd *next;
}UFD;

typedef struct afd{//AFD�ṹ��
	char filename[20];//�ļ���
	int protect;//�ļ�������
	unsigned int point;//�ļ���дָ��
	struct afd *next;
}AFD;


MDF *pmdf;//ȫ������ͷָ��
UFD *pufd;
AFD *pafd;
char UserUFD[20];//�Ѿ���½�ɹ����û���


void initMDF()//��ʼ��MDF��
{
	FILE *fp;
	pmdf= (MDF*)malloc(sizeof(MDF));
	MDF *p = pmdf;
	if((fp = fopen("MDF", "r+")) == NULL){//��MDF�ļ�
		puts("the MDF cannot open!\n");
		exit(1);
	}
	while (!feof(fp)){//��MDF�ļ��е�����װ������
		p->next = (MDF*)malloc(sizeof(MDF));
		p = p->next;
		fscanf(fp, "%s", p->username);
		fscanf(fp, "%s", p->filename);
	}
	p->next = NULL;
	fclose(fp);
}

void printUFD()//��ӡMDF��
{
	UFD *p = pufd->next;
	puts("�ļ���\t\t������\t\t�ļ�����\n");
	while (p){
		printf("%s", p->filename);
		printf("\t\t%d" , p->protect);
		printf("\t\t%d\n", p->length);
		p=p->next;
	}


}
void initUFD(char *name)//��ʼ��UFD��
{
	FILE *fp;
	pufd= (UFD*)malloc(sizeof(UFD));
	UFD *p = pufd;
	if((fp = fopen(name, "r+")) == NULL){
		puts("the UFD cannot open!\n");
		exit(1);
	}
	while (!feof(fp)){//����UFD����

		p->next = (UFD*)malloc(sizeof(UFD));
		p = p->next;
		fscanf(fp, "%s", p->filename);
		fscanf(fp, "%d", &p->protect);
		fscanf(fp, "%d", &p->length);
		fgetc(fp);

	}
	p->next = NULL;
	fclose(fp);


}

int checkuser()//����½���û���
{
	char username[20];
	while(1){
		puts("�������û���: \n");
		scanf("%s", username);
		MDF *p = pmdf;
		while(p){
			if(strcmp(username, p->username) == 0){
				strcpy(UserUFD, p->filename);
				initUFD(p->filename);
				printUFD();
				return 1;
			}
			p= p->next;
		}
		puts("ͬ����������\n");

	}
}

void initAFD()//��ʼ��AFD
{
	pafd = (AFD*)malloc(sizeof(AFD));
	pafd->next = NULL;
}

bool create()//�����ļ�����
{
	char filename[20];
	UFD *p = pufd->next;
	AFD *pa = pafd;
	puts("������Ҫ�������ļ���: \n");
	scanf("%s", filename);
	while (p){
		if(strcmp(filename, p->filename) == 0){
			puts("���ļ��Ѿ�������!\n");
			return 0;
		}
		if(!p->next)
			break;
		p= p->next;
	}
	p->next = (UFD*)malloc(sizeof(UFD));
	p=p->next;
	strcpy(p->filename, filename);
	p->protect = 2;
	p->length = 0;
	p->next = NULL;

	while(pa->next){//�����ļ�����뵽AFD
		pa=pa->next;
	}
	pa->next = (AFD*)malloc(sizeof(AFD));
	pa = pa->next;
	strcpy(pa->filename ,filename);
	pa->protect = 2;
	pa->point = 0;
	pa->next = NULL;


	return 1;
}

bool _delete()//ɾ���ļ�����
{
	char filename[20];
	puts("������Ҫɾ�����ļ���: \n");
	scanf("%s", filename);
	UFD *p = pufd;
	UFD *temp;
	while (p->next){
		if(strcmp(filename, p->next->filename) == 0){
			temp = p->next;
			p->next = p->next->next;
			free(temp);
			printf("�ļ�%sɾ���ɹ�!\n", filename);
			return 1;
		}
		p= p->next;
	}
	puts("Ҫɾ�����ļ�������!\n");
	return 0;

}

bool open()//���ļ�����
{
	char filename[20];
	unsigned int protect;
	puts("������Ҫ�򿪵��ļ���: \n");
	scanf("%s", filename);

	puts("������Ҫ�򿪵��ļ���������: \n");
	scanf("%d", &protect);

	UFD *p = pufd->next;
	AFD *pa = pafd->next;
	while (pa){
		if(strcmp(filename, pa->filename) == 0){
			printf("�ļ�%s�Ѿ���!\n",filename);
			return 1;
		}
		if(!pa->next)
			break;
		pa = pa->next;
	}

	if(!pa)
		pa=pafd;
	while (p){
		if(strcmp(filename, p->filename) == 0){
			pa->next = (AFD*)malloc(sizeof(AFD));
			pa = pa->next;
			strcpy(pa->filename , p->filename);
			pa->protect = protect;
			if(protect == 1)
				pa->point = 0;
			else
				pa->point = p->length;
			pa->next=NULL;
			printf("�ļ�%s�Ѿ���!\n",filename);
			return 1;
		}
		p= p->next;
	}
	puts("Ҫ�򿪵��ļ�������!\n");
	return 0;

}

void close()//�ر��ļ�����
{
	char filename[20];
	UFD *pu = pufd->next;
	puts("������Ҫ�رյ��ļ���: \n");
	scanf("%s", filename);
	AFD *p = pafd;
	AFD *temp;
	while (p&&p->next){
		if(strcmp(filename, p->next->filename) == 0){
			temp = p->next;
			p->next = p->next->next;

			if(temp->protect == 2){
				while(pu){
					if(strcmp(temp->filename, pu->filename) == 0){
						pu->length =temp->point;
						break;
					}
					pu = pu->next;
				}
			}


			free(temp);
			printf("�ļ�%s�رճɹ�!\n", filename);
			return ;
		}
		p= p->next;
	}
		puts("Ҫ�رյ��ļ�û�б���!\n");
}

int read()//���ļ�����
{
	char filename[20];
	unsigned int length;
	AFD *p = pafd->next;
	puts("������Ҫ�����ļ���: \n");
	scanf("%s", filename);
	puts("������Ҫ���ĳ���\n");
	scanf("%d", &length);

	while (p){
		if(strcmp(filename, p->filename) == 0){
			p->point += length;
			printf("�ļ�%s��ȡ�ɹ�!\n", filename);
			return 1;
		}
		p= p->next;
	}
	puts("��ȡʧ���ļ�û�д򿪹�!\n");
	return 0;
}

int write()//д�ļ�����
{
	char filename[20];
	unsigned int length;
	AFD *p = pafd->next;
	puts("������Ҫд���ļ���: \n");
	scanf("%s", filename);
	while (p){
		if(strcmp(filename, p->filename) == 0){
			if(p->protect != 2){
				printf("�ļ�%s����д!\n", filename);
				return 0;
			}
			puts("������Ҫд�ĳ���\n");
			scanf("%d", &length);
			p->point += length;
			printf("�ļ�%sд��ɹ�!\n", filename);
			return 1;
		}
		p= p->next;
	}
	puts("д��ʧ���ļ�û�д򿪹�!\n");
	return 0;


}

void destroy()//�ͷ��ڴ�
{
	MDF *pm = pmdf;
	while(pm){
		pmdf = pmdf->next;
		free(pm);
		pm = pmdf;
	}
	AFD *pa = pafd;
	while(pa){
		pafd = pafd->next;
		free(pa);
		pa = pafd;
	}
	UFD *pu = pufd;
	while(pu){
		pufd = pufd ->next;
		free(pu);
		pu = pufd;
	}
}

void saveUFD()//����UFD�ļ�
{
	FILE *fp;
	UFD *p = pufd->next;
	if((fp = fopen(UserUFD, "w")) == NULL){
		puts("the UFD cannot open!\n");
		exit(1);
	}
	while (p){
		fprintf(fp, "%s", "\n");
		fprintf(fp, "%s%s", p->filename,"\t\t");
		fprintf(fp, "%d%s", p->protect,"\t\t");
		fprintf(fp, "%d", p->length);
		p=p->next;
	}

	fclose(fp);

}

void exit()//�Ƴ�ϵͳ
{
	AFD *pa = pafd->next;
	UFD *pu = pufd->next;
	while(pa){
		if(pa->protect == 2){
			while(pu){
				if(strcmp(pa->filename, pu->filename) == 0){
					pu->length = pa->point;
					break;
				}
				pu = pu->next;
			}
		}
		pa =pa->next;

	}

	saveUFD();
	printUFD();
	destroy();



}

void operate()//����ʶ��
{
	while(1){
		char command[20];
		char name[][8] = {"create", "delete", "open", "close","read", "write","exit"};
		puts("����������: \n");
		scanf("%s", command);
		if(strcmp(command, name[0] ) == 0)
			create();
		else if(strcmp(command, name[1] ) == 0)
			_delete();
		else if(strcmp(command, name[2] ) == 0)
			open();
		else if(strcmp(command, name[3] ) == 0)
			close();
		else if(strcmp(command, name[4] ) == 0)
			read();
		else if(strcmp(command, name[5] ) == 0)
			write();
		else if(strcmp(command, name[6] ) == 0){
			exit();
			return;
		}else
			puts("��Ч�������������:\n");
	}


}

void print()
{
	puts("�ļ�ϵͳ\n");
	puts("***ʹ��˵��***:\n");
	puts("���ļ�ϵͳ���������û��ֱ���user1 user2 user3\n\
	  ϵͳ������create, delete, open, close,read, write,exit\
	  \nexit----------------------------------------�˳�ϵͳ");
}

int main()
{
	print();
	initMDF();
	checkuser();
	initAFD();
	operate();//����ʶ��

	return 0;

}
