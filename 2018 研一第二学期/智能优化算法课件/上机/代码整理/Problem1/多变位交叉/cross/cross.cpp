#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <time.h>


//全局变量
#define N 20
#define M 4
int rand_seq1[N];
int rand_seq2[N];
int cross_pos1[M];
ofstream pkFile;				// 输出流，用于保存结果


//n个数从小到大排序
static void sort(int a[],int n)
{
int k,j,t;
for(k=0;k<=n-1;k++)
	 {
	 for(j=0;j<=n-1-k;j++)
	 {
		 if (a[j]>a[j+1])
		{
		t=a[j];
		a[j]=a[j+1];
		a[j+1]=t;
		}
	 }
	 }
}


void main()
{
	int i,j,k,t;
	//打开txt文件用于记录结果
	pkFile.open("result.txt");
	if(!pkFile)
	{
		cout<<"result.txt不存在!"<<endl;
		exit(1);
	}
	pkFile<<"多交配位法"<<endl;
	pkFile<<endl<<"父代字符串1："<<endl;
	cout<<"父代字符串1："<<endl;
	srand((int)time(0));
	for(int ix=0;ix<N;ix++ )
	{
		rand_seq1[ix]=rand()%2;
		cout<<rand_seq1[ix]<<"\t";
		pkFile<<rand_seq1[ix]<<"\t";
	}
	cout<<endl<<"父代字符串2："<<endl;
	pkFile<<endl<<"父代字符串2："<<endl;
	for(int ib=0;ib<N;ib++ )
	{
		rand_seq2[ib]=rand()%2;
		cout<<rand_seq2[ib]<<"\t";
		pkFile<<rand_seq2[ib]<<"\t";

	}

	for(i=0;i<M;i++)//产生4个交叉位置
	 {
    	cross_pos1[i]=rand()%N;//20个数
	 }
	sort(cross_pos1,M);
	cout<<"交叉点的位置: "<<endl;
    pkFile<<endl<<"交叉点位置："<<endl;
	for(i=0;i<M;i++)
	    {
		 cout<<cross_pos1[i]<<"\t";
		 pkFile<<cross_pos1[i]<<"\t";
	 }
    for(j=cross_pos1[0];j<cross_pos1[1];j++)//交叉运算
	 {
		t=rand_seq1[j];
		rand_seq1[j]=rand_seq2[j];
		rand_seq2[j]=t;
	 }
	for(k=cross_pos1[2];k<cross_pos1[3];k++)
	 {
	     t=rand_seq1[k];
		 rand_seq1[k]=rand_seq2[k];
		 rand_seq2[k]=t;
	 }


	//输出交叉后的序列
	pkFile<<endl<<"子代字符串1："<<endl;
	cout<<endl<<"子代字符串1："<<endl;
	for(ix=0;ix<N;ix++ )
	{
		cout<<rand_seq1[ix]<<"\t";
		pkFile<<rand_seq1[ix]<<"\t";
	}
	cout<<endl<<"子代字符串2："<<endl;
	pkFile<<endl<<"子代字符串2："<<endl;
	for(ib=0;ib<N;ib++ )
	{
		cout<<rand_seq2[ib]<<"\t";
		pkFile<<rand_seq2[ib]<<"\t";

	}
	pkFile.close();
}
