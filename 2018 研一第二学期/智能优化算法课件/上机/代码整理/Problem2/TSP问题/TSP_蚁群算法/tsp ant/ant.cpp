#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#define N 30//��������
#define M 20//��������

int ant=M;
int MAXIT=500;					//����������
double alpha=1,beta=5,rho=0.5;	//��Ϣ��Ũ�ȡ�����ֵ���ӷ�ϵ��              
int Q=100;						//ÿֻ��������һ�����µ���Ϣ������                               
double Pheromone[N][N];			//��ʼʱ�̸����ϵ���Ϣ��Ϊ1                               
int bestroute[N];				// ��¼����·��
double deltaPheromone[N][N];	//��¼��Ϣ��                                       
double distance[N][N];			//��¼����
int sequence[N];				//��¼���Ͼ������ж��� 
int ever[N];					//��¼�����Ƿ�����
double Coordinates[N][2];		//�������
double probility[N];			//ת�Ƹ���
int route[M][N];

// ���������
double  rnd(int low,double uper)//����[low��uper]֮���һ�������
{
	double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);
	return (p);
};


int rnd(int uper)//����[0��uper]֮���һ������
{
	return (rand()%uper);
};


// ѡȡ��һ������
int ChooseNextCity(int CurCity)//ѡ����һ������
{
	int i,num=0;//num��ʾ���߹��ĳ��и���
	int j=10000;
	double temp=0,sum=0;
	for(i=0;i<N;i++)
	{
		if(ever[i]==0) 
		{
			temp+=pow((1.0/distance[CurCity][i]),beta)*pow((Pheromone[CurCity][i]),alpha);//����δ�߹��ĳ���ת�Ƹ���֮��
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
				break;//ֻʣһ����ʱ
			}
			probility[i]=pow((1.0/distance[CurCity][i]),beta)*pow((Pheromone[CurCity][i]),alpha)/temp;//ת�Ƹ���
		}
		else probility[i]=0;
		sum+=probility[i];           
	}

	double rate=rnd(0,sum);//��ȡ[0��sum]��һ������������̶ķ�ѡ���¸�����
	double choose=0;
	if(num!=1)
	{
		for(i=0;i<N;i++)
		{
			if(ever[i]==0)
			{
				choose+=probility[i];
				if(choose>=rate)//�����̶ķ�ѡ���¸�����
				{
					j=i;
					break;
				}
			}
		}
	}
	if(i!=j) 
	{
		for(i=0;i<N;i++)
			if(ever[i]==0)
			{
				j=i;
				break;
			}
	}
	return j;
}


void UpdateTrial(double length,int k)//������Ϣ��
{
	 int i;
	 int j;
	 for (j=0;j<N-1;j++)
	 {
		 deltaPheromone[route[k][j]][route[k][j+1]]+=Q/length;
		 deltaPheromone[route[k][j+1]][route[k][j]]+=Q/length;
	 }
	 deltaPheromone[route[k][N-1]][0]+=Q/length;
	 deltaPheromone[0][route[k][N-1]]+=Q/length;//�ϻ��㷨 
	 for (i=0;i<N;i++)
		 for (j=0;j<N;j++)
		 {
			 Pheromone[i][j]=(1-rho)*Pheromone[i][j]+deltaPheromone[i][j];//·��i-j�ϵ���Ϣ��Ũ��
		     deltaPheromone[i][j]=0;//�ͷ�����
		 }
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
		double value=0;//��ʼ·������Ϊ 0	
		p=FirCity;
		for(i=0;i<N;i++)
			sequence[i]=0;
		sequence[0]=p;//����������Ǵӵ�p�����г�����
		ever[p]=1;//���������з���sequence��
		i=0;
	do{
		CurCity=sequence[i];//���ϵ�ǰ���ڳ���
		NextCity=ChooseNextCity(CurCity);
		i++;
		if(NextCity>=N) break;
		sequence[i]=NextCity;//��¼��ĿǰΪֹ�����Ѿ����ʹ��ĳ��м���
		ever[NextCity]=1;//��־�ó����Ѿ�����
		value+=distance[CurCity][NextCity];
	}while(i<N);
	length=value+distance[sequence[0]][sequence[N-1]];//���ϴ����һ�����е�������ľ���
	return length;//�����ʼ����Ϊfircity��·������
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
double start_time=clock();

void main()
{
	ifstream infile("data.txt",ios::in|ios::nocreate); //����������Ϣ�����data.txt
	if(!infile)
	{
		cout<<"���ܴ��ļ�\n";
		exit(1);
	}
	double temp;
	int i,j,k=0;
	double *d;
	d=&Coordinates[0][0];
	while(infile>>temp)  //��data.txt�е�������������ʽ�����ڴ�
	{	
		k++;
		if(k%3==1) continue;
		*d=temp;
		d++;
	}

	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			if(i==j)distance[i][j]=0; // ��ͬ�����Ϊ��
			temp=pow((Coordinates[i][0]-Coordinates[j][0]),2)+pow((Coordinates[i][1]-Coordinates[j][1]),2);//�������ŷ�Ͼ��� X^2 + Y^2
			distance[i][j]=sqrt(temp);
		}

	infile.close();

	//��ʼ��	
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			Pheromone[i][j]=1;//Pheromone[N][N]��ʼʱ�̸����ϵ���Ϣ��Ϊ1
		}
	for(i=0;i<N;i++)
		bestroute[i]=0;
	double routelength=10e9;//������¼��ǰ������·����
	double length[M];//��ǰ��·������
	
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			deltaPheromone[i][j]=0;//deltaPheromone[N][N]��ʼΪ0
	int t;
	srand((int)time(0));
	int Ant[M];
	for(i=0;i<ant;i++)//�������ÿֻ���ϳ�ʼ�������????????/ÿֻ����ÿ�ε�����ʼ���ж�һ����������������
		Ant[i]=rnd(N);
	i=0;
	do{
		j=0;
		do{
			length[j]=Travel(Ant[j]);
			if(routelength>length[j]) 
			{
				routelength=length[j];//���ų���
				for(t=0;t<N;t++)
					bestroute[t]=sequence[t];//��������
			}
			for(t=0;t<N;t++)
				route[j][t]=sequence[t];//��¼ÿֻ���ϵ�·��
			j++;	
		}while(j<ant);
		cout<<routelength<<endl;
		for(j=0;j<ant;j++)
		{
			Ant[j]=route[j][N-1];
			UpdateTrial(length[j],j);
		}
		i++;		
	}while(i<MAXIT);//С������������
	double end_time=clock();
    double use_time=end_time-start_time;
	cout<<"���·��Ϊ��"<<endl;
	show();
	cout<<"·������Ϊ: "<<routelength<<endl;
    cout<<"����ʱ�䣺"<<use_time<<endl;
	//������浽
	ofstream f1;
	f1.open("Result.txt",ios::app);
	f1<<"��Ⱥ�Ż��㷨���TSP����"<<endl;
	f1<<"���·��Ϊ��";
	for(i=0;i<N;i++)
	   //f1<<add[bestroute[i]]<<"-->";
	   f1<<bestroute[i]+1<<"-->";
	//f1<<add[bestroute[0]]<<endl;
	f1<<bestroute[0]+1<<endl;
	f1<<"����·������Ϊ��"<<routelength<<endl;
	f1<<"��������ʱ��Ϊ��"<<use_time<<endl;
	f1.close();	

}