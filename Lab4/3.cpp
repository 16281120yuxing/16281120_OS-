#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int pages;     //虚拟内存尺寸N
int firstPageFramePos; //工作集的起始位置p
int pageFrames; //工作集中包含的页数e
int rateM;     //工作集移动率m（每处理m个页面访问则将起始位置p +1）
float t;       //一个范围在0和1之间的值t
int seq[1000000];//页面访问序列
int Inside[50];//物理内存
int memSize;   //物理内存的大小
int number;    //页面访问序列的个数
int count ;    //物理内存中空余量
int lost[6];      //缺页数

int state[1000];//clock置换算法中，内存中的每个页面号对应的状态
int state2[1000][2];//改进型clock算法中，访问位A和修改位M
int insert = 0;//先到先出置换算法fcfo中表示 当内存满的时候，新进入的页号放的位置

/*******************************************
*函数名称：GeneratingRandomSequence
*功能：   生成符合局部访问特性的随机序列
*参数：   无
*返回值： 无
********************************************/
void GeneratingRandomSequence(){
	srand((unsigned)time(NULL));
	for(int k = 0; k < number; k++){
    	for (int i = 0; i < rateM; ++i)
		{
			//生成rateM个取值范围在p和p + e间的随机数，并记录到页面访问序列串中
	    	int randomNum = (rand() % pageFrames) + firstPageFramePos;
	    	seq[i+k*rateM] = randomNum;
		//	printf("%d ",seq[i+k*rateM] );
		}
		//生成一个随机数r，0 ≤ r ≤ 1,如果r < t，则为p生成一个新值，否则p = (p + 1) mod N
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
    printf("| 页面访问序列生成\n");

}
/*******************************************
*函数名称：clean
*功能：   清空物理内存，并将部分变量的值初始化
*参数：   无
*返回值： 无
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
*函数名称：isInside
*功能：   检测页号是否在物理内存中
*参数：   int index  被访问的页面在随机访问
*                    序列数组中的位置下标
*返回值： bool    true  在物理内存中
*                 false 不在物理内存中
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
*函数名称：OPT
*功能：  最佳置换算法（OPT）。选择永不使用或是
*        在最长时间内不再被访问（即距现在最长
*        时间才会被访问）的页面淘汰出内存
*
*参数：  int num  被访问的页面在随机访问序列数
*                 组中的位置下标
*返回值： 无
**********************************************/
void OPT(int num){
	int max = 0; // 表示内存中的页号，下一次出现的距离
	int maxchange; //表示内存中下次出现距离最大的页号在内存中的位置
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
				  max = k;    //k表示在这个地方会再次出现给定页面
				  maxchange =j;//j 表示把 内存中第j个Inside中的页面从内存拿出，把新的页面放入
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
*函数名称：RAND
*功能：  随机置换算法（RAND）。产生一个取值范围
*        在0和N-1之间的随机数，该随机数即可表示
*        应被淘汰出内存的页面
*
*参数：  int num  被访问的页面在随机访问序列数
*                 组中的位置下标
*返回值： 无
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
*函数名称：FIFO
*功能：  先进先出置换算法（FIFO）。选择最先进入内
*        存即在内存驻留时间最久的页面换出到外存
*
*参数：  int num  被访问的页面在随机访问序列数
*                 组中的位置下标
*返回值： 无
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
*函数名称：LRU
*功能：  最近最久未使用置换算法（LRU）。以"最近的
*        过去"作为"最近的将来"的近似，选择最近一
*        段时间最长时间未被访问的页面淘汰出内存
*
*参数：  int num  被访问的页面在随机访问序列数
*                 组中的位置下标
*返回值： 无
************************************************/
void LRU(int num){
	int max = 0; // 表示内存中的页号，上一次出现的距离
	int maxchange; //表示内存中上次出现距离最大的页号在内存中的位置
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
				  maxchange =j;//j 表示把 内存中第j个Inside中的页面从内存拿出，把新的页面放入
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
*函数名称：CLOCK
*功能：  简单Clock置换算法
*
*参数：  int num  被访问的页面在随机访问序列数
*                 组中的位置下标
*返回值： 无
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
*函数名称：isInside2
*功能：   用于改进型clock置换算法，检测页号
*         是否在内存中，若在，则修改访问位
*         和修改位
*参数：   int num  被访问的页面在随机访问
*                    序列数组中的位置下标
*返回值： bool    true  在物理内存中
*                 false 不在物理内存中
********************************************/
bool isInside2(int num){
	for(int i = 0; i < memSize; i++){
		if(Inside[i] == seq[num]){
			state2[i][0] = 1;  //访问位置1
			state2[i][1] = 0;  //没有被修改
			return true;
		}
	}
	return false;
}

/*******************************************
*函数名称：whichpage
*功能：  用于改进型clock置换算法，判断内存中
*        第几个需要被置换
*参数：   int num  被访问的页面在随机访问
*                  序列数组中的位置下标
*返回值： int      物理内存中位置下标
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
*函数名称：LCLOCK
*功能：  改进型Clock置换算法。
*    ① 从查寻指针当前位置起扫描内存分页循环队列，
*       选择A=0且M=0的第一个页面淘汰；若未找到，
*       转②
*    ② 开始第二轮扫描，选择A=0且M=1的第一个页面
*       淘汰，同时将经过的所有页面访问位置0；若不
*       能找到，转①
*
*参数：  int num  被访问的页面在随机访问序列数
*                 组中的位置下标
*返回值： 无
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
	printf("| 1.请设置页面访问序列的个数number：");
	scanf("%d",&number);
	scanf("%c",&c);
	printf("| 2.请设置虚拟内存尺寸N：");
	scanf("%d",&pages);
	scanf("%c",&c);
	printf("| 3.请设置物理内存的大小memSize：");
	scanf("%d",&memSize);
	scanf("%c",&c);
	printf("| 4.请设置工作集的起始位置p：");
	scanf("%d",&firstPageFramePos);
	scanf("%c",&c);
    printf("| 5.请设置工作集中包含的页数e：");
    scanf("%d",&pageFrames);
	scanf("%c",&c);
	printf("| 6.请设置工作集移动率m：");
	scanf("%d",&rateM);
	scanf("%c",&c);
	printf("| 7.请设置一个范围在0和1之间的值t：");
	scanf("%f",&t);
	scanf("%c",&c);
	printf("| ------------------------------------------------------------\n");
	printf("| 保存设置信息                                                \n");
	GeneratingRandomSequence();
	printf("| ------------------------------------------------------------\n");
	printf("| 1 最佳置换算法（OPT）\n");
	printf("| 2 随机置换算法（RAND）\n");
	printf("| 3 先进先出置换算法（FIFO）\n");
    printf("| 4 最近最久未使用置换算法（LRU）\n");
	printf("| 5 简单Clock置换算法（NRU）\n");
	printf("| 6 改进型Clock置换算法（LCLOCK）\n");
	printf("| ------------------------------------------------------------\n");
    printf("| 实验结果如下\n");
	printf("| ------------------------------------------------------------\n");
    printf("| 算  法 |  OPT  |  RAND |  FIFO |  LRU  |  NRU  | LCLOCK |\n");
	printf("| ------------------------------------------------------------\n");
	printf("| 缺页率 |");
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
