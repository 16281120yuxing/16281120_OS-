#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int pages;     //�����ڴ�ߴ�N
int firstPageFramePos; //����������ʼλ��p
int pageFrames; //�������а�����ҳ��e
int rateM;     //�������ƶ���m��ÿ����m��ҳ���������ʼλ��p +1��
float t;       //һ����Χ��0��1֮���ֵt
int seq[1000000];//ҳ���������
int Inside[50];//�����ڴ�
int memSize;   //�����ڴ�Ĵ�С
int number;    //ҳ��������еĸ���
int count ;    //�����ڴ��п�����
int lost[6];      //ȱҳ��

int state[1000];//clock�û��㷨�У��ڴ��е�ÿ��ҳ��Ŷ�Ӧ��״̬
int state2[1000][2];//�Ľ���clock�㷨�У�����λA���޸�λM
int insert = 0;//�ȵ��ȳ��û��㷨fcfo�б�ʾ ���ڴ�����ʱ���½����ҳ�ŷŵ�λ��

/*******************************************
*�������ƣ�GeneratingRandomSequence
*���ܣ�   ���ɷ��Ͼֲ��������Ե��������
*������   ��
*����ֵ�� ��
********************************************/
void GeneratingRandomSequence(){
	srand((unsigned)time(NULL));
	for(int k = 0; k < number; k++){
    	for (int i = 0; i < rateM; ++i)
		{
			//����rateM��ȡֵ��Χ��p��p + e��������������¼��ҳ��������д���
	    	int randomNum = (rand() % pageFrames) + firstPageFramePos;
	    	seq[i+k*rateM] = randomNum;
		//	printf("%d ",seq[i+k*rateM] );
		}
		//����һ�������r��0 �� r �� 1,���r < t����Ϊp����һ����ֵ������p = (p + 1) mod N
		float r;
        r = (rand()%10)*0.1f;
		if(pageFrames+firstPageFramePos<pages){
	    	if(r<t){
	     		firstPageFramePos = rand() % (pages-pageFrames);
			}else{
		    	firstPageFramePos = (firstPageFramePos+1)%pages;
			}
		}
	}
    printf("| ҳ�������������\n");

}
/*******************************************
*�������ƣ�clean
*���ܣ�   ��������ڴ棬�������ֱ�����ֵ��ʼ��
*������   ��
*����ֵ�� ��
********************************************/
void clean(){
	for(int i = 0; i< 6; i++){
		lost[i]=0;
	}
	count=0;
	for(int j = 0; j < memSize; j++){
		Inside[j] = 0;
		state[j] = 0;
		state2[i][0]=0;
        state2[i][1]=0;
	}
}

/*******************************************
*�������ƣ�isInside
*���ܣ�   ���ҳ���Ƿ��������ڴ���
*������   int index  �����ʵ�ҳ�����������
*                    ���������е�λ���±�
*����ֵ�� bool    true  �������ڴ���
*                 false ���������ڴ���
********************************************/
bool isInside(int index){
	for(int i = 0; i < memSize; i++){
		if(Inside[i] == seq[index]){
			state[i] = 1;
			return true;
		}
	}
	return false;
}

/*********************************************
*�������ƣ�OPT
*���ܣ�  ����û��㷨��OPT����ѡ������ʹ�û���
*        ���ʱ���ڲ��ٱ����ʣ����������
*        ʱ��Żᱻ���ʣ���ҳ����̭���ڴ�
*
*������  int num  �����ʵ�ҳ�����������������
*                 ���е�λ���±�
*����ֵ�� ��
**********************************************/
void OPT(int num){
	int max = 0; // ��ʾ�ڴ��е�ҳ�ţ���һ�γ��ֵľ���
	int maxchange; //��ʾ�ڴ����´γ��־�������ҳ�����ڴ��е�λ��
	int k;

	if(!isInside(num)){
		if(count == memSize){
			lost[0]++;
			for(int j=0; j < memSize; j++){
				for( k = num; k < rateM*number;k++){
					if(Inside[j] == seq[k])
					break;
				}
				if( k > max){
				  max = k;    //k��ʾ������ط����ٴγ��ָ���ҳ��
				  maxchange =j;//j ��ʾ�� �ڴ��е�j��Inside�е�ҳ����ڴ��ó������µ�ҳ�����
				}
			}
			Inside[maxchange] = seq[num];
		}
		else{

			Inside[count] = seq[num];
			count++;
		}
	}
}

