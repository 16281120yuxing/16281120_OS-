#include<iostream>
#include<fstream>

using namespace std;

#define BlockSize 10
#define PageSize 100

int page[PageSize]; //页面数组存放页面
int block[BlockSize]; //物理块数组
int result[PageSize][BlockSize]; //存放页面和物理块二维数组
int pSize = 0; //用户使用页面数
int bSize = 0; //用户使用物理块数
int blockFlag[BlockSize]; //用于 LRU 与最佳置换算法中，辅助判断该换出的页面
int noPageCount = 0; //缺页次数


//输入数据
void inputData()
{
	cout<<endl<<"请输入物理块数(1<=bSize<="<<BlockSize<<')'<<endl;
	cin>>bSize;
	cout<<"请输入页面数 (1<=pSize<="<<PageSize<<')'<<endl;
	cin>>pSize;
	while(bSize<=0||bSize>BlockSize||pSize<=0||pSize>PageSize)
	{
		cout<<"输入范围错误，请重新输入： "<<endl;
		cout<<"请输入物理块数(1<=F<="<<BlockSize<<')';
		cin>>bSize;
		cout<<endl<<"请输入页面数 (1<=p<="<<PageSize<<')';
		cin>>pSize;
	}
	cout<<"请输入页面走向"<<endl;
	for(int i = 0;i <pSize;i++)
		cin>>page[i];
}


//初始化 page 数组
void initPage()
{
	for(int i = 0;i<PageSize;i++)
		page[i] = -1;
}


//初始化 block 与 result 数组
void initBlockResult()
{
	int i = 0;
	for(i = 0;i<BlockSize;i++)
		block[i] = -1;
	for(i = 0;i < PageSize;i++)
		for(int j = 0; j < BlockSize;j++)
			result[i][j] = -1;
}


//查找物理块中是否存在要调用的页面
int Exist(int i)
{
	for(int j = 0;j < bSize;j++)
		if(block[j] == i)
			return j;
	return -1;
}


//显示结果
void display(int noPageCount)
{
	for(int i =0 ;i < pSize;i++)
	{
		cout<<" "<<page[i]<<" ";
		for(int j = 0;j < bSize;j++)
		{
			if(result[i][j] == -1) break;
			else cout<<'['<<result[i][j]<<']'<<' ';
		}

		cout<<endl;
	}
	cout<<"____________________________________"<<endl;
	cout<<endl<<"缺页次数： "<<noPageCount-3<<endl;
	cout<<"缺页率： "<<(((double)noPageCount-3)/pSize)*100<<'%'<<endl;
	cout<<"===================================="<<endl;
}


//最佳置换算法 OPT
void OPT()
{
	int i = 0,j = 0;
	int position = 0,noPageCount = 0;
	int pageFlag = 0,resultFlag = 0; //页面标记（下标）指向下一个页面,结果标记表示结果的行， 即 result 数组的行标
	for(i = 0;i < BlockSize;i++)
		blockFlag[i] = 0;
	while(pageFlag < pSize)
	{
		if(Exist(page[pageFlag]) != -1) //判断页面是否已经存在
		    resultFlag++;
		else{
			if(position < bSize) //判断有无空闲物理块
			{ //若有则将页面放入空闲块
			    block[position] = page[pageFlag];
				position++;
				noPageCount++;
				for(i = 0;i < position;i++)
					result[resultFlag][i] = block[i];
				resultFlag++;
			}else{
				for(i = 0;i < bSize;i++)
				{
					for(j = pageFlag+1;j < pSize;j++)
					{
						if(block[i] == page[j])
						{
							blockFlag[i] = j;
							break;
						}
					}
					if(j == pSize)
						blockFlag[i] = 999;
				}
				int optPos = 0,max = blockFlag[0];
				for(int i = 0;i < bSize;i++)
					if(max < blockFlag[i])
					{
						max = blockFlag[i];
						optPos = i;
					}
					block[optPos] = page[pageFlag];
					noPageCount++;
					for(i = 0;i < bSize;i++)
						result[resultFlag][i] = block[i];
					resultFlag++;
				}
			}
			pageFlag++;
	}
	cout<<endl<<"最佳淘汰算法:"<<endl;
	display(noPageCount);
	return;
}


