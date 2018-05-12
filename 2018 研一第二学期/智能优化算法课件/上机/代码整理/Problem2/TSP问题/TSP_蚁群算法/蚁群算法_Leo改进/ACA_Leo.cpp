#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 178	// 城市数量
#define M 200	// 蚂蚁数量
#define RHO 0.1 // 初始化挥发系数

int ant=M;
int MAXIT=100;					// 最大迭代次数
int MAXIT_Update=5;				// 最大迭代次数
double alpha=1;					// 信息素浓度、              
double beta=5;					// 期望值
double rho=RHO; 				// 挥发系数
double rho_max=0.9; 			// 挥发系数最大值
double rho_rate=1.05; 			// 挥发系数 变化系数
int repetition_threshold=5;		// 重复阈值
double C_Pheromone=10.0;		// 信息素 常数
int Q=100;						// 每只蚂蚁周游一遍留下的信息素总量                               
double Pheromone[N][N];			// 初始时刻各边上的信息素为1                               
int BestPath[N];				// 记录最优路径
double deltaPheromone[N][N];	// 记录信息素                                       
double distance[N][N];			// 记录距离
int sequence[N];				// 记录蚂蚁经过城市队列 
int City_Mask[N];				// 记录城市是否来过
double Coordinates[N][2];		// 坐标矩阵
double probility[N];			// 转移概率
int route[M][N];
double Mult_Rate=0;			// 变异概率 (变异阈值[0,1]，数值越低变异概率越大)

// 产生随机数
double rnd(int low,double uper)//返回[low，uper]之间的一个随机数
{
	double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);
	return (p);
};

int rnd(int uper)//返回[0，uper]之间的一个整数
{
	return rand()%uper;
};

// 选取下一个城市
int ChooseNextCity(int CurCity)//选择下一个城市
{
	int i,num=0;//num表示已走过的城市个数
	int NextCity=10000;
	double temp_probability_sum=0,sum=0;
	// 更新转移概率总和
	for(i=0;i<N;i++) 
	{
		if(City_Mask[i]==0) // 判断是否有值 
		{
			temp_probability_sum+= pow((1.0/distance[CurCity][i]),beta) * pow((Pheromone[CurCity][i]),alpha); //所有未走过的城市转移概率之和
			num++; // 城市数+1
		}
	}
	// 计算城市的转移概率
    for(i=0;i<N;i++)
	{
		if(City_Mask[i]==0) // 未经过的城市计算转移概率
		{
			if(num == 1) 
			{
				NextCity=i;
				break;//只剩一个点时
			}
			probility[i] = pow((1.0/distance[CurCity][i]),beta) * pow((Pheromone[CurCity][i]),alpha) / temp_probability_sum; //转移概率
		}
		else // 经过的城市概率为 0 
		{
			probility[i] = 0;
		}
		sum+=probility[i];  // 总转移概率         
	}

	double rate=rnd(0,sum);//获取[0，sum]间一随机数，由轮盘赌法选择下个城市
	double choose=0;
	if(num!=1) // 选取下一个城市(轮盘赌) 
	{
		double temp_rate=rnd(0,1),max_rate=0;
		for(i=0;i<N;i++)
		{
			if(City_Mask[i]==0)
			{
				if(temp_rate>Mult_Rate)
				{
					choose += probility[i];
					if(choose >= rate)//由轮盘赌法选择下个城市
					{
						NextCity=i; // 赋值新城市编号
						break;
					}
				}
				else{
					if(probility[i]>max_rate)
					{
						max_rate=probility[i];
						NextCity=i;
					}
				}
			}
		}
	}
	if(NextCity>N) // 二次补充
	{
		for(i=0;i<N;i++)
		{
			if(City_Mask[i]==0)
			{
				NextCity=i; // 赋值新城市编号
				break;
			}
		}
	}
	return NextCity;
}


void UpdateTrial(double AntsPathLength,int k)//更新信息素
{
	 int i,j;
	 // 信息量增量
	 for (j=0;j<N-1;j++)
	 {
		 deltaPheromone[route[k][j]][route[k][j+1]]+=Q/AntsPathLength;
		 deltaPheromone[route[k][j+1]][route[k][j]]+=Q/AntsPathLength;
	 }
	 deltaPheromone[route[k][N-1]][0]+=Q/AntsPathLength;
	 deltaPheromone[0][route[k][N-1]]+=Q/AntsPathLength;//蚁环算法 
	 // 信息量更新
	 for (i=0;i<N;i++)
	 {
		 for (j=0;j<N;j++)
		 {
			 Pheromone[i][j]=(1-rho)*Pheromone[i][j]+deltaPheromone[i][j];//路径i-j上的信息素浓度
		     deltaPheromone[i][j]=0;//释放增量
		 }
	 }
 }

// 计算每只蚂蚁的旅行路径
double Travel(int FirstCity)
{
	int CurCity,NextCity;
	double AntsPathLength=0; 
	int i;
	// 转移概率矩阵,经过城市 初始化清零
	for(i=0;i<N;i++)
	{
		probility[i]=0;	// 转移概率清零
		City_Mask[i]=0;	// 城市清零
		sequence[i]=-1; // 路径清零
	}  
	double value=0;//初始路径长度为 0	
	for(i=0;i<N;i++)sequence[i]=0; // 蚂蚁经过的城市 初始化
	sequence[0]=FirstCity;//假设该蚂蚁是从第p个城市出发的
	City_Mask[FirstCity]=1;//将出发城市放入sequence表

	for(i=0;i<=N;i++) // 推算下一个城市并计算路径
	{
		CurCity=sequence[i];	//蚂蚁当前所在城市
		NextCity=ChooseNextCity(CurCity); // 得到下一个城市
		if(NextCity>=N || i+1>N ) break;	// 循环停止条件
		sequence[i+1]=NextCity;	//记录到目前为止蚂蚁已经访问过的城市集合
		City_Mask[NextCity]=1;	//标志该城市已经来过
		value+=distance[CurCity][NextCity]; // 累加距离
	}

	AntsPathLength = value + distance[sequence[0]][sequence[N-1]];//加上从最后一个城市到出发点的距离
	return AntsPathLength;//计算初始城市为FirstCity的路径长度
}