/***********************************************
*�������ƣ�RAND
*���ܣ�  ����û��㷨��RAND��������һ��ȡֵ��Χ
*        ��0��N-1֮��������������������ɱ�ʾ
*        Ӧ����̭���ڴ��ҳ��
*
*������  int num  �����ʵ�ҳ�����������������
*                 ���е�λ���±�
*����ֵ�� ��
************************************************/
void RAND(int num){
	int randomNum;
    if(!isInside(num)){
		if(count == memSize){
			lost[1]++;
			randomNum = rand()%memSize;
			Inside[randomNum] = seq[num];
		}
		else{
			Inside[count] = seq[num];
			count++;
		}
	}
}

/***********************************************
*�������ƣ�FIFO
*���ܣ�  �Ƚ��ȳ��û��㷨��FIFO����ѡ�����Ƚ�����
*        �漴���ڴ�פ��ʱ����õ�ҳ�滻�������
*
*������  int num  �����ʵ�ҳ�����������������
*                 ���е�λ���±�
*����ֵ�� ��
************************************************/
void FIFO(int num){
	if(!isInside(num)){
		if(count == memSize){
			lost[2]++;
			Inside[insert] = seq[num];
			insert++;
			insert = insert % memSize;
		}
		else{
			Inside[count] = seq[num];
			count++;
		}
	}
}

/***********************************************
*�������ƣ�LRU
*���ܣ�  ������δʹ���û��㷨��LRU������"�����
*        ��ȥ"��Ϊ"����Ľ���"�Ľ��ƣ�ѡ�����һ
*        ��ʱ���ʱ��δ�����ʵ�ҳ����̭���ڴ�
*
*������  int num  �����ʵ�ҳ�����������������
*                 ���е�λ���±�
*����ֵ�� ��
************************************************/
void LRU(int num){
	int max = 0; // ��ʾ�ڴ��е�ҳ�ţ���һ�γ��ֵľ���
	int maxchange; //��ʾ�ڴ����ϴγ��־�������ҳ�����ڴ��е�λ��
	if(!isInside(num)){
		if(count == memSize){
			lost[3]++;
			for(int j=0; j < memSize; j++){
				for(int k = num; k >0;k--){
					if(Inside[j] == seq[k])
					break;
				}
				if(  num - k > max){
				  max = num - k;
				  maxchange =j;//j ��ʾ�� �ڴ��е�j��Inside�е�ҳ����ڴ��ó������µ�ҳ�����
				}
			}
			Inside[maxchange] = seq[num];
		}
		else{
			Inside[count] = seq[num];
			count++;
		}
	}
}

/***********************************************
*�������ƣ�CLOCK
*���ܣ�  ��Clock�û��㷨
*
*������  int num  �����ʵ�ҳ�����������������
*                 ���е�λ���±�
*����ֵ�� ��
************************************************/
void NRU(int num){
	int j;
	if(!isInside(num)){
		if(count == memSize){
			lost[4]++;
			for(j=0; j < memSize; ){
				if(state[j] == 0){
					break;
				}
				else{
					state[j] = 0;
				}
				j++;
				j = j %memSize;
			}
			Inside[j] = seq[num];
			state[j] = 1;
		}
		else{
			Inside[count] = seq[num];
			count++;
		}
	}
}
/*******************************************
*�������ƣ�isInside2
*���ܣ�   ���ڸĽ���clock�û��㷨�����ҳ��
*         �Ƿ����ڴ��У����ڣ����޸ķ���λ
*         ���޸�λ
*������   int num  �����ʵ�ҳ�����������
*                    ���������е�λ���±�
*����ֵ�� bool    true  �������ڴ���
*                 false ���������ڴ���
********************************************/
bool isInside2(int num){
	for(int i = 0; i < memSize; i++){
		if(Inside[i] == seq[num]){
			state2[i][0] = 1;  //����λ��1
			state2[i][1] = 0;  //û�б��޸�
			return true;
		}
	}
	return false;
}

