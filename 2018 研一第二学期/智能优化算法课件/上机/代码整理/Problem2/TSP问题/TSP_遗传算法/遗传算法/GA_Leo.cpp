// Genetic Algorithm for TSP
// Written by 刘裕 117104010651

#include <iostream.h>
#include<fstream.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
// 函数名
static void  initialization(void);
static void  evaluation(int gen);
static void  selection(void);
static void  crossover(void);
static void  mutation(void);
static void  objective_function(void);
static void  input_data(void);
double myu(double a, double b);

#define N 178			// 城市数量
#define TYPE -1			// 1=max;-1=min
#define GEN 3000		// maximum generation number
#define POP_SIZE 178	// 基因长度
#define P_MUTATION 0.5	// 变异率
#define P_CROSSOVER 0.5	// 交叉率

int  CHROMOSOME[POP_SIZE+1][N+1];
double  OBJECTIVE[POP_SIZE+1];
double  d[N+1][N+1]; // 城市间距离矩阵
double  q[POP_SIZE+1];


static void objective_function(void)
{
	int x[N+2];
	int i,j;
	for(i = 1; i <= POP_SIZE; i++) {
		for(j=1; j<=N; j++)
			x[j]=CHROMOSOME[i][j];
		x[N+1]=x[1];
		OBJECTIVE[i]=0.0;
		for(j=1; j<=N; j++)
		    OBJECTIVE[i] = OBJECTIVE[i] + d[x[j]][x[j+1]];
	}
}



static void  input_data(void)
{
	double x[N+1], y[N+1];
	int i,j; 
	float index,read1, read2;
	FILE *fp;

	fp=fopen("data.txt","r");
	for(i=1; i<=N; i++){
	    fscanf(fp,"%f %f %f",&index,&read1,&read2);
		x[i]=read1; y[i]=read2;
	}
	fclose(fp);

	for(i=1; i<=N; i++)
		for(j=1; j<=N; j++)
			d[i][j]=sqrt(pow(x[i]-x[j],2)+pow(y[i]-y[j],2));
	
}


static void initialization(void) // 初始化程序
{
  int x[N+1], m; // N is the number of cities
  int i,j,k;
  
  for(i=1; i<=POP_SIZE; i++){
	  for(j=1; j<=N; j++) x[j]=j;
	  for(j=1; j<=N; j++) {
		  m=(int)(myu(1,N-j+1+0.9));
	      CHROMOSOME[i][j]=x[m];
		  for(k=1; k<m; k++) x[k]=x[k];
		  for(k=m; k<=N-j; k++) x[k]=x[k+1];
	  }
  }
}

void main() // 主函数
{
  int i, j;
  double a;
  double start_time,end_time,use_time;
  start_time=clock(); // 初始化记录时间
  srand(time(NULL)); // 随机数
  q[0]=0.05; a=0.05;
  for(i=1; i<=POP_SIZE; i++) {a=a*0.95; q[i]=q[i-1]+a;}
  input_data(); // 读取 data
  initialization(); // 初始化
  evaluation(0);
  for(i=1; i<=GEN; i++) {
	  selection();  // 选择
	  crossover();  // 交叉
	  mutation();   // 变异
	  evaluation(i);// 生成下一代
	  printf("\nGeneration NO.%d,  ", i);	  
	  printf("Distance=%3.4f\n", OBJECTIVE[0]);
  }
  end_time=clock();
  use_time=end_time-start_time;
  printf("\n最佳路径\n");
  for(j=1; j<=N; j++) {
	  if(j<N) printf("%d->",CHROMOSOME[0][j]);
	  else printf("%d",CHROMOSOME[0][j]);
  }
  printf("->%d \n",CHROMOSOME[0][1]);
  printf("\n最短路程 %3.4f\n", OBJECTIVE[0]);
  printf("使用时间 %f \n",use_time);
  /*
  FILE *fp;
  fp=fopen("RESULT.txt","w");
  fprintf(fp,"最佳路径\n");
  for(j=1; j<=N; j++) {
	  if(j<N) fprintf(fp,"%d->",CHROMOSOME[0][j]);
	  else fprintf(fp,"%d",CHROMOSOME[0][j]);
  }
  fprintf(fp,"->%d \n",CHROMOSOME[0][1]);
  fprintf(fp,"\n最短路程 %3.4f\n", OBJECTIVE[0]);
  fclose(fp);
  */
  //结果保存到
  ofstream f1;
  f1.open("Result_Leo.txt",ios::app);
  f1<<"遗传算法求解TSP问题"<<endl;
  f1<<"参数:"<<endl;
  f1<<"群体个数:"<<GEN<<"\t";
  f1<<"交叉概率:"<<P_CROSSOVER<<"\t";
  f1<<"变异概率:"<<P_MUTATION<<"\t";
  f1<<"基因长度:"<<POP_SIZE<<"\n";
  f1<<"最佳路径为:"<<endl;
  for(i=1;i<=N;i++)f1<<CHROMOSOME[0][i]<<"->";
  f1<<CHROMOSOME[0][1]<<endl;
  f1<<"最优路径长度为："<<OBJECTIVE[0]<<endl;
  f1<<"程序运行时间为："<<use_time<<" s"<<endl<<endl;
  f1.close();
  printf("\n");
}


