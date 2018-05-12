#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 178	// ��������
#define M 200	// ��������
#define RHO 0.1 // ��ʼ���ӷ�ϵ��

int ant=M;
int MAXIT=100;					// ����������
int MAXIT_Update=5;				// ����������
double alpha=1;					// ��Ϣ��Ũ�ȡ�              
double beta=5;					// ����ֵ
double rho=RHO; 				// �ӷ�ϵ��
double rho_max=0.9; 			// �ӷ�ϵ�����ֵ
double rho_rate=1.05; 			// �ӷ�ϵ�� �仯ϵ��
int repetition_threshold=5;		// �ظ���ֵ
double C_Pheromone=10.0;		// ��Ϣ�� ����
int Q=100;						// ÿֻ��������һ�����µ���Ϣ������                               
double Pheromone[N][N];			// ��ʼʱ�̸����ϵ���Ϣ��Ϊ1                               
int BestPath[N];				// ��¼����·��
double deltaPheromone[N][N];	// ��¼��Ϣ��                                       
double distance[N][N];			// ��¼����
int sequence[N];				// ��¼���Ͼ������ж��� 
int City_Mask[N];				// ��¼�����Ƿ�����
double Coordinates[N][2];		// �������
double probility[N];			// ת�Ƹ���
int route[M][N];
double Mult_Rate=0;			// ������� (������ֵ[0,1]����ֵԽ�ͱ������Խ��)

// ���������
double rnd(int low,double uper)//����[low��uper]֮���һ�������
{
	double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);
	return (p);
};

int rnd(int uper)//����[0��uper]֮���һ������
{
	return rand()%uper;
};