/*******************************************
*�������ƣ�whichpage
*���ܣ�  ���ڸĽ���clock�û��㷨���ж��ڴ���
*        �ڼ�����Ҫ���û�
*������   int num  �����ʵ�ҳ�����������
*                  ���������е�λ���±�
*����ֵ�� int      �����ڴ���λ���±�
********************************************/
int whichpage(){
	int j;
	for(j=0; j < memSize;j++){
        if(state2[j][0] == 0&&state2[j][1] == 0){
			return j;
		}
	}
	for(j=0; j < memSize;j++ ){
        if(state2[j][0] == 0&&state2[j][1] == 1){
			return j;
		}
		state2[j][0] = 0 ;
	}
	for(j=0; j < memSize;j++ ){
		state2[j][0] = 0 ;
	}
	return whichpage();
}

/***********************************************
*�������ƣ�LCLOCK
*���ܣ�  �Ľ���Clock�û��㷨��
*    �� �Ӳ�Ѱָ�뵱ǰλ����ɨ���ڴ��ҳѭ�����У�
*       ѡ��A=0��M=0�ĵ�һ��ҳ����̭����δ�ҵ���
*       ת��
*    �� ��ʼ�ڶ���ɨ�裬ѡ��A=0��M=1�ĵ�һ��ҳ��
*       ��̭��ͬʱ������������ҳ�����λ��0������
*       ���ҵ���ת��
*
*������  int num  �����ʵ�ҳ�����������������
*                 ���е�λ���±�
*����ֵ�� ��
************************************************/
void LCLOCK(int num){
	int j;
	if(!isInside2(num)){
		if(count == memSize){
			lost[5]++;
			j =  whichpage();
			Inside[j] = seq[num];
			state2[j][0] = 1;
			state2[j][1] = 1;
		}
		else{
			Inside[count] = seq[num];
			count++;
		}
	}
}


int main(int argc, char* argv[])
{
    char c;
	printf("| 1.������ҳ��������еĸ���number��");
	scanf("%d",&number);
	scanf("%c",&c);
	printf("| 2.�����������ڴ�ߴ�N��");
	scanf("%d",&pages);
	scanf("%c",&c);
	printf("| 3.�����������ڴ�Ĵ�СmemSize��");
	scanf("%d",&memSize);
	scanf("%c",&c);
	printf("| 4.�����ù���������ʼλ��p��");
	scanf("%d",&firstPageFramePos);
	scanf("%c",&c);
    printf("| 5.�����ù������а�����ҳ��e��");
    scanf("%d",&pageFrames);
	scanf("%c",&c);
	printf("| 6.�����ù������ƶ���m��");
	scanf("%d",&rateM);
	scanf("%c",&c);
	printf("| 7.������һ����Χ��0��1֮���ֵt��");
	scanf("%f",&t);
	scanf("%c",&c);
	printf("| ------------------------------------------------------------\n");
	printf("| ����������Ϣ                                                \n");
	GeneratingRandomSequence();
	printf("| ------------------------------------------------------------\n");
	printf("| 1 ����û��㷨��OPT��\n");
	printf("| 2 ����û��㷨��RAND��\n");
	printf("| 3 �Ƚ��ȳ��û��㷨��FIFO��\n");
    printf("| 4 ������δʹ���û��㷨��LRU��\n");
	printf("| 5 ��Clock�û��㷨��NRU��\n");
	printf("| 6 �Ľ���Clock�û��㷨��LCLOCK��\n");
	printf("| ------------------------------------------------------------\n");
    printf("| ʵ��������\n");
	printf("| ------------------------------------------------------------\n");
    printf("| ��  �� |  OPT  |  RAND |  FIFO |  LRU  |  NRU  | LCLOCK |\n");
	printf("| ------------------------------------------------------------\n");
	printf("| ȱҳ�� |");
    for(int i = 0; i < 6; i++){
		clean();
		for(int j = 0; j < rateM*number; j++){
			switch(i){
			case 0:
				OPT(j);
				break;
			case 1:
				RAND(j);
				break;
			case 2:
				FIFO(j);
				break;
			case 3:
				LRU(j);
				break;
			case 4:
				NRU(j);
				break;
			case 5:
				LCLOCK(j);
				break;

			}
		}
		printf(" %.2f%%|",(double)lost[i]/(rateM*number)*100);
	}
	printf("\n");
	printf("| ------------------------------------------------------------\n");
    getch();
	return 0;
}
