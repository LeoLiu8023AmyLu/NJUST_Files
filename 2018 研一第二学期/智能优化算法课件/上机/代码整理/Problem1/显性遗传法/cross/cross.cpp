#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <time.h>


//全局变量
#define N 10
int rand_seq1[N];
int rand_seq2[N];
ofstream f;				// 输出流，用于保存结果


void main()
{
	int i,j,k,t,cross_pos;
	//打开txt文件用于记录结果
	f.open("result.txt");
	if(!f)
	{
		cout<<"result.txt不存在!"<<endl;
		exit(1);
	}
	f<<"变化交配法"<<endl;
	f<<endl<<"父代字符串1："<<endl;
	cout<<"父代字符串1："<<endl;
	srand((int)time(0));
	for(int ix=0;ix<N;ix++ )
	{
		rand_seq1[ix]=rand()%2;
		cout<<rand_seq1[ix]<<"\t";
		f<<rand_seq1[ix]<<"\t";
	}
	cout<<endl<<"父代字符串2："<<endl;
	f<<endl<<"父代字符串2："<<endl;
	for(int ib=0;ib<N;ib++ )
	{
		rand_seq2[ib]=rand()%2;
		cout<<rand_seq2[ib]<<"\t";
		f<<rand_seq2[ib]<<"\t";

	}
	cout<<"子代字符串"<<endl;
	f<<endl<<"子代字符串"<<endl;
	for(ix=0;ix<N;ix++ )
	{
		t=rand_seq1[ix]|rand_seq2[ix];
		cout<<t<<"\t";
		f<<t<<"\t";
	}
	f.close();
}