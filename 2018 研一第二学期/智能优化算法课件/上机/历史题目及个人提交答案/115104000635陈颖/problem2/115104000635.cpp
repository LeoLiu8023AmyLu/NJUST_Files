#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <algorithm>
//****************************��ԣ**********************************************

#define N 154//��������
#define M 20//��������


int ant=M;//
int MAXIT=1000;//����������
double alpha=1,beta=5,rho=0.2;// alpha������Ϣ�����Ҫ�̶�,betaԤ��ֵ�����Ҫ�̶�,rho�ӷ�ϵ��             
int Q=100;//ÿֻ��������һ�����µ���Ϣ������                               
double tao[N][N];//��ʼʱ�̸����ϵ���Ϣ��Ϊ1                               
int bestroute[N];// ��¼����·��
double deltatao[N][N];//��¼��Ϣ��                                       
double distance[N][N];//��¼����
int sequence[N];//��¼���Ͼ������ж��� 
int ever[N];//��¼�����Ƿ�����
double xy[N][2];//�������
double probility[N];//ת�Ƹ���
int route[M][N];//��iֻ����Ҫ��N������

double  rnd(int low,double uper)//����[low��uper]֮���һ�������
{
	double p=(rand()/(double)RAND_MAX)*((uper)-(low))+(low);
	return (p);
};


int rnd(int uper)//����[0��uper]֮���һ������
{
	return (rand()%uper);
};


int ChooseNextCity(int CurCity)//ѡ����һ������
{
	int i,num=0;//num��ʾû�߹��ĳ��и���
	int j=10000;
	double temp=0,sum=0;
	for(i=0;i<N;i++)
	{
		if(ever[i]==0) //û����
		{
			temp+=pow((1.0/distance[CurCity][i]),beta)*pow((tao[CurCity][i]),alpha);//����δ�߹��ĳ���ת�Ƹ���֮��
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
			probility[i]=pow((1.0/distance[CurCity][i]),beta)*pow((tao[CurCity][i]),alpha)/temp;//ת�Ƹ���
		}
		else probility[i]=0;//ȥ���� ת�Ƹ���Ϊ0
		sum+=probility[i];           
	}

	double rate=rnd(0,sum);//��ȡ[0��sum]��һ������������̶ķ�ѡ���¸�����
	double choose=0;
	if(num!=1)//���д���һ������ûȥ��
	{
		for(i=0;i<N;i++)
		{
			if(ever[i]==0)//û�����ĳ���
			{
				choose+=probility[i];//ûȥ���ĳ��е�ת�Ƹ��ʺ� choose
				if(choose>=rate)//�����̶ķ�ѡ���¸�����
				{
					j=i;
					break;
				}
			}
		}
	}
	if(i!=j) //ѡ���µ�ûȥ���ĳ���J
	{
		for(i=0;i<N;i++)
			if(ever[i]==0)
			{
				j=i;
				break;
			}
	}
	return j; //ѡ�����һ������
}


