//��Ⱥ�㷨��C++Դ����

#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h> 
using namespace std;


const int ant_num=24;//���ϵĸ���
const int city_num=32;//���ʵĳ��еĸ���
const int repeat=1000;//����������
const double Q=200;//��������������Ϣ��������һ������
const double a=1;//������Ϣ�������Ҫ��:������
const double b=5;//Ԥ��ֵ�������Ҫ��:����
const double p=0.5;//��Ϣ�صı���ϵ��rou��1-p��Ϊ��Ϣ�صĻӷ�ϵ����

int besttour[city_num];//����·���б�

double  rnd(int min,double max)//��������
{
 double r=(rand()/(double)RAND_MAX)*((max)-(min))+(min);

 return (r);
};
int rnd(int max)
{
 return (rand()%max);
};


class Information//����ͼ�����Ϣ����
{
public: 
 double T[city_num][city_num];//��Ϣ��tuo
 double T_add[city_num][city_num];//��Ϣ������
 double distance[city_num][city_num];//�����ľ���
};

Information Map;

class ant
{
private:
 int ChooseNextCity();//ѡ����һ������
 double prob[city_num];
 int m_city_num;
 int City_flag[city_num];//���б�־λ��1��ʾδ�ߣ�0��ʾ�߹�
public:
 void addcity(int city);
 int visited[city_num];
 void Clear();
 void UpdateResult();
 double path_Length;
 void move();
 ant();
 void move_to_end();
};
void ant::move_to_end()
{
 int i;
 for(i=0;i<city_num;i++)
  if (City_flag[i]==1)
  {
   addcity(i);
   break;
  }
}
void ant::Clear()
{
 path_Length=0;
 int i;
 for(i=0;i<city_num;i++)
 {
  prob[i]=0;
  City_flag[i]=1;
 }
 i=visited[city_num-1];
 m_city_num=0;
 addcity(i);
}
ant::ant()//��ʼ�������еĳ��о�Ϊδ��
{
 path_Length=0;
 m_city_num=0;
 int i;
 for(i=0;i<city_num;i++)
 {
  City_flag[i]=1;
  prob[i]=0;
 }
}
void ant::addcity(int city)//���߹��ĳ��м��뵽����
{
 visited[m_city_num]=city;
 m_city_num++;
 City_flag[city]=0;
}
int ant::ChooseNextCity()//���¿�ѡ����·���Ŀ����ԣ�ѡ�������ĩһ�����е���һ�����е�·��
{
 //Update the probability of path selection
 //select a path from visited[m_city_num-1] to next


 int i;
 int j=10000;
 double temp=0;
 int curCity=visited[m_city_num-1];
 for (i=0;i<city_num;i++)
 {
  if((City_flag[i]==1)) 
  {
   temp+=pow((1.0/Map.distance[curCity][i]),b)*pow((Map.T[curCity][i]),a);
  }
 }
 double sel=0;
 for (i=0;i<city_num;i++)
 {  
  if((City_flag[i]==1))
  {
   prob[i]=pow((1.0/Map.distance[curCity][i]),b)*pow((Map.T[curCity][i]),a)/temp;
   sel+=prob[i];
  }
  else 
   prob[i]=0;
 }
 double mRate=rnd(0,sel);
 double mSelect=0;

 for ( i=0;i<city_num;i++)
 {
  if((City_flag[i]==1))
   mSelect+=prob[i] ;
  if (mSelect>=mRate) {j=i;break;}
 }

 if (j==10000)
 {
  temp=-1;
  for (i=0;i<city_num;i++)
  { 
   if((City_flag[i]==1))
    if (temp<pow((1.0/Map.distance[curCity][i]),b)*pow((Map.T[curCity][i]),a))     
    {
     temp=pow((1.0/Map.distance[curCity][i]),b)*pow((Map.T[curCity][i]),a);
     j=i;
    }
  }
 }

 return j;

}
void ant::UpdateResult()//����������·������
{
  int i;
 for(i=0;i<city_num-1;i++)
  path_Length+=Map.distance[visited[i]][visited[i+1]];
 path_Length+=Map.distance[visited[city_num-1]][visited[0]];
}
void ant::move()//�����н�����һ�����У������˳��е���ż������
{
 int j;
 j=ChooseNextCity();
 addcity(j);
}
class search
{
public:

