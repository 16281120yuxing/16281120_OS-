#include<iostream>
#include<fstream>

using namespace std;

#define BlockSize 10
#define PageSize 100

int page[PageSize]; //ҳ��������ҳ��
int block[BlockSize]; //���������
int result[PageSize][BlockSize]; //���ҳ���������ά����
int pSize = 0; //�û�ʹ��ҳ����
int bSize = 0; //�û�ʹ���������
int blockFlag[BlockSize]; //���� LRU ������û��㷨�У������жϸû�����ҳ��
int noPageCount = 0; //ȱҳ����


//��������
void inputData()
{
	cout<<endl<<"�������������(1<=bSize<="<<BlockSize<<')'<<endl;
	cin>>bSize;
	cout<<"������ҳ���� (1<=pSize<="<<PageSize<<')'<<endl;
	cin>>pSize;
	while(bSize<=0||bSize>BlockSize||pSize<=0||pSize>PageSize)
	{
		cout<<"���뷶Χ�������������룺 "<<endl;
		cout<<"�������������(1<=F<="<<BlockSize<<')';
		cin>>bSize;
		cout<<endl<<"������ҳ���� (1<=p<="<<PageSize<<')';
		cin>>pSize;
	}
	cout<<"������ҳ������"<<endl;
	for(int i = 0;i <pSize;i++)
		cin>>page[i];
}


//��ʼ�� page ����
void initPage()
{
	for(int i = 0;i<PageSize;i++)
		page[i] = -1;
}


//��ʼ�� block �� result ����
void initBlockResult()
{
	int i = 0;
	for(i = 0;i<BlockSize;i++)
		block[i] = -1;
	for(i = 0;i < PageSize;i++)
		for(int j = 0; j < BlockSize;j++)
			result[i][j] = -1;
}


//������������Ƿ����Ҫ���õ�ҳ��
int Exist(int i)
{
	for(int j = 0;j < bSize;j++)
		if(block[j] == i)
			return j;
	return -1;
}


//��ʾ���
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
	cout<<endl<<"ȱҳ������ "<<noPageCount-3<<endl;
	cout<<"ȱҳ�ʣ� "<<(((double)noPageCount-3)/pSize)*100<<'%'<<endl;
	cout<<"===================================="<<endl;
}


//����û��㷨 OPT
void OPT()
{
	int i = 0,j = 0;
	int position = 0,noPageCount = 0;
	int pageFlag = 0,resultFlag = 0; //ҳ���ǣ��±ָ꣩����һ��ҳ��,�����Ǳ�ʾ������У� �� result ������б�
	for(i = 0;i < BlockSize;i++)
		blockFlag[i] = 0;
	while(pageFlag < pSize)
	{
		if(Exist(page[pageFlag]) != -1) //�ж�ҳ���Ƿ��Ѿ�����
		    resultFlag++;
		else{
			if(position < bSize) //�ж����޿��������
			{ //������ҳ�������п�
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
	cout<<endl<<"�����̭�㷨:"<<endl;
	display(noPageCount);
	return;
}


//�Ƚ��ȳ�ҳ���û��㷨 FIFO
void FIFO()
{
	int blockFlag = 0,pageFlag = 0,resultFlag = 0; //������ǣ�ȷ���û�����������±�
	int i = 0,j = 0,noPageCount = 0;
	int position = 0; //ָʾ����飬�������޿���
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
				block[blockFlag] = page[pageFlag]; //blockFlag ָʾҪ������ҳ��
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
	cout<<endl<<"�Ƚ��ȳ���̭�㷨 FIFO:"<<endl;
	display(noPageCount);
	return;
}


//������δ���㷨 LRU
void LRU()
{
	int i = 0,noPageCount = 0;
	int pageFlag = 0,resultFlag = 0,position = 0;
	for(i = 0;i < BlockSize;i++) //��ʼ��ʱ���¼����
	    blockFlag[i] = 0;
	while(pageFlag < pSize)
	{
		if(Exist(page[pageFlag]) != -1)
		{ //�ж�ҳ���Ƿ��Ѿ���������
	        resultFlag++;
			blockFlag[Exist(page[pageFlag])] = 0; //������ʱ���¼�����Ӧλ��Ϊ 0
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
					block[last] = page[pageFlag]; //�û����δ��ҳ��
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
	cout<<endl<<"������δʹ����̭�㷨 LRU:"<<endl;
	display(noPageCount);
	return;
}


//ʱ��(clock)�û��㷨
void clock()
{
	int i = 0,position = 0,noPageCount = 0;
	bool boolBlockFlag[BlockSize];
	int flag = 0; //����λѭ�����
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
				{ //����ѭ�����ҳ�����λΪ false��ҳ��
			        if(boolBlockFlag[flag] == false) break;
					boolBlockFlag[flag] = false; //��Ϊ true����Ϊ false
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
	cout<<endl<<"ʱ��(clock)�û��㷨:"<<endl;
	display(noPageCount);
	return;
}


int main()
{
	initPage();
	int func;
	while(func!=5)
	{
		cout<<"��ѡ������Ҫ�Ĺ��ܣ� "<<endl;
		cout<<"0.��������"<<endl;
		cout<<"1.�����̭�㷨"<<endl;
		cout<<"2.�Ƚ��ȳ���̭�㷨"<<endl;
		cout<<"3.������δʹ����̭�㷨"<<endl;
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
		cout<<"��ѡ����:";
		cin>>func;
		system("cls");
	}
	return 0;
}