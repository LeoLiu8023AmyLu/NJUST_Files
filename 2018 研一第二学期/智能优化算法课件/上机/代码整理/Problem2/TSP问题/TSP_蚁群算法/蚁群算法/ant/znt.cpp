#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 70//表示城市个数
#define ant 20
//全局变量定义
int MAXIT=100;//最大循环次数
double alpha=1,beta=5,rho=0.5;
int bestroute[N];// 用来记录最优路径,初值为0
int mant=ant;//蚂蚁数量
int Q=100;

double tao[N][N];//初始时刻各边上的信息素为1
double deltatao[N][N];//记录信息素
double distance[N][N];//记录信息素
int tabuk[N];//记录路径
int ever[N];//ever记录城市是否来过
double xy[N][2];//坐标矩阵
double prob[N];
int route[ant][N];


double  rnd(int low,double uper)//产生一个low-uper之间的随机数
{
 double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);

 return (p);
};

int rnd(int uper)//产生一个0-uper之间的整数
{
 return (rand()%uper);
};


int choosenext(int curCity)
{
	int i,count=0;
	int j=1000;
	double temp=0,sum=0;
	for(i=0;i<N;i++)
	{
		if(ever[i]==0) 
		{temp+=pow((1.0/distance[curCity][i]),beta)*pow((tao[curCity][i]),alpha);
		count++;
		
		}
	}
    for(i=0;i<N;i++)
	{
		if(ever[i]==0)
		{
			if(count==1) {j=i+1;break;}//当只剩一个点时，不用随机
			prob[i]=pow((1.0/distance[curCity][i]),beta)*pow((tao[curCity][i]),alpha)/temp;
		}
		else prob[i]=0;
		
		sum+=prob[i];           
	}

	double mRate=rnd(0,sum);//随机一个0到sum之间的数
	double mSelect=0;
	if(count!=1)
	{

	for(i=0;i<N;i++)
	{
		if(ever[i]==0)
		{
			mSelect+=prob[i];
			if(mSelect>=mRate) {j=i;break;}
		}
	}
	}
	if(i!=j) 
	{for(i=0;i<N;i++)
	if(ever[i]==0) {j=i;break;}
	}
	return j;
}


void UpdateTrial(double lengthk,int k)
{
 
 int i;
 int j;
  for (j=0;j<N-1;j++)
  {
   deltatao[route[k][j]][route[k][j+1]]+=Q/lengthk;
   deltatao[route[k][j+1]][route[k][j]]+=Q/lengthk;
  }
   deltatao[route[k][N-1]][0]+=Q/lengthk;
   deltatao[0][route[k][N-1]]+=Q/lengthk;  
 for (i=0;i<N;i++)
	 for (j=0;j<N;j++)
	 {tao[i][j]=(1-rho)*tao[i][j]+deltatao[i][j];
	 deltatao[i][j]=0;
	 }

 }

double travel(int firststep)
{
	int p,curCity,NextCity;
	double lengthk=0; 
	int i;
	for(i=0;i<N;i++)
	{prob[i]=0;ever[i]=0;}
	double p_sum=0;    
	double val=0;//初始路径长度设为 0	
	p=firststep;
	for(i=0;i<N;i++)
		tabuk[i]=0;
	tabuk[0]=p;//假设该蚂蚁都是从第 p 个城市出发的
	ever[p]=1;//p城市已经来过了
	i=0;
	do
	{
		curCity=tabuk[i];//蚂蚁当前所在城市
		NextCity=choosenext(curCity);
		i++;
		if(NextCity>=N) break;
		tabuk[i]=NextCity;
		ever[NextCity]=1;
		val+=distance[curCity][NextCity];
	}while(i<N);
	lengthk=val+distance[tabuk[0]][tabuk[N-1]];
	return lengthk;
}




void main(void)
{
	ifstream infile("data.txt",ios::in|ios::nocreate);
	if(!infile){
		cout<<"不能打开文件\n";
		exit(1);
	}
	double temp;
	int i,j,count=0;
	double *d;
	d=&xy[0][0];
	while(infile>>temp){
		count++;
		if(count%3==1) continue;
		*d=temp;
		d++;
	}
    for(i=0;i<N;i++)
		cout<<xy[i][0]<<" "<<xy[i][1]<<endl;
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			if(i==j) distance[i][j]=0;
			temp=pow((xy[i][0]-xy[j][0]),2)+pow((xy[i][1]-xy[j][1]),2);
			distance[i][j]=sqrt(temp);
		}

	infile.close();



	cout<<"距离矩阵如下"<<endl;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			cout<<distance[i][j]<<"   ";
			cout<<endl;
	}


	//初始化
	//tao[N][N]初始时刻各边上的信息素为1
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			tao[i][j]=1;
			//if(i==j) tao[i][j]=0;
		}
	//bestroute[N] 用来记录最优路径,初值为0
	for(i=0;i<N;i++)
		bestroute[i]=0;
	double routelength=99999;//用来记录当前找到的最优路径长
	double lengthk[50];//当前的路径长度
	//deltatao[N][N]初始为0
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			deltatao[i][j]=0;
	int t;
	srand((int)time(0));
	int theant[N];
	for(i=0;i<mant;i++)//每只蚂蚁初始位置的确定
		theant[i]=rnd(N);
	i=0;
	do{
		j=0;
		do{
			lengthk[j]=travel(theant[j]);
		

			if(routelength>lengthk[j]) 
			{
				routelength=lengthk[j];
				for(t=0;t<N;t++)
				bestroute[t]=tabuk[t];
			}
			for(t=0;t<N;t++)
				route[j][t]=tabuk[t];
			j++;
			
		}while(j<mant);
		cout<<routelength<<endl;
		for(j=0;j<mant;j++)
		{
			theant[j]=route[j][N-1];
			UpdateTrial(lengthk[j],j);
		}
		i++;
		
	}

	while(i<MAXIT);
	fstream f1("save.txt",ios::out|ios::binary);
	for(i=0;i<N;i++)
	{t=bestroute[i];
	f1<<xy[t][0]<<" "<<xy[t][1]<<endl;
	}
	f1.close;
	

	cout<<"最佳路径为："<<endl;
	for(i=0;i<N;i++)
		cout<<bestroute[i]+1<<"-->";
	cout<<endl;
	cout<<"路径长度为：　　"<<routelength<<endl;
}