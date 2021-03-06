#include <stdafx.h>
#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include <algorithm>
#include <math.h>
  
#define CityNumber 5                   //定义城市数目
#define rate 10                        //城市路径差的比率系数
#define EndTem 0.1                     //定义结束的温度
#define TemRate 0.9                    //定义温度下降的比率

void main()
{
	int a[CityNumber][CityNumber]={{0,10,15,6,2},{10,0,8,13,9},{15,8,0,20,15},{6,13,20,0,5},{2,9,15,5,0}};
	int InitialCityRoute[CityNumber]={1,2,3,4,5},    //初始化城市间路径
		NowCityRoute[CityNumber]={0,0,0,0,0},        
		NewCityRoute[CityNumber]={0,0,0,0,0},
		TemCityRoute[CityNumber]={0,0,0,0,0};        //初始化当前路径，新路径，临时路径
    int *pt;
	int i=0,j=0,k=0,q=0,
		w=0,                                         //模拟退火算法进行的次数
		Cdistancemax=0,                          //城市间路径的最大值
		Cdistancemin=10000;                              //路径最小值
	int m=0,n=0;
	long fc=0,NL=0,Z=0;
    int sum=0,OldRouteLong=0,NewRouteLong=0,NowRouteLong=0;          //初始化路径长度
	double InitialTem=0,NowTem=0,d=0.0;

	Z=CityNumber*4;
	NL=Z;                                           //定义loop循环次数

/***********************输入城市坐标信息************************************/
	
cout<<"输入的城市数目为:  "<<CityNumber<<endl<<endl;
cout<<"输入的城市距离矩阵如下所示："<<endl<<endl;

for(i=0;i<CityNumber;i++)
{
	for(j=0;j<CityNumber;j++)
    cout<<setw(3)<<a[i][j]<<"  ";
    cout<<endl<<endl;
}

/************************选择初始路径；确定初始温度**************************/

    cout<<"选择的初始路径为：";
    for( q=0;q<CityNumber;q++)	
	cout<<setw(2)<<InitialCityRoute[q]<<" "<<endl;
    
	for(i=0;i<CityNumber-1;i++)
	 {	k++;	  
	    sum+=a[i][k];
	 }
	 
     sum+=a[i][0];
     OldRouteLong=sum;
     cout<<"初始路径总长度为： "<<OldRouteLong<<endl;

     for(i=0;i<CityNumber-1;i++)
	 {
		 for(j=1;j<CityNumber;j++)
		 {
            if(j!=i)
			{
				if(a[i][j]>Cdistancemax) Cdistancemax=a[i][j];
			    if(a[i][j]<Cdistancemin) Cdistancemin=a[i][j];
			}
			else continue;
		 }
	 }

	 cout<<"城市间最大距离为： "<<Cdistancemax<<endl;
	 cout<<"城市间最小距离为： "<<Cdistancemin<<endl;

     InitialTem=rate*(Cdistancemax-Cdistancemin);
     cout<<"确定的初始温度为： "<<InitialTem<<endl<<endl;

/*************************选择当前路径；确定当前温度***************************/

  for(i=0;i<CityNumber;i++) 
  { NowCityRoute[i]=InitialCityRoute[i];
    TemCityRoute[i]=InitialCityRoute[i];
  }
  
  cout<<"当前城市路径为： ";
  for( q=0;q<CityNumber;q++)	
	cout<<setw(2)<<NowCityRoute[q]<<" "<<endl;

  NowTem=InitialTem;
  cout<<"当前的温度为：   "<<NowTem<<endl<<endl;

/***************************用 2-opt 方法产生新的路径*****************************/

mark:
 
	while(1)
	{
		i= (rand()%CityNumber)-1;
        if(i>0)break;
	}

	while(1)
	{
		j=(rand()%CityNumber);

		if(i!=j && j>0)
		{
			int t;
			t=i;
			i=j;
			j=t;
			break;
		}
	}

    k=TemCityRoute[i];
    TemCityRoute[i]=TemCityRoute[j];
    TemCityRoute[j]=k;

	w++;

   	cout<<"第 "<<w<<" 次产生的新路径为:";
	for(q=0;q<CityNumber;q++) NewCityRoute[q]=TemCityRoute[q];

	for( q=0;q<CityNumber;q++)	
	cout<<setw(3)<<NewCityRoute[q];
    cout<<endl;
   
/*****************计算新路径和当前路径的总长度；计算新路径与当前路径的路程差**************/

    pt=NewCityRoute;	

    for(sum=0,i=0;i<CityNumber-1;i++)
	{
      m=*pt;
	  m--;
	  pt++;
	  n=*pt;
	  n--;
	  sum+=a[m][n];	 	  
    }

	m=*pt-1;
	sum+=a[m][0];
    NewRouteLong=sum;
	cout<<"新路径的总长度为:      "<<NewRouteLong<<endl;

    pt=NowCityRoute;	

    for(sum=0,i=0;i<CityNumber-1;i++)
	{
      m=*pt;
	  m--;
	  pt++;
	  n=*pt;
	  n--;
	  sum+=a[m][n];	 	  
    }

	m=*pt-1;
	sum+=a[m][0];
    NowRouteLong=sum;
	cout<<"当前路径的总长度为:    "<<NowRouteLong<<endl;

	fc=NewRouteLong-NowRouteLong;
	cout<<"新路径与当前路径差为： "<<fc<<endl;

/************设置内外循环；判断fc,确定当前路径；判断内外循环终止条件;输出最佳路径*************/
      
	if(fc<=0)
	  {
		for(i=0;i<CityNumber;i++) NowCityRoute[i]=NewCityRoute[i];
    	cout<<"当前城市路径为：    ";
		for( q=0;q<CityNumber;q++)	
    	cout<<setw(3)<<NowCityRoute[q];
        cout<<endl<<endl;

loop:	NL--;
		if(NL>0)
			goto mark;
		else 
		{
			if(NowTem<EndTem)
			{		                 			  
			  int *p=NowCityRoute;
		      cout<<"当前温度为： "<<NowTem<<endl; 
	          cout<<"循环结束，此时最佳路径为:";
		      for( q=0;q<CityNumber;q++)	
    	      cout<<setw(3)<<NowCityRoute[q];
              cout<<endl; 

			  cout<<"此时对应的路径总长度为：   ";

                for(sum=0,i=0;i<CityNumber-1;i++)
				{
                    m=*p;
	                m--;
	                p++;
	                n=*p;
	                n--;
	                sum+=a[m][n];	 	  
				}

  	        m=*p-1;
        	sum+=a[m][0];

			cout<<sum<<endl<<endl;
			}
			else 
			{
		      NowTem=TemRate*NowTem; 
		      cout<<"当前温度为： "<<NowTem<<endl<<endl;
			  NL=Z;
		      goto mark;
			}		
		}
	  }
     else 
	 {
      double x=0.0,y=0.0;
      x=(rand()%100);
	  x=x/100;
      y=exp(-fc/NowTem);
        if(y>x)
		{
          for(i=0;i<CityNumber;i++) NowCityRoute[i]=NewCityRoute[i];
		  cout<<"当前城市路径为：     "<<endl;
		  for( q=0;q<CityNumber;q++)	
    	  cout<<setw(3)<<NowCityRoute[q];
		  cout<<endl<<endl;  
       	  goto loop;
		 } 
		else 	
		{ 
			cout<<"当前城市路径为：     ";
		     for( q=0;q<CityNumber;q++)	
    	         cout<<setw(3)<<NowCityRoute[q];
			 cout<<endl<<endl;  
		     goto loop;
		}
	  }
}