void showBestPath(void) // 打印路径
{
	int i;
	for(i=0;i<N;i++)cout<<BestPath[i]+1<<"-->"; // 循环打印路径
    cout<<BestPath[0]+1; // 增加最后一个城市(起始点)
    cout<<endl;
}

void main()
{
	ifstream infile("data.txt",ios::in|ios::nocreate); //城市坐标信息存放在data.txt
	if(!infile)
	{
		cout<<"不能打开文件\n";
		exit(1);
	}
	double temp;
	int i,j,t,k=0;
	double *d;
	d=&Coordinates[0][0]; // 将指针指向 坐标矩阵
	while(infile>>temp)  //将data.txt中的数据以数组形式读入内存
	{	
		k++;
		if(k%3==1) continue; // 去除序号
		*d=temp; 	// 数组赋值
		d++;		// 指针增加 
	}
	infile.close(); // 关闭文件
	// 计算距离
	for(i=0;i<N;i++)
	{
		BestPath[i]=0; // 最佳路径 初始化
		sequence[i]=-1;
		for(j=0;j<N;j++)
		{
			if(i==j)distance[i][j]=0; // 相同两项记为零
			temp=pow((Coordinates[i][0]-Coordinates[j][0]),2)+pow((Coordinates[i][1]-Coordinates[j][1]),2);//计算距离欧氏距离 X^2 + Y^2
			distance[i][j]=sqrt(temp); // 得到距离
		}
	}

	double BestPathLength=10e9; //用来记录当前的最优路径长
	double AntsPathLength[M]; //当前的路径长度	
	
	int Ant[M]; // 蚂蚁数组
	double start_time,end_time,use_time;
	for(int try_time=0;try_time<MAXIT_Update;try_time++)
	{
		start_time=clock(); // 初始化记录时间
		srand((int)time(0)); // 初始化随机数
		rho=RHO;
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				Pheromone[i][j] = C_Pheromone/distance[i][j];//Pheromone[N][N]初始时刻各边上的信息素为1
				deltaPheromone[i][j] = 0;//deltaPheromone[N][N]初始为0
			}
		}
		for(i=0;i<ant;i++) 
		{
			Ant[i]=rnd(N);	// 随机产生每只蚂蚁初始城市序号
			AntsPathLength[i]=0;	// 初始化每个蚂蚁的爬行距离
		}
		double temp_distace=0;
		int temp_frequency=0;
		// 主算法循环
		for(i=0;i<MAXIT;i++) //小于最大迭代次数
		{
			if(BestPathLength == temp_distace) // 增加动态调整挥发系数功能
			{
				temp_frequency+=1;
				if(temp_frequency>=repetition_threshold)
				{
					double temp_rho= rho_rate * rho;
					if(temp_rho<=rho_max)rho=temp_rho; // 更新挥发系数
				}
			}
			else
			{
				temp_distace=BestPathLength;// 更新最短路程
				temp_frequency=0;			// 更新频率
				rho=RHO;					// 还原挥发系数
			}
			if(temp_frequency>=10*repetition_threshold) // 判断收敛 结束循环
			{
				cout<<"求解趋于收敛，停止循环"<<endl;
				break;
			}
			for(j=0;j<ant;j++)
			{
				AntsPathLength[j]=Travel(Ant[j]);	// 计算得到一个蚂蚁的路径长度及路径
				if(BestPathLength>AntsPathLength[j]) 
				{
					BestPathLength=AntsPathLength[j];//最优长度
					for(t=0;t<N;t++)BestPath[t]=sequence[t];//最优序列
				}
				for(t=0;t<N;t++)
				{
					route[j][t]=sequence[t];//记录每只蚂蚁的路径
				}	
			}
			cout<<"迭代次数 "<<i+1<<"\t重复数 "<<temp_frequency<<"\t最短距离 "<<BestPathLength<<endl;
			//cout<<"路径为:";showBestPath();cout<<endl;
			for(j=0;j<ant;j++) // 更新
			{
				Ant[j]=route[j][N-1];
				UpdateTrial(AntsPathLength[j],j);
			}
		}
		end_time=clock();
		use_time=end_time-start_time;
		cout<<"最佳路径为:"<<endl;showBestPath();
		cout<<"路径长度为: "<<BestPathLength<<endl;
		cout<<"运行时间:"<<use_time<<endl<<endl;	
	}
	//结果保存到
	ofstream f1;
	f1.open("Result.txt",ios::app);
	f1<<"蚁群优化算法(ant-cycle)求解TSP问题"<<endl;
	f1<<"参数:"<<endl;
	f1<<"蚂蚁数量:"<<M<<"\t";
	f1<<"迭代次数:"<<MAXIT<<"\t";
	f1<<"信息素浓度:"<<alpha<<"\t";
	f1<<"期望值:"<<beta<<"\t";
	f1<<"挥发系数:"<<rho<<"\t";
	f1<<"信息素总量:"<<Q<<endl;
	f1<<"最佳路径为:"<<endl;
	for(i=0;i<N;i++)f1<<BestPath[i]+1<<"-->";
	f1<<BestPath[0]+1<<endl;
	f1<<"最优路径长度为："<<BestPathLength<<endl;
	f1<<"程序运行时间为："<<use_time<<endl<<endl;
	f1.close();
}