#include<iostream.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>

#define N 10 //������������λ��
#define M 10 //����������������

void main()
{
	srand((int)time(0));
	printf("���%d��%dλ�����������\n",M,N);
	for(int i=0;i<M;i++)
	{
		cout<<i+1<<". ";
	    for(int j=0;j<N;j++)
		    cout<<rand()%2;
	    cout<<"\n";
	}
}