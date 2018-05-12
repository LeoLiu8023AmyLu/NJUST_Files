//蚁群算法的C++源程序

#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h> 
using namespace std;


const int ant_num=24;//蚂蚁的个数
const int city_num=32;//访问的城市的个数
const int repeat=1000;//最大迭代次数
const double Q=200;//体现蚂蚁所留信息素数量的一个常数
const double a=1;//残留信息的相对重要性:阿尔法
const double b=5;//预见值的相对重要性:贝塔
const double p=0.5;//信息素的保持系数rou（1-p即为信息素的挥发系数）

int besttour[city_num];//最优路径列表

double  rnd(int min,double max)//获得随机数
{
 double r=(rand()/(double)RAND_MAX)*((max)-(min))+(min);

 return (r);
};
int rnd(int max)
{
 return (rand()%max);
};


class Information//定义图相关信息的类
{
public: 
 double T[city_num][city_num];//信息素tuo
 double T_add[city_num][city_num];//信息素增量
 double distance[city_num][city_num];//两点间的距离
};

Information Map;

class ant
{
private:
 int ChooseNextCity();//选择下一个城市
 double prob[city_num];
 int m_city_num;
 int City_flag[city_num];//城市标志位，1表示未走；0表示走过
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
ant::ant()//初始化，所有的城市均为未走
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
void ant::addcity(int city)//将走过的城市加入到表中
{
 visited[m_city_num]=city;
 m_city_num++;
 City_flag[city]=0;
}
int ant::ChooseNextCity()//更新可选城市路径的可能性，选择表中最末一个城市到下一个城市的路径
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
void ant::UpdateResult()//更新游历的路径长度
{
  int i;
 for(i=0;i<city_num-1;i++)
  path_Length+=Map.distance[visited[i]][visited[i+1]];
 path_Length+=Map.distance[visited[city_num-1]][visited[0]];
}
void ant::move()//蚂蚁行进到下一个城市，并将此城市的序号加入表中
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
void search::UpdateTrial(int k)//计算信息素的信息改变
{
  int i;
 int j;

 
  for (j=0;j<city_num-1;j++)//计算信息素增量
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

search::search()//初始地图信息，从关联文件中将城市坐标读入
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
	//计算地图上两点间的距离
   }
  }


}
void search::GetAnt()//随机产生一个蚂蚁，放入到地图中
{
 int i=0;
 int city;
 srand( (unsigned)time( NULL ) +rand());//利用时间函数生成随机数
 for (i=0;i<ant_num;i++)
 {
  city=rnd(city_num);//随机选择一个城市
  ants[i].addcity(city);
 }

}
void search::StartSearch()//开始寻中最优路径
{

 int repeat_time=0;//每个蚂蚁的游历的次数
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


   //寻找最优路径，并将其放入最优路径列表里
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

