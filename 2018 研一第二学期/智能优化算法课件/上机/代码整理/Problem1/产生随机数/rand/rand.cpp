#include<stdio.h>
#include<stdlib.h>
#include<iostream.h>
#include <ctime>
int main()
{
#define n 8
double a=5,b=8,x,p,q,y,con[n];
int i;
srand(time(NULL)); 
//x = (int)(rand()/(double)RAND_MAX);

for(i=0;i<n;i++)
con[i]=rand()%2;
for(i=0;i<n;i++)
cout<<con[i]<<" ";//����n����������

x=rand()%2;//����0,1���������;

y=rand()%10;//����0��100֮��������

p=(rand()/(double)RAND_MAX)*(b-a)+a;//����a,b֮���һ�������

q=(int)((rand()/(double)RAND_MAX)*(b-a)+a);//��������a,b֮����������

cout<<endl<<"x="<<x<<endl;
cout<<"p="<<p<<endl;
cout<<"q="<<q<<endl;
cout<<"y="<<y<<endl;
return 0;
}
