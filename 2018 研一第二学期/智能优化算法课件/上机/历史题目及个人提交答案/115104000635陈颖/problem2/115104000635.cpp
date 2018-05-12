#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <algorithm>
//****************************刘裕**********************************************

#define N 154//城市数量
#define M 20//蚂蚁数量


int ant=M;//
int MAXIT=1000;//最大迭代次数
double alpha=1,beta=5,rho=0.2;// alpha残留信息相对重要程度,beta预见值相对重要程度,rho挥发系数             
int Q=100;//每只蚂蚁周游一遍留下的信息素总量                               
double tao[N][N];//初始时刻各边上的信息素为1                               
int bestroute[N];// 记录最优路径
double deltatao[N][N];//记录信息素                                       
double distance[N][N];//记录距离
int sequence[N];//记录蚂蚁经过城市队列 
int ever[N];//记录城市是否来过
double xy[N][2];//坐标矩阵
double probility[N];//转移概率
int route[M][N];//第i只蚂蚁要走N个城市

double  rnd(int low,double uper)//返回[low，uper]之间的一个随机数
{
	double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);
	return (p);
};


int rnd(int uper)//返回[0，uper]之间的一个整数
{
	return (rand()%uper);
};


int ChooseNextCity(int CurCity)//选择下一个城市
{
	int i,num=0;//num表示没走过的城市个数
	int j=10000;
	double temp=0,sum=0;
	for(i=0;i<N;i++)
	{
		if(ever[i]==0) //没来过
		{
			temp+=pow((1.0/distance[CurCity][i]),beta)*pow((tao[CurCity][i]),alpha);//所有未走过的城市转移概率之和
			num++;
		}
	}
    for(i=0;i<N;i++)
	{
		if(ever[i]==0)
		{
			if(num==1) 
			{
				j=i;
				break;//只剩一个点时
			}
			probility[i]=pow((1.0/distance[CurCity][i]),beta)*pow((tao[CurCity][i]),alpha)/temp;//转移概率
		}
		else probility[i]=0;//去过了 转移概率为0
		sum+=probility[i];           
	}

	double rate=rnd(0,sum);//获取[0，sum]间一随机数，由轮盘赌法选择下个城市
	double choose=0;
	if(num!=1)//还有大于一个城市没去过
	{
		for(i=0;i<N;i++)
		{
			if(ever[i]==0)//没来过的城市
			{
				choose+=probility[i];//没去过的城市的转移概率和 choose
				if(choose>=rate)//由轮盘赌法选择下个城市
				{
					j=i;
					break;
				}
			}
		}
	}
	if(i!=j) //选择新的没去过的城市J
	{
		for(i=0;i<N;i++)
			if(ever[i]==0)
			{
				j=i;
				break;
			}
	}
	return j; //选择的下一个城市
}


void UpdateTrial(double length,int k)//更新信息素
{
	 int i;
	 int j;
	 for (j=0;j<N-1;j++)
	 {
		 deltatao[route[k][j]][route[k][j+1]]+=Q/length;//Q 每只蚂蚁周游一遍留下的信息素总量   行走路径上其本身的信息素增量加上Q/length增量
		 deltatao[route[k][j+1]][route[k][j]]+=Q/length;//length当前的路径长度   第K只蚂蚁从J城市到J+1 来回的信息素增量是一样的
	 }
	 deltatao[route[k][N-1]][0]+=Q/length;  //回到初始城市的信息素来回也增加
	 deltatao[0][route[k][N-1]]+=Q/length;//蚁环算法 
	 for (i=0;i<N;i++) //第i只蚂蚁
		 for (j=0;j<N;j++)  //第J个城市
		 {
			 if(rho*deltatao[i][j]<0.3)//路径i-j上的信息素浓度 rho挥发系数 
			    tao[i][j]=(1-rho)*tao[i][j]+0.1;  //路径i-j上的信息素浓度低于0.3  更新为(1-rho)*tao[i][j]+0.1  挥发？
		     else
                tao[i][j]=(1-rho)*tao[i][j]+rho*deltatao[i][j]; //否则更新为(1-rho)*tao[i][j]+rho*deltatao[i][j];  挥发再加强
			 deltatao[i][j]=0;//释放增量  
		 }//tao[i][j]才是信息素值   deltatao[i][j]是一个信息素增量 
 }



