#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 70//��ʾ���и���
#define ant 20
//ȫ�ֱ�������
int MAXIT=100;//���ѭ������
double alpha=1,beta=5,rho=0.5;
int bestroute[N];// ������¼����·��,��ֵΪ0
int mant=ant;//��������
int Q=100;

double tao[N][N];//��ʼʱ�̸����ϵ���Ϣ��Ϊ1
double deltatao[N][N];//��¼��Ϣ��
double distance[N][N];//��¼��Ϣ��
int tabuk[N];//��¼·��
int ever[N];//ever��¼�����Ƿ�����
double xy[N][2];//�������
double prob[N];
int route[ant][N];


double  rnd(int low,double uper)//����һ��low-uper֮��������
{
 double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);

 return (p);
};

int rnd(int uper)//����һ��0-uper֮�������
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
			if(count==1) {j=i+1;break;}//��ֻʣһ����ʱ���������
			prob[i]=pow((1.0/distance[curCity][i]),beta)*pow((tao[curCity][i]),alpha)/temp;
		}
		else prob[i]=0;
		
		sum+=prob[i];           
	}

	double mRate=rnd(0,sum);//���һ��0��sum֮�����
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
	double val=0;//��ʼ·��������Ϊ 0	
	p=firststep;
	for(i=0;i<N;i++)
		tabuk[i]=0;
	tabuk[0]=p;//��������϶��Ǵӵ� p �����г�����
	ever[p]=1;//p�����Ѿ�������
	i=0;
	do
	{
		curCity=tabuk[i];//���ϵ�ǰ���ڳ���
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
		cout<<"���ܴ��ļ�\n";
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



	cout<<"�����������"<<endl;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			cout<<distance[i][j]<<"   ";
			cout<<endl;
	}


	//��ʼ��
	//tao[N][N]��ʼʱ�̸����ϵ���Ϣ��Ϊ1
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			tao[i][j]=1;
			//if(i==j) tao[i][j]=0;
		}
	//bestroute[N] ������¼����·��,��ֵΪ0
	for(i=0;i<N;i++)
		bestroute[i]=0;
	double routelength=99999;//������¼��ǰ�ҵ�������·����
	double lengthk[50];//��ǰ��·������
	//deltatao[N][N]��ʼΪ0
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			deltatao[i][j]=0;
	int t;
	srand((int)time(0));
	int theant[N];
	for(i=0;i<mant;i++)//ÿֻ���ϳ�ʼλ�õ�ȷ��
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
	

	cout<<"���·��Ϊ��"<<endl;
	for(i=0;i<N;i++)
		cout<<bestroute[i]+1<<"-->";
	cout<<endl;
	cout<<"·������Ϊ������"<<routelength<<endl;
}