 void UpdateTrial();
 double path_Length;
 void initmap();
 ant ants[ant_num];
 void GetAnt();
 void StartSearch();
 search();
};
void search::UpdateTrial(int k)//������Ϣ�ص���Ϣ�ı�
{
  int i;
 int j;

 
  for (j=0;j<city_num-1;j++)//������Ϣ������
  {
   Map.T_add[ants[k].visited[j]][ants[k].visited[j+1]]+=Q/ants[k].path_Length;
   Map.T_add[ants[k].visited[j+1]][ants[k].visited[j]]+=Q/ants[k].path_Length;
  }
  Map.T_add[ants[k].visited[city_num-1]][ants[k].visited[0]]+=Q/ants[k].path_Length;
  Map.T_add[ants[k].visited[0]][ants[k].visited[city_num-1]]+=Q/ants[k].path_Length;
 
 for (i=0;i<city_num;i++)
 {
  for (j=0;j<city_num;j++)
  {
   Map.T[i][j]=((1-p)*Map.T[i][j]+p*Map.T_add[i][j]);
   Map.T_add[i][j]=0;
  }

 }
}


void search::initmap()
{
 int i;
 int j;
 for(i=0;i<city_num;i++)
  for (j=0;j<city_num;j++)
  {

   Map.T[i][j]=1;
   Map.T_add[i][j]=0;
  }
}

search::search()//��ʼ��ͼ��Ϣ���ӹ����ļ��н������������
{
 initmap();
 path_Length=10e9;


 ifstream in("city_coordinate.tsp");

 struct city
 {
  int num;
 float x;
  float y;
 }cc[city_num];
 
 for (int i=0;i<city_num;i++)
 {
  in>>cc[i].num>>cc[i].x>>cc[i].y;
  besttour[i]=0;
 }
 int j;
 for(i=0;i<city_num;i++)
  for (j=0;j<city_num;j++)
  {
   {
    Map.distance[i][j]=sqrt(pow((cc[i].x-cc[j].x),2)+pow((cc[i].y-cc[j].y),2));
	//�����ͼ�������ľ���
   }
  }


}
void search::GetAnt()//�������һ�����ϣ����뵽��ͼ��
{
 int i=0;
 int city;
 srand( (unsigned)time( NULL ) +rand());//����ʱ�亯�����������
 for (i=0;i<ant_num;i++)
 {
  city=rnd(city_num);//���ѡ��һ������
  ants[i].addcity(city);
 }

}
void search::StartSearch()//��ʼѰ������·��
{

 int repeat_time=0;//ÿ�����ϵ������Ĵ���
 int i;
 int j;
 double temp;
 int temptour[city_num];
 while (repeat_time<repeat)
 {  
  for(j=0;j<ant_num;j++) 

  { 
   for (i=0;i<city_num-1;i++)
    ants[j].move();
  }

  for(j=0;j<ant_num;j++) 
  {
   ants[j].move_to_end();
   ants[j].UpdateResult ();
  }


   //Ѱ������·�����������������·���б���
    int t;
	int k;
  temp=ants[0].path_Length;
  for (t=0;t<city_num;t++)
   temptour[t]=ants[0].visited[t];
  for(j=0;j<ant_num;j++) 
  {
   if (temp>ants[j].path_Length) 
   {
    temp=ants[j].path_Length;
    k=j;
	for ( t=0;t<city_num;t++)
     temptour[t]=ants[j].visited[t];
   }

  }

  if(temp<path_Length){
   path_Length=temp;
   for ( t=0;t<city_num;t++)
    besttour[t]=temptour[t];
  }
  printf("%d : %f\n",repeat_time,path_Length);
  UpdateTrial(k); 

  for(j=0;j<ant_num;j++) 
   ants[j].Clear();

  repeat_time++;

 }
 printf("The shortest toure is : %f\n",path_Length);

 for ( int t=0;t<city_num;t++)
  printf(" %d ",besttour[t]);

}

int main()
{

 search TSP;
 TSP.GetAnt();
 TSP.StartSearch();
 return 0;
}