//先进先出页面置换算法 FIFO
void FIFO()
{
	int blockFlag = 0,pageFlag = 0,resultFlag = 0; //物理块标记，确定该换出的物理块下标
	int i = 0,j = 0,noPageCount = 0;
	int position = 0; //指示物理块，查找有无空闲
	while (pageFlag < pSize)
	{
		if(Exist(page[pageFlag]) != -1)
			resultFlag++;
		else{
			if(position < bSize)
			{
				block[position] = page[pageFlag];
				position++;
				noPageCount++;
				for(i = 0;i <= position;i++)
					result[resultFlag][i] = block[i];
				resultFlag++;
			}else{
				block[blockFlag] = page[pageFlag]; //blockFlag 指示要换出的页面
				noPageCount++;
				for(i = 0;i < bSize;i++)
					result[resultFlag][i] = block[i];
				resultFlag++;
				blockFlag++;
				blockFlag = blockFlag % bSize;
				}
			}
			pageFlag++;
	}
	cout<<endl<<"先进先出淘汰算法 FIFO:"<<endl;
	display(noPageCount);
	return;
}


//最近最久未用算法 LRU
void LRU()
{
	int i = 0,noPageCount = 0;
	int pageFlag = 0,resultFlag = 0,position = 0;
	for(i = 0;i < BlockSize;i++) //初始化时间记录数组
	    blockFlag[i] = 0;
	while(pageFlag < pSize)
	{
		if(Exist(page[pageFlag]) != -1)
		{ //判断页面是否已经在主存中
	        resultFlag++;
			blockFlag[Exist(page[pageFlag])] = 0; //若在则将时间记录数组对应位置为 0
		}else{
			if(position < bSize)
			{
				block[position] = page[pageFlag];
				blockFlag[position] = 0;
				position++;
				noPageCount++;
				for(i = 0;i <= position;i++)
					result[resultFlag][i] = block[i];
				resultFlag++;
			}else{
				int last = 0,min = blockFlag[0];
				for(int i = 0;i < bSize;i++)
					if(min < blockFlag[i])
					{
						min = blockFlag[i];
						last = i;
					}
					block[last] = page[pageFlag]; //置换最久未用页面
					blockFlag[last] = 0;
					noPageCount++;
					for(i = 0;i < bSize;i++)
						result[resultFlag][i] = block[i];
					resultFlag++;
			}
			}
			for(i = 0;i < bSize;i++)
				blockFlag[i]++;
			pageFlag++;
	}
	cout<<endl<<"最近最久未使用淘汰算法 LRU:"<<endl;
	display(noPageCount);
	return;
}


//时钟(clock)置换算法
void clock()
{
	int i = 0,position = 0,noPageCount = 0;
	bool boolBlockFlag[BlockSize];
	int flag = 0; //访问位循环标记
	int pageFlag = 0,resultFlag = 0;
	while(pageFlag < pSize)
	{
		if(Exist(page[pageFlag]) != -1)
		{
			resultFlag++;
			boolBlockFlag[Exist(page[pageFlag])] = true;
		}else{
			if(position < bSize)
			{
				block[position] = page[pageFlag];
				noPageCount++;
				boolBlockFlag[position] = true;
				position++;
				for(i = 0;i < position;i++)
					result[resultFlag][i] = block[i];
				resultFlag++;
			}else{
				while(true)
				{ //无限循环，找出访问位为 false的页面
			        if(boolBlockFlag[flag] == false) break;
					boolBlockFlag[flag] = false; //若为 true，置为 false
					flag++;flag = flag%bSize;
				}
				block[flag] = page[pageFlag];
				boolBlockFlag[flag] = true;
				flag++;
				flag = flag%bSize;
				noPageCount++;
				for(i = 0;i < position;i++)
					result[resultFlag][i] = block[i];
				resultFlag++;
			}
		}
		pageFlag++;
	}
	cout<<endl<<"时钟(clock)置换算法:"<<endl;
	display(noPageCount);
	return;
}


int main()
{
	initPage();
	int func;
	while(func!=5)
	{
		cout<<"请选择所需要的功能： "<<endl;
		cout<<"0.输入数据"<<endl;
		cout<<"1.最佳淘汰算法"<<endl;
		cout<<"2.先进先出淘汰算法"<<endl;
		cout<<"3.最近最久未使用淘汰算法"<<endl;
		switch(func)
		{
			case 0:
			    inputData();
			    break;
			case 1:
			    initBlockResult();
			    OPT();
			    break;
			case 2:
			    initBlockResult();
			    FIFO();
				break;
			case 3:
			    initBlockResult();
			    LRU();
				break;
			case 4:
			    initBlockResult();
			    clock();
				break;
			case 5:
			    break;
		}
		cout<<"请选择功能:";
		cin>>func;
		system("cls");
	}
	return 0;
}