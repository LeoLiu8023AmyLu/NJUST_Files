#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <time.h>


//ȫ�ֱ���
#define N 10
int rand_seq1[N];
int rand_seq2[N];
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
		rand_seq1[ix]=rand()%2;
		cout<<rand_seq1[ix]<<"\t";
		f<<rand_seq1[ix]<<"\t";
	}
	cout<<endl<<"�����ַ���2��"<<endl;
	f<<endl<<"�����ַ���2��"<<endl;
	for(int ib=0;ib<N;ib++ )
	{
		rand_seq2[ib]=rand()%2;
		cout<<rand_seq2[ib]<<"\t";
		f<<rand_seq2[ib]<<"\t";

	}
	cout<<"�Ӵ��ַ���"<<endl;
	f<<endl<<"�Ӵ��ַ���"<<endl;
	for(ix=0;ix<N;ix++ )
	{
		t=rand_seq1[ix]|rand_seq2[ix];
		cout<<t<<"\t";
		f<<t<<"\t";
	}
	f.close();
}