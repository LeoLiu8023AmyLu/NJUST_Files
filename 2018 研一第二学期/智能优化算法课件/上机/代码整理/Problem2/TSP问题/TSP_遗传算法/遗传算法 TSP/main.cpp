
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#define Num_city 48

double coord[Num_city][2]; //坐标矩阵
double d[Num_city][Num_city];
double pc=0.2;
double pm=0.1;
int pop[100][Num_city]; //编码长度为Num_city位,分别代表Num_city个城市
int M_pos1,M_pos2;
int pop_size,maxgen;

static void distance(void)
{
	int i,j;
	double tem;
    for(i=0;i<Num_city;i++)	
	{
		for (j=i;j<Num_city;j++)
		{   tem=pow((coord[i][0]-coord[j][0]),2)+pow((coord[i][1]-coord[j][1]),2);
			d[i][j]=sqrt(tem);
			d[j][i]=d[i][j];
		}
		d[i][i]=0;
	}

}

static void swap(int m,int n,int mn)
{
	int s;
	s=pop[m][mn];
	pop[m][mn]=pop[n][mn];
	pop[n][mn]=s;

}

static void swap_pop(int m,int n,int mn)
{
	int s;
	s=pop[mn][m];
	pop[mn][m]=pop[mn][n];
	pop[mn][n]=s;

}

static void local(void)
{
	int ij;
	M_pos1=(int)(Num_city*(double)rand()/(RAND_MAX));
	do {
		M_pos2=(int)(Num_city*(double)rand()/(RAND_MAX));
	} while (M_pos1==M_pos2);
	if (M_pos1>M_pos2)
		{
			ij=M_pos1;
			M_pos1=M_pos2;
			M_pos2=ij;
		}
}


static void crossover(void)
{    double cross_P=0;
     int cross_pos=0;
     int i,j;
     for(i=0;i<pop_size-1;i=i+2){
    	cross_P=(rand()%101)/100.0;//随机交叉概率
    	cross_pos=rand()%Num_city;//随机交叉位置
    	if((cross_P<=pc)&&(cross_pos!=0)){//若位置为0，则不进行交叉操作
	    	for(j=cross_pos;j<Num_city;j++){//从交叉位始进行交叉
             swap(i,i+1,j);
				}
			}
		}
}


static void mutation(void)
{ 	double mut_p=0.0;
		int q,i;
        for(i=0;i<pop_size;i++)
		{
			mut_p=(rand()%101)/100.0;//随机变异概率
			if(mut_p<=pm)
            local();
         	for (q=M_pos1+1;q<(M_pos2+M_pos1+1)/2;q++)
			{
		        swap_pop(q,M_pos2+M_pos1+1-q,i);
			}
      	}   
}


void main()
{
	cout<<"\n   遗传算法解决TSP问题\n";
    cout<<endl;
	cout<<"请输入种群大小(20-100):";
    cout<<endl;
	cin>>pop_size;
    cout<<"请输入最大世代数(100-300):";
	cout<<endl;
	cin>>maxgen;
    cout<<"城市坐标如下:"<<"\n";

//*******************编码操作**********************//
    int code[100][Num_city];//编解码表
	double adapt_best=100;//最佳个体
	int best_individual[Num_city];//最优个体
	double adapt_sum[200];//存取历代适应度均值
	int i=0;
	int j=0;
	int t=0;
	for(i=0;i<maxgen;i++){			
				adapt_sum[i]=0;//初始化
			}
	for(i=0;i<pop_size;i++){
		for(j=0;j<Num_city;j++){
			pop[i][j]=j;
			code[i][j]=j;//初始解码模板
			//cout<<pop[i][j]<<" ";
		}//cout<<"\n";
    }
    //cout<<"首次编码输出"<<"\n";
	for(i=0;i<pop_size;i++){
		for(j=0;j<Num_city;j++){
            t=rand();
			pop[i][j]=t%(Num_city-j);//grefenstette编码
			//cout<<pop[i][j]<<" ";
		}//cout<<"\n";
    }
  	
	ifstream infile("data.txt",ios::in|ios::nocreate);
	if(!infile){
		cout<<"不能打开文件\n";
		exit(1);
	}
	double tem;
	int count=0;
	double *dd;

	dd=&coord[0][0];
	while(infile>>tem){
		count++;
		if(count%3==1) continue;
		*dd=tem;
		dd++;
	}
	for(i=0;i<Num_city;i++)
		cout<<coord[i][0]<<" "<<coord[i][1]<<endl;

	distance();

	infile.close();
	cout<<"距离矩阵如下"<<endl;
	for(i=0;i<Num_city;i++)
	{
		for(j=0;j<Num_city;j++)
			cout<<d[i][j]<<" ";
			cout<<endl;
	}
//*******************主循环开始********************//
for(int G=0;G<maxgen;G++)
	{
//*******************交叉操作**********************//
	    crossover();
	//	cout<<"交叉输出"<<"\n";
	//	for(i=0;i<pop_size;i++){
	//		for(j=0;j<Num_city;j++){
	//			cout<<pop[i][j]<<" ";
	//		}  cout<<"\n";
	//	}

//*******************解码操作**********************//
		int temp[100][Num_city];
		int k=0;
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				code[i][j]=j;//解码模板
			}
		}
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				temp[i][j]=code[i][pop[i][j]];//解码
				if(pop[i][j]!=Num_city-1)//排除第一次解码出现编号最大值最后一个城市的情况
					for(k=(pop[i][j]+1);k<Num_city;k++){
						code[i][k-1]=code[i][k];//后位前移，填补抽取留下空缺
					}
			}
		}
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				pop[i][j]=temp[i][j];
			}
		}
	//	cout<<"解码输出"<<"\n";
	//	for(i=0;i<pop_size;i++){
	//		for(j=0;j<Num_city;j++){
	//			cout<<pop[i][j]<<" ";
	// 		}  cout<<"\n";
	//	}

