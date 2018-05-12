#include<iostream.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define U 1000                       //外循环次数
#define L 100                        //内循环次数
#define K 20.0                       //用来确定退火温度的常数
#define N 50                         //物品总数
static int r=1000;                   //背包容量

//--------------------定义获取温度函数----------------------
double get_init_temp(int va1[],int na,int vo1[], int no)      //va1[]为各物品的价值，vo1[]为各物品的体积，na、no均为对应的个数。
{
	double p[N],temp1,T0;
	int i,j,temp2;
	int q[N];
	for(i=0;i<N;i++)
	{
		p[i]=float(va1[i])/float(vo1[i]);                     //p[i]为价值与体积之比
		q[i]=i;                                               //物品编号
	}
	for(i=0;i<N;i++)
		for(j=i;j<N;j++)
		{
			if(p[i]<p[j])                                     //将比值最大的付给p[i]
			{
				temp1=p[i];
				p[i]=p[j];
				p[j]=temp1;


				temp2=q[i];                                   //将比值最大的物品编号付给q[i]
				q[i]=q[j];
				q[j]=temp2;
			}
		}
    int	v_max=0;                                              //初始化最大装入量
    int v_min=0;                                              //初始化最小装入量
	int rr;                                                   //装入物品的体积和 
	int k;
	for(rr=0,i=0;i<N,rr<r;i++)                                //获得最大装入量
	{

         k=q[i];
         v_max+=va1[k];
         rr=rr+vo1[k];
	}
    for(rr=0,i=N-1;i>=0,rr<r;i--)                             //获得最小装入量
	{
        k=q[i];
        v_min=v_min+va1[k];
        rr=rr+vo1[k];
	}
    T0=(v_max-v_min)*K;                                       //获得温度T0
	return T0;
}

void main(void)
{
	int va[N]={220,208,198,192,180,180,165,162,160,158,
               155,130,125,122,120,118,115,110,105,101,
               100,100,98,96,95,90,88,82,80,77,
               75,73,72,70,69,66,65,63,60,58,
               56,50,30,20,15,10,8,5,3,1};                    //各物品的价值
    int vo[N]={80,82,85,70,72,70,66,50,55,25,
               50,55,40,48,50,32,22,60,30,32,
               40,38,35,32,25,28,30,22,50,30,
               45,30,60,50,20,65,20,25,30,10,
                20,25,15,10,10,10,4,4,2,1};                   //各物品对应的体积
    int *point1,int *point2;  
	point1=va;                                                //指针point1指向数组va
	point2=vo;                                                //指针point2指向数组vo
    
    int best_value=0;                                         //定义最大价值
    int best_volumn=0;                                        //定义最大体积
    int pa_best[N];                                           //定义最佳装载方法


    int va_init=0;                                           //定义初始化价值
    int vo_init=0;                                           //定义初始化体积
    int pa_init[N];                                          //定义初始化装载方法

    int va_pre=0;                                            //装入最新物品前的总价值
    int vo_pre=0;                                            //装入最新物品前的总体积
    int pa_pre[N];                                           //装入最新物品前的装载方法

	int va_current=0;                                        //装入最新物品后的总价值
    int vo_current=0;                                        //装入最新物品后的总价值
    int pa_current[N];                                       //装入最新物品后的总价值

    int k=0 ,i,j,m=0;                               
		
//---------------初始化上述变量----------------
    while(vo_init<r)                                         //装入物品
	{
         va_init=va_init+va[k];
         vo_init=vo_init+vo[k];
         pa_init[k]=1;
         k=k+1;
	}
	k=k-1;
	va_init=va_init-va[k];                                   //将最后放不进去的那个物品拿掉
    vo_init=vo_init-vo[k];

	for(j=k;j<N;j++)pa_init[j]=0;
	for(i=0;i<N;i++)                                         //保存装载方法
	{
		pa_current[i]=pa_init[i];
		pa_best[i]=pa_init[i];
	}
	                          
    k=0;                                   
    for(i=0;i<N;i++)                                         //保存最大价值、最大体积
	{
		best_value=best_value+va[i]*pa_best[i];
        best_volumn=best_volumn+vo[i]*pa_best[i];
	}
//--------------初始化结束--------------

    double t0=get_init_temp(point1,N,point2,N),tk=t0;        //获取初始化温度t0，再付给tk
    int n_rec=0;
    int n_rej=0;
    int v_cnt=1;
    int df;
    k=0;                                     
    va_pre=va_init;                                          //将初始化价值付给va_pre                                 
    vo_pre=vo_init;                                          //将初始化体积付给vo_pre
    for(i=0;i<N;i++)pa_pre[i]=pa_init[i];                    //将初始化装载方法付给pa_pre[i]              

//-------------外循环开始-------------
	while(tk>=0.0001)
	{	
		srand((unsigned)time(NULL));

//-------------内循环开始-------------
		while(k<U)
		{                   
		    int rand1,rand2,tmp;                             //产生两个随机数  
		    
            rand1=rand()%N;
            rand2=rand()%N;
		    
            if(rand2<rand1)
			{
			   tmp=rand2;
		       rand2=rand1;
	           rand1=tmp;
			} 
	
            for(;rand1<=rand2;rand1++)                       //将两个随机数之间的数的装载方式取反
               pa_current[rand1]=!pa_current[rand1];
            
//-----------当前装载方法---------------
            m=0;
            while(vo_current<r && m<N)                       //如果装载量小于背包容量并且装载数不满50，就继续装入物品
			{
				va_current=va_current+va[m]*pa_current[m];
                vo_current=vo_current+vo[m]*pa_current[m];
                m=m+1;
			}
            m=m-1;
            va_current=va_current-va[m]*pa_current[m];       //拿掉最后那个装不下的物品
            vo_current=vo_current-vo[m]*pa_current[m];
            for(j=m;j<N;j++)pa_current[m]=0;
            df=va_current-va_pre;
			
		    double d;
		    d=double((rand() %10)/10.0);
        
//-------------判决是否接受新的装载方法---------
            if(df>0 || exp(df/tk)>d)                        //如果满足条件，就保存为新的方法
			{
                   v_cnt=v_cnt+1;     
                   va_pre=va_current;
		           for(m=0;m<N;m++)pa_pre[m]=pa_current[m];
                   n_rec=n_rec+1;
                   best_value=va_current;
                   best_volumn=vo_current;
		           for(m=0;m<N;m++)pa_best[m]=pa_current[m];            
			} 
			
            else                                            //如果不满足条件，就不保存
				n_rej=n_rej+1;                                         
                k=k+1;
				va_current=0;
				vo_current=0;
            if(n_rec>L)break;                                           
			else
                 continue;
		}
//---------------内循环结束---------------

		tk=tk*0.95;                                        //降低温度，进行下一轮循环         
        n_rec=0;
        n_rej=0;
        k=1;
	}	
		system("cls");		
//-----------------外循环结束-------------

//------------------输出结果--------------
        printf("-------------------------------结果------------------------------\n");
        cout<<"初始温度:"<<t0<<endl;
        cout<<"最后温度:"<<tk<<endl;
        cout<<"最大背包价值:"<<best_value<<endl;
        cout<<"最大背包体积:"<<best_volumn<<endl;
        cout<<"最短路径:"<<endl;
        for(i=0;i<N;i++)printf("%d->",pa_best[i]);
		getch();
}

