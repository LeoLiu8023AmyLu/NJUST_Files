#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <time.h>


//ȫ�ֱ���
#define N 7
int rand_seq1[N]={1,2,3,4,5,6,7};
int rand_seq2[N]={7,6,5,4,3,2,1};
ofstream f;				// ����������ڱ�����


void main()
{
	int i,j,k,t,cross_pos;
	//��txt�ļ����ڼ�¼���
	f.open("result.txt");
	if(!f)
	{
		cout<<"result.txt������!"<<endl;
		exit(1);
	}

	f<<"�仯���䷨"<<endl;
	f<<endl<<"�����ַ���1��"<<endl;
	cout<<"�����ַ���1��"<<endl;
	srand((int)time(0));
	for(int ix=0;ix<N;ix++ )
	{
		//rand_seq1[ix]=rand()%2;
		cout<<rand_seq1[ix]<<"\t";
		f<<rand_seq1[ix]<<"\t";
	}
	cout<<endl<<"�����ַ���2��"<<endl;
	f<<endl<<"�����ַ���2��"<<endl;
	for(int ib=0;ib<N;ib++ )
	{
		//rand_seq2[ib]=rand()%2;
		cout<<rand_seq2[ib]<<"\t";
		f<<rand_seq2[ib]<<"\t";
	}
	cross_pos=rand()%N;
	cout<<"�����"<<cross_pos<<endl;
	f<<endl<<"�����"<<cross_pos<<endl;
	for(i=cross_pos;i<N;i++)
	{
	     t=rand_seq1[i];
		 rand_seq1[i]=rand_seq2[i];
		 rand_seq2[i]=t;
	}

	//�������������
	f<<endl<<"�Ӵ��ַ���1��"<<endl;
	cout<<endl<<"�Ӵ��ַ���1��"<<endl;
	for(ix=0;ix<N;ix++ )
	{
		cout<<rand_seq1[ix]<<"\t";
		f<<rand_seq1[ix]<<"\t";
	}
	cout<<endl<<"�Ӵ��ַ���2��"<<endl;
	f<<endl<<"�Ӵ��ַ���2��"<<endl;
	for(ib=0;ib<N;ib++ )
	{
		cout<<rand_seq2[ib]<<"\t";
		f<<rand_seq2[ib]<<"\t";

	}
	f.close();
}