//*******************变异操作**********************//
        mutation();
	
        // cout<<"变异输出"<<"\n";
		// for(i=0;i<pop_size;i++){
		// 	  for(j=0;j<=Num_city;j++){
		//		 cout<<pop[i][j]<<" ";
		//  	} cout<<"\n";
		//  }

//*******************计算适应值********************//
		double adapt[100];//种群适应值之和		
		int best_pos=0;//最佳个体在种群中的位置

    	for(i=0;i<pop_size;i++){
			adapt[i]=0;
			for(j=0;j<Num_city-1;j++){
				adapt[i]=adapt[i]+d[pop[i][j]][pop[i][j+1]];
			}
			adapt[i]=adapt[i]+d[pop[i][Num_city-1]][pop[i][0]];
		    //cout<<adapt[i]<<" ";
		}//cout<<"\n";
		best_pos=0;
		adapt_best=100;
		for(i=0;i<pop_size;i++){
			if(adapt_best>adapt[i]){
				if(adapt[i]!=0){
				adapt_best=adapt[i];
				best_pos=i;//记录最佳个体在种群中的位置
				}
			}
		}cout<<"\n"<<"第"<<G+1<<"代最佳路线所需行程"<<adapt_best;
		//cout<<"\n"<<"第"<<G+1<<"代最佳路线在种群中的位置"<<best_pos;

//*******************保优操作**********************//
        for(j=0;j<Num_city;j++){
			best_individual[j]=0;//初始化
		}
		//cout<<"\n"<<"当代最佳个体"<<"\n";		
		for(j=0;j<Num_city;j++){
			best_individual[j]=pop[best_pos][j];
			//cout<<best_individual[j]<<" ";
		}//cout<<"\n";

		double w;
		w=0;
		for(j=0;j<Num_city-1;j++){
			w=w+d[best_individual[j]][best_individual[j+1]];
			}
			w=w+d[best_individual[Num_city-1]][best_individual[0]];
		//cout<<"\n"<<"当前最佳路线里程为"<<w<<" ";

//*******************选择操作**********************//
		double ada_sum=0;
		double ada_temp=0;//初始化累加值为0
		int new_pop[100][Num_city];//存取新种群		
		double r=0;			
		for(i=0;i<pop_size;i++){
			ada_sum+=adapt[i];//计算种群中个体适应度之和
		}//cout<<ada_sum<<"\n";
		adapt_sum[G]=ada_sum/pop_size;//计算种群历代平均适应度
		for(i=0;i<pop_size-1;i++){
			r=(rand()%1001)/1000.0;//产生0～1间的小数
			r=r*(pop_size*100-ada_sum);//cout<<r<<"\n";
			j=0;
			for(ada_temp=0;ada_temp<=r;ada_temp=ada_temp+(100-adapt[j])){
				j=j++;				
			}
            j=j-1;//退出循环时的j值即为被选择的个体序号
			//cout<<j<<"\n";
			for(k=0;k<Num_city;k++){
				new_pop[i][k]=pop[j][k];//取出被选择的个体存入新种群
			}
		}
		for(j=0;j<Num_city;j++){
			new_pop[pop_size-1][j]=best_individual[j];//最优解存至最后一个个体
		}
		//cout<<"选择输出"<<"\n";
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				pop[i][j]=new_pop[i][j];
				//cout<<pop[i][j]<<" ";
			}//cout<<"\n";
		}


//*******************编码操作**********************//
	int l=Num_city-1;
		int m=0;
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				code[i][j]=j;//编码模板
			}
		}
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				for(k=0;k<=l;k++){
					if(pop[i][j]==code[i][k]){//查找pop中元素在码表code中的位置
						if(k!=Num_city-1){
							temp[i][j]=k;//依照模板进行编码
							for(m=k;m<=l-1;m++){
							    code[i][m]=code[i][m+1];//后位前移，填补抽取留下空缺
							}
                            l=l-1;
						}
                        else {temp[i][j]=k;
                            l=l-1;
						}
					}
				}
			}
			l=Num_city-1;//复原l值
		}
        //cout<<"编码输出"<<"\n";
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
			    pop[i][j]=temp[i][j];
				//cout<<pop[i][j]<<" ";
			}//cout<<"\n";
		}
	}
//*******************主循环结束********************//


//*******************输出操作**********************//

 cout<<"\n"<<"\n";
 cout<<"历代种群平均适应度"<<"\n";
    for(i=0;i<maxgen;i++){
	    cout<<adapt_sum[i]<<" ";
	}
 cout<<"\n"<<"\n"<<"最佳路线为"<<"\n";
    for(j=0;j<Num_city;j++){
    	cout<<best_individual[j]<<"-";
	}
 cout<<best_individual[0];

 cout<<"\n"<<"\n"<<"最佳路线所需行程"<<adapt_best<<"(单位：1000km)"<<"\n";
	fstream result;  
	result.open("输出结果.txt", ios::app);  
	if(!result)  
	{  
		cout<<"can't open the <输出结果.log> file!\n";  
		exit(0);  
	}  
	result<<"*-------------------------------------------------------------------------*"<<endl;  

	result<<"最优路线:"<<endl;
	for(j=0;j<Num_city;j++){
		result<<best_individual[j]<<"-";
	}
	result<<best_individual[0];
	result<<"\n"<<"\n"<<"最佳路线所需行程"<<adapt_best<<"(单位：1000km)"<<"\n";
	result<<"*-------------------------------------------------------------------------*"<<endl<<endl;  
	result.close();  

}