void UpdateTrial(double length,int k)//������Ϣ��
{
	 int i;
	 int j;
	 for (j=0;j<N-1;j++)
	 {
		 deltatao[route[k][j]][route[k][j+1]]+=Q/length;//Q ÿֻ��������һ�����µ���Ϣ������   ����·�����䱾�����Ϣ����������Q/length����
		 deltatao[route[k][j+1]][route[k][j]]+=Q/length;//length��ǰ��·������   ��Kֻ���ϴ�J���е�J+1 ���ص���Ϣ��������һ����
	 }
	 deltatao[route[k][N-1]][0]+=Q/length;  //�ص���ʼ���е���Ϣ������Ҳ����
	 deltatao[0][route[k][N-1]]+=Q/length;//�ϻ��㷨 
	 for (i=0;i<N;i++) //��iֻ����
		 for (j=0;j<N;j++)  //��J������
		 {
			 if(rho*deltatao[i][j]<0.3)//·��i-j�ϵ���Ϣ��Ũ�� rho�ӷ�ϵ�� 
			    tao[i][j]=(1-rho)*tao[i][j]+0.1;  //·��i-j�ϵ���Ϣ��Ũ�ȵ���0.3  ����Ϊ(1-rho)*tao[i][j]+0.1  �ӷ���
		     else
                tao[i][j]=(1-rho)*tao[i][j]+rho*deltatao[i][j]; //�������Ϊ(1-rho)*tao[i][j]+rho*deltatao[i][j];  �ӷ��ټ�ǿ
			 deltatao[i][j]=0;//�ͷ�����  
		 }//tao[i][j]������Ϣ��ֵ   deltatao[i][j]��һ����Ϣ������ 
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
		p=FirCity; //ǰһ�����б��
		for(i=0;i<N;i++)
			sequence[i]=0; //��ʼ��01234.����N
		sequence[0]=p;//����������Ǵӵ�p�����г�����
		ever[p]=1;//���������з���sequence��  ever[p]=1˵��ȥ����
		i=0;
	do{
		CurCity=sequence[i];//���ϵ�ǰ���ڳ���
		NextCity=ChooseNextCity(CurCity);//ѡ����һ������j
		i++;//sequence��¼����˳��i  
		if(NextCity>=N) break; //����ֻ���ܵ��ڰ�  ֹͣ
		sequence[i]=NextCity;//��¼��ĿǰΪֹ�����Ѿ����ʹ��ĳ��м���
		ever[NextCity]=1;//��־�ó����Ѿ�����
		value+=distance[CurCity][NextCity];//��ǰ����һ�����еľ���
	}while(i<N);//�������л�û������N��  ���������³���
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
double start_time=clock();//��ʱ

void main()
{
	ifstream infile("data.txt",ios::in|ios::nocreate);//����������Ϣ�����data.txt
	if(!infile)
	{
		cout<<"���ܴ��ļ�\n";
		exit(1);
	}
	double temp;
	int i,j,k=0;
	double *d;
	d=&xy[0][0];
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
			if(i==j) 
				distance[i][j]=0;
			temp=pow((xy[i][0]-xy[j][0]),2)+pow((xy[i][1]-xy[j][1]),2);//����������
			distance[i][j]=sqrt(temp);
		}

	infile.close();


	//��ʼ��	
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			tao[i][j]=1;//tao[N][N]��ʼʱ�̸����ϵ���Ϣ��Ϊ1
		}
	for(i=0;i<N;i++)
		bestroute[i]=0;
	double routelength=10e9;//������¼��ǰ������·����
	double length[M];//��ǰ��·������
	
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			deltatao[i][j]=0;//deltatao[N][N]��ʼΪ0  ��Ϣ������
	int t;
	srand((int)time(0));
	int Ant[M];
	for(i=0;i<ant;i++)//�������ÿֻ���ϳ�ʼ�������
		Ant[i]=rnd(N);
	i=0;
	do{
		j=0;
		do{   //jֻ���� �ж���·
			length[j]=Travel(Ant[j]);
			if(routelength>=length[j])  //��·���Ƚ϶�
			{
				routelength=length[j];//���ų���
				k=j;//��jֻ����
				for(t=0;t<N;t++)
					bestroute[t]=sequence[t];//��������
			}
			for(t=0;t<N;t++)
				route[j][t]=sequence[t];//��¼ÿֻ���ϵ�·��
			j++;	
		}while(j<ant);//ant=M
		cout<<i<<" "<<routelength<<endl;//routelength������·��
		for(j=0;j<ant;j++)
		{
			Ant[j]=route[j][N-1];//���³�ʼ�������
		
		}
		UpdateTrial(length[k],k);////������Ϣ��
		i++;		 //i ��������
	    if((150<i)&&(i<750))        //���ŵ������������ӣ�rho������
			if(i%100==0)
                rho=rho+0.1; //�Ľ���

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
	f1<<"***************************�Ľ�����Ⱥ�Ż��㷨���TSP����************************"<<endl;
	f1<<endl;
	f1<<"��Ⱥ�㷨�Ĳ���Ϊ:";  
	f1<<"alpha="<<alpha<<", beta="<<beta<<", rho="<<rho<<endl;  
	f1<<"��Ⱥ�㷨�ĵ�������Ϊ:"<<MAXIT<<endl;  
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