//
double Travel(int FirCity)
{
	int p,CurCity,NextCity;
	double length=0; 
	int i;
	for(i=0;i<N;i++)
	{
		probility[i]=0;ever[i]=0;}  
		double value=0;//初始路径长度为 0	
		p=FirCity; //前一个城市标记
		for(i=0;i<N;i++)
			sequence[i]=0; //初始化01234.。。N
		sequence[0]=p;//假设该蚂蚁是从第p个城市出发的
		ever[p]=1;//将出发城市放入sequence表  ever[p]=1说明去过了
		i=0;
	do{
		CurCity=sequence[i];//蚂蚁当前所在城市
		NextCity=ChooseNextCity(CurCity);//选择下一个城市j
		i++;//sequence记录城市顺序i  
		if(NextCity>=N) break; //好像只可能等于吧  停止
		sequence[i]=NextCity;//记录到目前为止蚂蚁已经访问过的城市集合
		ever[NextCity]=1;//标志该城市已经来过
		value+=distance[CurCity][NextCity];//当前和下一个城市的距离
	}while(i<N);//城市序列还没有走满N个  继续产生新城市
	length=value+distance[sequence[0]][sequence[N-1]];//加上从最后一个城市到出发点的距离
	return length;//计算初始城市为fircity的路径长度
}

void show(void)
{
	int temp,i;
	for(i=0;i<N;i++)
	//{temp=bestroute[i];
	//cout<<add[temp]<<"-->";
	{cout<<bestroute[i]+1<<"-->";
	}
	//temp=bestroute[0];
	//cout<<add[temp]<<endl;
    cout<<bestroute[0]+1;
    cout<<endl;
	


}
double start_time=clock();//计时

void main()
{
	ifstream infile("data.txt",ios::in|ios::nocreate);//城市坐标信息存放在data.txt
	if(!infile)
	{
		cout<<"不能打开文件\n";
		exit(1);
	}
	double temp;
	int i,j,k=0;
	double *d;
	d=&xy[0][0];
	while(infile>>temp)  //将data.txt中的数据以数组形式读入内存
	{	
		k++;
		if(k%3==1) continue;
		*d=temp;
		d++;
	}

	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			if(i==j) 
				distance[i][j]=0;
			temp=pow((xy[i][0]-xy[j][0]),2)+pow((xy[i][1]-xy[j][1]),2);//计算距离矩阵
			distance[i][j]=sqrt(temp);
		}

	infile.close();


	//初始化	
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			tao[i][j]=1;//tao[N][N]初始时刻各边上的信息素为1
		}
	for(i=0;i<N;i++)
		bestroute[i]=0;
	double routelength=10e9;//用来记录当前的最优路径长
	double length[M];//当前的路径长度
	
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			deltatao[i][j]=0;//deltatao[N][N]初始为0  信息素增量
	int t;
	srand((int)time(0));
	int Ant[M];
	for(i=0;i<ant;i++)//随机产生每只蚂蚁初始城市序号
		Ant[i]=rnd(N);
	i=0;
	do{
		j=0;
		do{   //j只蚂蚁 判断新路
			length[j]=Travel(Ant[j]);
			if(routelength>=length[j])  //新路径比较短
			{
				routelength=length[j];//最优长度
				k=j;//第j只蚂蚁
				for(t=0;t<N;t++)
					bestroute[t]=sequence[t];//最优序列
			}
			for(t=0;t<N;t++)
				route[j][t]=sequence[t];//记录每只蚂蚁的路径
			j++;	
		}while(j<ant);//ant=M
		cout<<i<<" "<<routelength<<endl;//routelength是最优路径
		for(j=0;j<ant;j++)
		{
			Ant[j]=route[j][N-1];//更新初始城市序号
		
		}
		UpdateTrial(length[k],k);////更新信息素
		i++;		 //i 迭代次数
	    if((150<i)&&(i<750))        //随着迭代次数的增加，rho逐渐增大
			if(i%100==0)
                rho=rho+0.1; //改进吧

	}while(i<MAXIT);//小于最大迭代次数
	double end_time=clock();
    double use_time=end_time-start_time;
	cout<<"最佳路径为："<<endl;
	show();
	cout<<"路径长度为: "<<routelength<<endl;
    cout<<"运行时间："<<use_time<<endl;
	//结果保存到
	ofstream f1;
	f1.open("Result.txt",ios::app);
	f1<<"***************************改进的蚁群优化算法求解TSP问题************************"<<endl;
	f1<<endl;
	f1<<"蚁群算法的参数为:";  
	f1<<"alpha="<<alpha<<", beta="<<beta<<", rho="<<rho<<endl;  
	f1<<"蚁群算法的迭代次数为:"<<MAXIT<<endl;  
	f1<<"最佳路径为：";
	for(i=0;i<N;i++)
	   //f1<<add[bestroute[i]]<<"-->";
	   f1<<bestroute[i]+1<<"-->";
	//f1<<add[bestroute[0]]<<endl;
	f1<<bestroute[0]+1<<endl;
	f1<<"最优路径长度为："<<routelength<<endl;
	f1<<"程序运行时间为："<<use_time<<endl;
	f1.close();	

}