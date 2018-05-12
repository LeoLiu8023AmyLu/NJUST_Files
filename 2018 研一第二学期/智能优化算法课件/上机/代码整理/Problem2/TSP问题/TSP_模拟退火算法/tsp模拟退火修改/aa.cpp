#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include <algorithm>
#include <math.h>
#include <fstream.h>
  
#define CityNumber 5                      //定义城市数目
#define rate 0.01                         //定义城市路径差的比率系数
#define EndTem 0.1                        //定义结束温度
#define TemRate 0.9                       //定义温度下降比率

void main()
{
	ifstream in("city_x_y.txt");          //写入城市的坐标信息
//////////////////////////////////////////////////////
	struct City_x_y//结构体
	{
		int num;
		double x;
		double y;
	}City[CityNumber];
//////////////////////////////////////////////////////
	for (int p=0;p<CityNumber;p++)
	{
       in>>City[p].num>>City[p].x>>City[p].y;
       cout<<City[p].num<<" "<<City[p].x<<" "<<City[p].y<<endl;
	}
///////////////////////读入数据/////////
//	int InitialCityRoute[CityNumber]={1,4,2,3,5};
			int InitialCityRoute[CityNumber],//初始化城市间路径
		NowCityRoute[CityNumber]={0},                   //初始化当前路径
		NewCityRoute[CityNumber]={0},                   //初始化新路径
		TemCityRoute[CityNumber]={0};                   //初始化临时路径

			for (int s=0; s<CityNumber; s++) 
	          InitialCityRoute[s] = s+1; //初始化路径


    int *pt;
	int i=0,j=0,k=0,q=0,
		Times=0;                                        //模拟退火算法进行的次数
	double	Cdistancemax=0.0;                           //城市间路径的最大值  
    double  Cdistancemin=10000;                         //路径最小值
	double  fc=0;                                       //路径差
	int m=0,n=0;
	long NL=CityNumber*4;                                             //定义loop循环次数
    double sum=0,OldRouteLong=0,NewRouteLong=0,NowRouteLong=0;        //初始化路径长度
	double InitialTem=0,NowTem=0,d=0.0;                               //初始化温度
    
	
	ofstream fout("result.txt");                        //输出结果到文件‘result.txt’

	                                        
/***********************计算城市间距离信息************************************/
 double a[CityNumber][CityNumber];	
	
 for(i=0;i<CityNumber;i++)
		for(j=0;j<CityNumber;j++)
		{	
			double d,d1,d2;
			if(j==i) {a[i][j]=0;} //同城距离0
			else
			{
			d1=City[i].x-City[j].x;//x坐标差
			d2=City[i].y-City[j].y;//Y坐标差
			d=sqrt(pow((double)d1,2)+pow((double)d2,2)); //距离
			a[i][j]=d; //城市距离矩阵
			}
		}
	
cout<<"城市数目为:  "<<CityNumber<<endl;
cout<<"城市距离矩阵如下所示："<<endl;


for(i=0;i<CityNumber;i++)
{
	for(j=0;j<CityNumber;j++)
	{
    cout<<setw(3)<<a[i][j]<<"  ";
	fout<<setw(3)<<a[i][j]<<"  ";//setw(n)是设置域宽
	}
    cout<<endl<<endl;
	
}

/************************选择初始路径；确定初始温度**************************/

    cout<<"选择的初始路径为："<<endl;
	fout<<"选择的初始路径为："<<endl;
    for( q=0;q<CityNumber;q++)
	{
	cout<<setw(2)<<InitialCityRoute[q]<<"->";
    fout<<setw(2)<<InitialCityRoute[q]<<"->";
	}
		cout<<setw(2)<<InitialCityRoute[0];
    fout<<setw(2)<<InitialCityRoute[0];
	cout<<endl<<endl;
	fout<<endl<<endl;
    
	for(i=0;i<CityNumber-1;i++)
	 {	k++;	  
	    sum+=a[i][k];//从起始出发 初始总路线长 不算回到起点
	 }	 
     sum+=a[i][0];//回到出发的城市  
     OldRouteLong=sum;
     cout<<"初始路径总长度为： "<<OldRouteLong<<endl;
     fout<<"初始路径总长度为： "<<OldRouteLong<<endl;


	 for(i=0;i<CityNumber-1;i++)
	 {
		 for(j=i+1;j<CityNumber;j++)
		 {
            
				if(a[i][j]>Cdistancemax) Cdistancemax=a[i][j];//城市间路径最大值
			    if(a[i][j]<Cdistancemin) Cdistancemin=a[i][j];//最小
		 }
	 }

/*
	 cout<<"城市间最大距离为： "<<Cdistancemax<<endl;
	 cout<<"城市间最小距离为： "<<Cdistancemin<<endl;
	 fout<<"城市间最大距离为： "<<Cdistancemax<<endl;
	 fout<<"城市间最小距离为： "<<Cdistancemin<<endl;
*/
     InitialTem=rate*(Cdistancemax-Cdistancemin);
     cout<<"确定的初始温度为： "<<InitialTem<<endl;
     fout<<"确定的初始温度为： "<<InitialTem<<endl;

/*************************选择当前路径；确定当前温度***************************/

  for(i=0;i<CityNumber;i++) 
  { NowCityRoute[i]=InitialCityRoute[i];//当前路径
    TemCityRoute[i]=InitialCityRoute[i];//临时路径
  }
  
  NowTem=InitialTem;
  cout<<"当前的温度为：   "<<NowTem<<endl;
  fout<<"当前的温度为：   "<<NowTem<<endl;

  cout<<"当前城市路径为： "<<endl;
  fout<<"当前城市路径为： "<<endl;
  for( q=0;q<CityNumber;q++)
  {
	cout<<setw(2)<<NowCityRoute[q]<<"->";
    fout<<setw(2)<<NowCityRoute[q]<<"->";
  }
  	cout<<setw(2)<<NowCityRoute[0];//新路径显示
    fout<<setw(2)<<NowCityRoute[0];


 
/***************************用 2-opt 方法产生新的路径*****************************/

mark:  //MARK是内循环
 
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

	Times++;//模拟退火算法产生新路径的次数 

    cout<<endl<<endl;
    fout<<endl<<endl;
   	cout<<"第 "<<Times<<" 次产生的新路径为:"<<endl;
    fout<<"第 "<<Times<<" 次产生的新路径为:"<<endl;
	for(q=0;q<CityNumber;q++) NewCityRoute[q]=TemCityRoute[q];


	for( q=0;q<CityNumber;q++)	
	{
	cout<<setw(3)<<NewCityRoute[q]<<"->";//新路径显示
    fout<<setw(3)<<NewCityRoute[q]<<"->";
	}
	cout<<setw(3)<<NewCityRoute[0];//新路径显示
    fout<<setw(3)<<NewCityRoute[0];
    cout<<endl;
	fout<<endl;
   
/*****************计算新路径和当前路径的总长度；计算新路径与当前路径的路程差**************/
///////////新路径**********
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
    fout<<"新路径的总长度为:      "<<NewRouteLong<<endl;

    pt=NowCityRoute;	

    for(sum=0,i=0;i<CityNumber-1;i++)
	{
      m=*pt;
	  m--;
	  pt++;
	  n=*pt;
	  n--;
	  sum+=a[m][n];	 //将当前路径所走的长度求和	  
    }

	m=*pt-1;//最后一个城市在距离矩阵里面的序号
	sum+=a[m][0];//回到初始城市
    NowRouteLong=sum;//新路径长度 
	cout<<"当前路径的总长度为:    "<<NowRouteLong<<endl;
	fout<<"当前路径的总长度为:    "<<NowRouteLong<<endl;

	fc=NewRouteLong-NowRouteLong;//新路径和当前路径长度差
	cout<<"新路径与当前路径差为： "<<fc<<endl;
	fout<<"新路径与当前路径差为： "<<fc<<endl;



      
	if(fc<=0)//新路径比较短  接受新路径
	  {
		for(i=0;i<CityNumber;i++) NowCityRoute[i]=NewCityRoute[i];//用新路径取代当前路径
    	cout<<"当前城市路径为：    "<<endl;
        fout<<"当前城市路径为：    "<<endl;
		for( q=0;q<CityNumber;q++)	
		{
    	cout<<setw(3)<<NowCityRoute[q]<<"->";
		fout<<setw(3)<<NowCityRoute[q]<<"->";
		}
		 cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
        cout<<endl<<endl;
		fout<<endl<<endl;

loop:	NL--;//定义loop循环次数     *******以下在判断内循环次数和是否达到停止温度*****每更新了路径都要跳转到这里进行判断****
		if(NL>0)
		  goto mark;//2-opt 方法产生新的路径  开始随机选择新路径
		else 
		{			
			if(NowTem<EndTem)
			{		                 			  
			  int *p=NowCityRoute;
			  cout<<endl<<endl;
		      fout<<endl<<endl;
		      cout<<"当前温度为： "<<NowTem<<endl; 
	          cout<<"循环结束，此时最佳路径为:"<<endl;
			  fout<<"当前温度为： "<<NowTem<<endl; 
	          fout<<"循环结束，此时最佳路径为:"<<endl;
		      for( q=0;q<CityNumber;q++)
			  {
    	      cout<<setw(3)<<NowCityRoute[q]<<"->";
              fout<<setw(3)<<NowCityRoute[q]<<"->";
			  }
			   cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
              cout<<endl; 
              fout<<endl;

			  cout<<"此时对应的路径总长度为：   ";
			  fout<<"此时对应的路径总长度为：   ";

                for(sum=0,i=0;i<CityNumber-1;i++)//最终输出的都是NowCityRoute
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
			fout<<sum<<endl<<endl;
			}
			else //  外循环降温
			{
		      NowTem=TemRate*NowTem;
		      cout<<"当前温度为： "<<NowTem<<endl<<endl;
              fout<<"当前温度为： "<<NowTem<<endl<<endl;
			  NL=CityNumber*4;//定义loop循环次数  重新进行选择 
		      goto mark;
			}		
		}
	  }
     else 
	 {
      double x=0.0,y=0.0;
      x=(rand()%100);
	  x=x/100;
      y=exp(-fc/NowTem);//随机产生一个值X去和y比较  如果y>x  就接受新路径
        if(y>x)
		{
          for(i=0;i<CityNumber;i++) NowCityRoute[i]=NewCityRoute[i];
		  cout<<"当前城市路径为：     "<<endl;
          fout<<"当前城市路径为：     "<<endl;
		  for( q=0;q<CityNumber;q++)
		  {
    	  cout<<setw(3)<<NowCityRoute[q]<<"->";
          fout<<setw(3)<<NowCityRoute[q]<<"->";
		  }
		  cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
		  cout<<endl<<endl; 
		  fout<<endl<<endl;
       	  goto loop; 
		 } 
		else 	//如果y<x  就不接受新路径  输出当前路径
		{ 
			cout<<"当前城市路径为：     "<<endl;
            fout<<"当前城市路径为：     "<<endl;
		     for( q=0;q<CityNumber;q++)
			 {
    	         cout<<setw(3)<<NowCityRoute[q]<<"->";
                 fout<<setw(3)<<NowCityRoute[q]<<"->";
			 }
			  cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
			 cout<<endl<<endl; 
			 cout<<endl<<endl; 
		     goto loop;//记录下循环次数 GOTO MARK重新产生新路径 继续判断
		}
	  }
}