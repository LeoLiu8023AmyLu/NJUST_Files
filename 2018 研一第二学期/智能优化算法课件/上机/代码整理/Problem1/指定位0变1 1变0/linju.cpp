#include <iostream.h> 
#include <stdlib.h> 
#include <time.h> 

int Len_gene=20;//基因个数
int pop[2][20];

int main() 
{ 

    int j,i;
	srand((int)time(0)); //利用本地时间产生随机数所需的基因

    cout<<"姓名:郭明,学号:514104001555"<<endl<<endl;
	cout<<"产生20位随机0、1序列作为基因"<<endl<<endl;
    cout<<"说明:选择四个位置变换基因，位置有可能重复，如果重复则位置小于四个"<<endl<<endl;
	cout<<"随机产生的1个"<<Len_gene<<"位基因为:"<<endl;
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

    for(j=0;j<4;j++)     //选择基因的四个位置，位置有可能重复，则位置小于四个
	{
        pop[1][k[j]]=!pop[1][k[j]];
	}


    for(j=0;j<4;j++)
	cout<<"四个变换的基因位置为:"<<k[j]+1<<" "<<endl;
    cout<<endl<<endl;
		cout<<"产生的邻居为:"<<endl;
		for( i = 0; i < Len_gene;i++ ) 
		{
			cout <<pop[1][i]; 
		}
		cout<<endl<<endl;
}