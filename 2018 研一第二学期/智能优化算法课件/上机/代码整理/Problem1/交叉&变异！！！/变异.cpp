#include<iostream>
#include <ctime>
#include <cstdlib>
#define N 10
using namespace std;

int main()
{
	int num[N],jiyin[2];
    double random(double,double);
	srand(unsigned(time(0)));
    for(int i=0;i<N;i++)
        num[i]=int(random(0,2));
	cout<<"生成的二进制数为: ";
	for(int u=0;u<N;u++)
		cout<<num[u];
	cout<<endl;
    for(int j=0;j<2;j++)
	{
		jiyin[j]=int (random(0,N));
	}
	cout<<"基因变异位分别为：";
	cout<<jiyin[0]<<" "<<jiyin[1]<<endl;
	if(jiyin[0]==jiyin[1]) return false;
	else if(jiyin[0]>jiyin[1])
	{
       int a=num[jiyin[1]];
	   for(int p=jiyin[1];p<(jiyin[0]-1);p++)
	   {
		   num[p]=num[p+1];

	   }
	   
	     num[jiyin[0]-1]=a;
	    cout<<"变异的二进制数为: ";
    	for(int y=0;y<N;y++)
		cout<<num[y];
		cout<<endl;
	}
    else if(jiyin[0]<jiyin[1])
	{
       int a=num[jiyin[0]];
	   for(int p=jiyin[0];p<(jiyin[1]-1);p++)
	   {
		   num[p]=num[p+1];

	   }
	    num[jiyin[1]-1]=a;
	    cout<<"变异的二进制数为: ";
    	for(int y=0;y<N;y++)
		cout<<num[y];
		cout<<endl;
	}
	return 0;
}



double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}