// ѡȡ��һ������
int ChooseNextCity(int CurCity)//ѡ����һ������
{
	int i,num=0;//num��ʾ���߹��ĳ��и���
	int NextCity=10000;
	double temp_probability_sum=0,sum=0;
	// ����ת�Ƹ����ܺ�
	for(i=0;i<N;i++) 
	{
		if(City_Mask[i]==0) // �ж��Ƿ���ֵ 
		{
			temp_probability_sum+= pow((1.0/distance[CurCity][i]),beta) * pow((Pheromone[CurCity][i]),alpha); //����δ�߹��ĳ���ת�Ƹ���֮��
			num++; // ������+1
		}
	}
	// ������е�ת�Ƹ���
    for(i=0;i<N;i++)
	{
		if(City_Mask[i]==0) // δ�����ĳ��м���ת�Ƹ���
		{
			if(num == 1) 
			{
				NextCity=i;
				break;//ֻʣһ����ʱ
			}
			probility[i] = pow((1.0/distance[CurCity][i]),beta) * pow((Pheromone[CurCity][i]),alpha) / temp_probability_sum; //ת�Ƹ���
		}
		else // �����ĳ��и���Ϊ 0 
		{
			probility[i] = 0;
		}
		sum+=probility[i];  // ��ת�Ƹ���         
	}

	double rate=rnd(0,sum);//��ȡ[0��sum]��һ������������̶ķ�ѡ���¸�����
	double choose=0;
	if(num!=1) // ѡȡ��һ������(���̶�) 
	{
		double temp_rate=rnd(0,1),max_rate=0;
		for(i=0;i<N;i++)
		{
			if(City_Mask[i]==0)
			{
				if(temp_rate>Mult_Rate)
				{
					choose += probility[i];
					if(choose >= rate)//�����̶ķ�ѡ���¸�����
					{
						NextCity=i; // ��ֵ�³��б��
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
	if(NextCity>N) // ���β���
	{
		for(i=0;i<N;i++)
		{
			if(City_Mask[i]==0)
			{
				NextCity=i; // ��ֵ�³��б��
				break;
			}
		}
	}
	return NextCity;
}


void UpdateTrial(double AntsPathLength,int k)//������Ϣ��
{
	 int i,j;
	 // ��Ϣ������
	 for (j=0;j<N-1;j++)
	 {
		 deltaPheromone[route[k][j]][route[k][j+1]]+=Q/AntsPathLength;
		 deltaPheromone[route[k][j+1]][route[k][j]]+=Q/AntsPathLength;
	 }
	 deltaPheromone[route[k][N-1]][0]+=Q/AntsPathLength;
	 deltaPheromone[0][route[k][N-1]]+=Q/AntsPathLength;//�ϻ��㷨 
	 // ��Ϣ������
	 for (i=0;i<N;i++)
	 {
		 for (j=0;j<N;j++)
		 {
			 Pheromone[i][j]=(1-rho)*Pheromone[i][j]+deltaPheromone[i][j];//·��i-j�ϵ���Ϣ��Ũ��
		     deltaPheromone[i][j]=0;//�ͷ�����
		 }
	 }
 }

// ����ÿֻ���ϵ�����·��
double Travel(int FirstCity)
{
	int CurCity,NextCity;
	double AntsPathLength=0; 
	int i;
	// ת�Ƹ��ʾ���,�������� ��ʼ������
	for(i=0;i<N;i++)
	{
		probility[i]=0;	// ת�Ƹ�������
		City_Mask[i]=0;	// ��������
		sequence[i]=-1; // ·������
	}  
	double value=0;//��ʼ·������Ϊ 0	
	for(i=0;i<N;i++)sequence[i]=0; // ���Ͼ����ĳ��� ��ʼ��
	sequence[0]=FirstCity;//����������Ǵӵ�p�����г�����
	City_Mask[FirstCity]=1;//���������з���sequence��

	for(i=0;i<=N;i++) // ������һ�����в�����·��
	{
		CurCity=sequence[i];	//���ϵ�ǰ���ڳ���
		NextCity=ChooseNextCity(CurCity); // �õ���һ������
		if(NextCity>=N || i+1>N ) break;	// ѭ��ֹͣ����
		sequence[i+1]=NextCity;	//��¼��ĿǰΪֹ�����Ѿ����ʹ��ĳ��м���
		City_Mask[NextCity]=1;	//��־�ó����Ѿ�����
		value+=distance[CurCity][NextCity]; // �ۼӾ���
	}

	AntsPathLength = value + distance[sequence[0]][sequence[N-1]];//���ϴ����һ�����е�������ľ���
	return AntsPathLength;//�����ʼ����ΪFirstCity��·������
}

void showBestPath(void) // ��ӡ·��
{
	int i;
	for(i=0;i<N;i++)cout<<BestPath[i]+1<<"-->"; // ѭ����ӡ·��
    cout<<BestPath[0]+1; // �������һ������(��ʼ��)
    cout<<endl;
}

void main()
{
	ifstream infile("data.txt",ios::in|ios::nocreate); //����������Ϣ�����data.txt
	if(!infile)
	{
		cout<<"���ܴ��ļ�\n";
		exit(1);
	}
	double temp;
	int i,j,t,k=0;
	double *d;
	d=&Coordinates[0][0]; // ��ָ��ָ�� �������
	while(infile>>temp)  //��data.txt�е�������������ʽ�����ڴ�
	{	
		k++;
		if(k%3==1) continue; // ȥ�����
		*d=temp; 	// ���鸳ֵ
		d++;		// ָ������ 
	}
	infile.close(); // �ر��ļ�
	// �������
	for(i=0;i<N;i++)
	{
		BestPath[i]=0; // ���·�� ��ʼ��
		sequence[i]=-1;
		for(j=0;j<N;j++)
		{
			if(i==j)distance[i][j]=0; // ��ͬ�����Ϊ��
			temp=pow((Coordinates[i][0]-Coordinates[j][0]),2)+pow((Coordinates[i][1]-Coordinates[j][1]),2);//�������ŷ�Ͼ��� X^2 + Y^2
			distance[i][j]=sqrt(temp); // �õ�����
		}
	}

	double BestPathLength=10e9; //������¼��ǰ������·����
	double AntsPathLength[M]; //��ǰ��·������	
	
	int Ant[M]; // ��������
	double start_time,end_time,use_time;
	for(int try_time=0;try_time<MAXIT_Update;try_time++)
	{
		start_time=clock(); // ��ʼ����¼ʱ��
		srand((int)time(0)); // ��ʼ�������
		rho=RHO;
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				Pheromone[i][j] = C_Pheromone/distance[i][j];//Pheromone[N][N]��ʼʱ�̸����ϵ���Ϣ��Ϊ1
				deltaPheromone[i][j] = 0;//deltaPheromone[N][N]��ʼΪ0
			}
		}
		for(i=0;i<ant;i++) 
		{
			Ant[i]=rnd(N);	// �������ÿֻ���ϳ�ʼ�������
			AntsPathLength[i]=0;	// ��ʼ��ÿ�����ϵ����о���
		}
		double temp_distace=0;
		int temp_frequency=0;
		// ���㷨ѭ��
		for(i=0;i<MAXIT;i++) //С������������
		{
			if(BestPathLength == temp_distace) // ���Ӷ�̬�����ӷ�ϵ������
			{
				temp_frequency+=1;
				if(temp_frequency>=repetition_threshold)
				{
					double temp_rho= rho_rate * rho;
					if(temp_rho<=rho_max)rho=temp_rho; // ���»ӷ�ϵ��
				}
			}
			else
			{
				temp_distace=BestPathLength;// �������·��
				temp_frequency=0;			// ����Ƶ��
				rho=RHO;					// ��ԭ�ӷ�ϵ��
			}
			if(temp_frequency>=10*repetition_threshold) // �ж����� ����ѭ��
			{
				cout<<"�������������ֹͣѭ��"<<endl;
				break;
			}
			for(j=0;j<ant;j++)
			{
				AntsPathLength[j]=Travel(Ant[j]);	// ����õ�һ�����ϵ�·�����ȼ�·��
				if(BestPathLength>AntsPathLength[j]) 
				{
					BestPathLength=AntsPathLength[j];//���ų���
					for(t=0;t<N;t++)BestPath[t]=sequence[t];//��������
				}
				for(t=0;t<N;t++)
				{
					route[j][t]=sequence[t];//��¼ÿֻ���ϵ�·��
				}	
			}
			cout<<"�������� "<<i+1<<"\t�ظ��� "<<temp_frequency<<"\t��̾��� "<<BestPathLength<<endl;
			//cout<<"·��Ϊ:";showBestPath();cout<<endl;
			for(j=0;j<ant;j++) // ����
			{
				Ant[j]=route[j][N-1];
				UpdateTrial(AntsPathLength[j],j);
			}
		}
		end_time=clock();
		use_time=end_time-start_time;
		cout<<"���·��Ϊ:"<<endl;showBestPath();
		cout<<"·������Ϊ: "<<BestPathLength<<endl;
		cout<<"����ʱ��:"<<use_time<<endl<<endl;	
	}
	//������浽
	ofstream f1;
	f1.open("Result.txt",ios::app);
	f1<<"��Ⱥ�Ż��㷨(ant-cycle)���TSP����"<<endl;
	f1<<"����:"<<endl;
	f1<<"��������:"<<M<<"\t";
	f1<<"��������:"<<MAXIT<<"\t";
	f1<<"��Ϣ��Ũ��:"<<alpha<<"\t";
	f1<<"����ֵ:"<<beta<<"\t";
	f1<<"�ӷ�ϵ��:"<<rho<<"\t";
	f1<<"��Ϣ������:"<<Q<<endl;
	f1<<"���·��Ϊ:"<<endl;
	for(i=0;i<N;i++)f1<<BestPath[i]+1<<"-->";
	f1<<BestPath[0]+1<<endl;
	f1<<"����·������Ϊ��"<<BestPathLength<<endl;
	f1<<"��������ʱ��Ϊ��"<<use_time<<endl<<endl;
	f1.close();
}