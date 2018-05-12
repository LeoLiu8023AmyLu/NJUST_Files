#include<iostream.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>

#define N 10 //产生二进制数位数
#define M 10 //产生二进制数个数

void main()
{
	srand((int)time(0));
	printf("输出%d个%d位随机二进制数\n",M,N);
	for(int i=0;i<M;i++)
	{
		cout<<i+1<<". ";
	    for(int j=0;j<N;j++)
		    cout<<rand()%2;
	    cout<<"\n";
	}
}