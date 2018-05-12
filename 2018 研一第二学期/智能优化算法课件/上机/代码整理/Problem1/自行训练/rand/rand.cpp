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
cout<<con[i]<<" ";//产生n个二进制数

x=rand()%2;//产生0,1两个随机数;

y=rand()%10;//产生0—100之间的随机数

p=(rand()/(double)RAND_MAX)*(b-a)+a;//产生a,b之间的一个随机数

q=(int)((rand()/(double)RAND_MAX)*(b-a)+a);//产生啊，a,b之间的随机整数

cout<<endl<<"x="<<x<<endl;
cout<<"p="<<p<<endl;
cout<<"q="<<q<<endl;
cout<<"y="<<y<<endl;
return 0;
}
