#include<iostream.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>



void main()
{
	int s;
	int a[10],b[10],c[10];
	srand((int)time(0));//初始化随机种子
	cout<<"a[10]=";
	for(int j=0;j<10;j++)
	{
	    a[j]=rand()%2;
	    cout<<a[j];
	}
	cout<<endl;
	cout<<"b[10]=";
	for(int k=0;k<10;k++)
	{
	    b[k]=rand()%10;
	    cout<<b[k];
	}
	cout<<endl;
	s=rand()%7;
    printf("第%d位开始交换，交换后为",s+1);
	cout<<endl;
	for(int l=s;l<10;l++)
	{
		c[l]=a[l];
		a[l]=b[l];
		b[l]=c[l];
	}
	cout<<"a[10]=";
	for(int t=0;t<10;t++)
	{
	    cout<<a[t];	
	}
	cout<<endl;
	cout<<"b[10]=";
	for(int r=0;r<10;r++)
	{
	    cout<<b[r];	
	}
	cout<<endl;


}