static void evaluation(int gen)
{
  double a;
  int b;
  int   i, j, label;
  objective_function();
  if(gen==0){
	 OBJECTIVE[0]=OBJECTIVE[1];
	 for(j = 1; j <= N; j++) CHROMOSOME[0][j]=CHROMOSOME[1][j];
  }
  for(i=0; i<POP_SIZE; i++){
	  label=0;  a=OBJECTIVE[i];
	  for(j=i+1; j<=POP_SIZE; j++)
		 if((TYPE*a)<(TYPE*OBJECTIVE[j])) {
			 a=OBJECTIVE[j];
			 label=j;
		 }
	  if(label!=0) {
			 a=OBJECTIVE[i];
			 OBJECTIVE[i]=OBJECTIVE[label];
			 OBJECTIVE[label]=a;
		 for(j=1; j<=N; j++) {
			 b=CHROMOSOME[i][j];
			 CHROMOSOME[i][j]=CHROMOSOME[label][j];
			 CHROMOSOME[label][j]=b;
		 }
	  }
  }
}

static void selection()
{
  double r;
  int temp[POP_SIZE+1][N+1];
  int   i, j, k;
  for(i=1; i<=POP_SIZE; i++) {
	  r=myu(0, q[POP_SIZE]);
	  for(j=0; j<=POP_SIZE; j++) {
		  if(r<=q[j]) {
			  for(k=1; k<=N; k++) temp[i][k]=CHROMOSOME[j][k];
			  break;
		  }
	  }
  }
  for(i=1; i<=POP_SIZE; i++)
	 for(k=1; k<=N; k++)
		 CHROMOSOME[i][k]=temp[i][k];
}

static void crossover()
{
  int   i, j, jj, k, s, m, n, pop, TEMP[POP_SIZE+1][N+1];
  int label;
 
  pop=POP_SIZE/2;
  for(i=1; i<=pop; i++) {
	 if(myu(0,1)>P_CROSSOVER) continue;
	 j=(int)myu(1,POP_SIZE);
	 jj=(int)myu(1,POP_SIZE);
	 for(k=1; k<=N; k++) TEMP[j][k]=CHROMOSOME[j][k];
	 s=(int)myu(1,N); 
	  label=0;
	 for(k=s+1; k<=N; k++){
		 for(m=label+1; m<=N; m++){
			 for(n=1; n<=s; n++){
				 if(CHROMOSOME[jj][m]==CHROMOSOME[j][n])  goto mark1;				
			 }
			 label=m;
			 break;
mark1:
			 ;
		 }
		 CHROMOSOME[j][k]=CHROMOSOME[jj][label];
	 }
	  label=0;
	 for(k=s+1; k<=N; k++){
		 for(m=label+1; m<=N; m++){
			 for(n=1; n<=s; n++){
				 if(TEMP[j][m]==CHROMOSOME[jj][n]) goto mark2;
			 }
			 label=m;
			 break;
mark2:
			 ;
		 }
		 CHROMOSOME[jj][k]=TEMP[j][label];
	 }

  }
}

static void mutation(void)
{
  int i, j, k;
  int m, n;
  int x[N+1];
 
  for(i=1; i<=POP_SIZE; i++) {
	  if(myu(0,1)>P_MUTATION) continue;
	  m=(int)myu(1,N); n=(int)myu(1,N);
	  if(m>n) {
		  k=m; m=n; n=k;
	  }
	  for(j=m; j<=n; j++) x[j] = CHROMOSOME[i][j];
	  for(j=m; j<=n; j++) CHROMOSOME[i][j]=x[n-j+m];
  }
}

static double myu(double a, double b) // Uniform Distribution
{
  double y;
  if(a>b) {
	printf("\nThe first parameter should be less than the second!");
	exit(1);
  }
  y = (double)rand()/(RAND_MAX);
  return (a+(b-a)*y); 
}

