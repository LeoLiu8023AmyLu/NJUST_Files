#include <iostream.h> 
#include <stdlib.h> 
#include <time.h> 

int Len_gene=30;//�������
int pop[2][30];

int main() 
{ 

    int j,i;
	srand((int)time(0)); //���ñ���ʱ��������������Ļ���

    cout<<"����:��ӱ,ѧ��:115104000635"<<endl<<endl;
	cout<<"����30λ���0��1������Ϊ����"<<endl<<endl;
    cout<<"˵��:ѡ���ĸ�λ�ñ任����λ���п����ظ�������ظ���λ��С���ĸ�"<<endl<<endl;
	cout<<"���������1��"<<Len_gene<<"λ����Ϊ:"<<endl;
	for( i = 0; i < Len_gene;i++ ) 
	{
		pop[0][i]=rand()%2;
	    cout <<pop[0][i]; 
	}
	cout<<endl;
	for( i = 0; i < Len_gene;i++ ) 
	{
		pop[1][i]=pop[0][i];
	}
    cout << endl<<endl; 
    int k[4];
    for(j=0;j<4;j++)
	{
		k[j]=rand()%20;
	}

    for(j=0;j<4;j++)     //ѡ�������ĸ�λ�ã�λ���п����ظ�����λ��С���ĸ�
	{
        pop[1][k[j]]=!pop[1][k[j]];
	}


    for(j=0;j<4;j++)
	cout<<"�ĸ��任�Ļ���λ��Ϊ:"<<k[j]+1<<" "<<endl;
    cout<<endl<<endl;
		cout<<"�������ھ�Ϊ:"<<endl;
		for( i = 0; i < Len_gene;i++ ) 
		{
			cout <<pop[1][i]; 
		}
		cout<<endl<<endl;
}