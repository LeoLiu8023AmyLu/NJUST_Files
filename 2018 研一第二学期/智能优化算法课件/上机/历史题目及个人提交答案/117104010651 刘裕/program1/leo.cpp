#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <time.h>


//ȫ�ֱ���
#define N 40
#define M 7
int rand_seqA[N];
int rand_seqB[N];
int cross_pos[2];
ofstream f;				// ����������ڱ�����

// ����[0��uper]֮���һ�����������
int RandomInt(int uper)
{
	return rand()%uper;
}


//n������С��������
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
	//��txt�ļ����ڼ�¼���
	f.open("result.txt");
	if(!f)
	{
		cout<<"result.txt������!"<<endl;
		exit(1);
	}
	f<<"�������һ��40λ�Ķ������ַ������Ա仯������7λ�ķ����������ھ�"<<endl;
	f<<endl<<"�����ַ�����"<<endl;
	cout<<"�����ַ�����"<<endl;
	srand((int)time(0));
	for(int ix=0;ix<N;ix++ )
	{
		rand_seqA[ix]=RandomInt(2);
		rand_seqB[ix]=rand_seqA[ix];
		cout<<rand_seqA[ix]<<" ";
		f<<rand_seqA[ix]<<" ";
	}
	do{
		for(i=0;i<2;i++)//����2������λ��
		{
    		cross_pos[i]=RandomInt(N); // ���������
		}
		while(cross_pos[0]==cross_pos[1]){
			cross_pos[1]=RandomInt(N);
		}
		sort(cross_pos,2); // ���򽻻��������к�
	}while(cross_pos[1]-cross_pos[0]>M);
	cout<<endl<<"��������㣺"<<endl;
	f<<endl<<"��������㣺"<<endl;
	for(i=0;i<2;i++)
	{
		cout<<cross_pos[i]<<"\t";
		f<<cross_pos[i]<<"\t";
	}
	for(i=0;i<=(cross_pos[1]-cross_pos[0]);i++)
	{
		rand_seqA[cross_pos[0]+i]=rand_seqB[cross_pos[1]-i]; 
	}

	//�������������
	f<<endl<<"�Ӵ��ַ�����"<<endl;
	cout<<endl<<"�Ӵ��ַ�����"<<endl;
	for(ix=0;ix<N;ix++ )
	{
		cout<<rand_seqA[ix]<<" ";
		f<<rand_seqA[ix]<<" ";
	}
	f.close();
	cout<<endl;
}