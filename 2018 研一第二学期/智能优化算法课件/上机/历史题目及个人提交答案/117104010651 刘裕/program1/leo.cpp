#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <time.h>


//全局变量
#define N 40
#define M 7
int rand_seqA[N];
int rand_seqB[N];
int cross_pos[2];
ofstream f;				// 输出流，用于保存结果

// 返回[0，uper]之间的一个整数随机数
int RandomInt(int uper)
{
	return rand()%uper;
}


//n个数从小到大排序
static void sort(int a[],int n)
{
	int t;
	if (a[0]>a[1])
	{
		t=a[0];
		a[0]=a[1];
		a[1]=t;
	}
}

void main()
{
	int i,j,t;
	//打开txt文件用于记录结果
	f.open("result.txt");
	if(!f)
	{
		cout<<"result.txt不存在!"<<endl;
		exit(1);
	}
	f<<"随机生成一个40位的二进制字符串，以变化不超过7位的方法产生其邻居"<<endl;
	f<<endl<<"父代字符串："<<endl;
	cout<<"父代字符串："<<endl;
	srand((int)time(0));
	for(int ix=0;ix<N;ix++ )
	{
		rand_seqA[ix]=RandomInt(2);
		rand_seqB[ix]=rand_seqA[ix];
		cout<<rand_seqA[ix]<<" ";
		f<<rand_seqA[ix]<<" ";
	}
	do{
		for(i=0;i<2;i++)//产生2个交叉位置
		{
    		cross_pos[i]=RandomInt(N); // 产生随机数
		}
		while(cross_pos[0]==cross_pos[1]){
			cross_pos[1]=RandomInt(N);
		}
		sort(cross_pos,2); // 排序交换区间序列号
	}while(cross_pos[1]-cross_pos[0]>M);
	cout<<endl<<"两个交叉点："<<endl;
	f<<endl<<"两个交叉点："<<endl;
	for(i=0;i<2;i++)
	{
		cout<<cross_pos[i]<<"\t";
		f<<cross_pos[i]<<"\t";
	}
	for(i=0;i<=(cross_pos[1]-cross_pos[0]);i++)
	{
		rand_seqA[cross_pos[0]+i]=rand_seqB[cross_pos[1]-i]; 
	}

	//输出交叉后的序列
	f<<endl<<"子代字符串："<<endl;
	cout<<endl<<"子代字符串："<<endl;
	for(ix=0;ix<N;ix++ )
	{
		cout<<rand_seqA[ix]<<" ";
		f<<rand_seqA[ix]<<" ";
	}
	f.close();
	cout<<endl;
}