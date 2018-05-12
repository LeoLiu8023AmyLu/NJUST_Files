
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#define Num_city 48

double coord[Num_city][2]; //�������
double d[Num_city][Num_city];
double pc=0.2;
double pm=0.1;
int pop[100][Num_city]; //���볤��ΪNum_cityλ,�ֱ����Num_city������
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
    	cross_P=(rand()%101)/100.0;//����������
    	cross_pos=rand()%Num_city;//�������λ��
    	if((cross_P<=pc)&&(cross_pos!=0)){//��λ��Ϊ0���򲻽��н������
	    	for(j=cross_pos;j<Num_city;j++){//�ӽ���λʼ���н���
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
			mut_p=(rand()%101)/100.0;//����������
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
	cout<<"\n   �Ŵ��㷨���TSP����\n";
    cout<<endl;
	cout<<"��������Ⱥ��С(20-100):";
    cout<<endl;
	cin>>pop_size;
    cout<<"���������������(100-300):";
	cout<<endl;
	cin>>maxgen;
    cout<<"������������:"<<"\n";

//*******************�������**********************//
    int code[100][Num_city];//������
	double adapt_best=100;//��Ѹ���
	int best_individual[Num_city];//���Ÿ���
	double adapt_sum[200];//��ȡ������Ӧ�Ⱦ�ֵ
	int i=0;
	int j=0;
	int t=0;
	for(i=0;i<maxgen;i++){			
				adapt_sum[i]=0;//��ʼ��
			}
	for(i=0;i<pop_size;i++){
		for(j=0;j<Num_city;j++){
			pop[i][j]=j;
			code[i][j]=j;//��ʼ����ģ��
			//cout<<pop[i][j]<<" ";
		}//cout<<"\n";
    }
    //cout<<"�״α������"<<"\n";
	for(i=0;i<pop_size;i++){
		for(j=0;j<Num_city;j++){
            t=rand();
			pop[i][j]=t%(Num_city-j);//grefenstette����
			//cout<<pop[i][j]<<" ";
		}//cout<<"\n";
    }
  	
	ifstream infile("data.txt",ios::in|ios::nocreate);
	if(!infile){
		cout<<"���ܴ��ļ�\n";
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
	cout<<"�����������"<<endl;
	for(i=0;i<Num_city;i++)
	{
		for(j=0;j<Num_city;j++)
			cout<<d[i][j]<<" ";
			cout<<endl;
	}
//*******************��ѭ����ʼ********************//
for(int G=0;G<maxgen;G++)
	{
//*******************�������**********************//
	    crossover();
	//	cout<<"�������"<<"\n";
	//	for(i=0;i<pop_size;i++){
	//		for(j=0;j<Num_city;j++){
	//			cout<<pop[i][j]<<" ";
	//		}  cout<<"\n";
	//	}

//*******************�������**********************//
		int temp[100][Num_city];
		int k=0;
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				code[i][j]=j;//����ģ��
			}
		}
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				temp[i][j]=code[i][pop[i][j]];//����
				if(pop[i][j]!=Num_city-1)//�ų���һ�ν�����ֱ�����ֵ���һ�����е����
					for(k=(pop[i][j]+1);k<Num_city;k++){
						code[i][k-1]=code[i][k];//��λǰ�ƣ����ȡ���¿�ȱ
					}
			}
		}
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				pop[i][j]=temp[i][j];
			}
		}
	//	cout<<"�������"<<"\n";
	//	for(i=0;i<pop_size;i++){
	//		for(j=0;j<Num_city;j++){
	//			cout<<pop[i][j]<<" ";
	// 		}  cout<<"\n";
	//	}

//*******************�������**********************//
        mutation();
	
        // cout<<"�������"<<"\n";
		// for(i=0;i<pop_size;i++){
		// 	  for(j=0;j<=Num_city;j++){
		//		 cout<<pop[i][j]<<" ";
		//  	} cout<<"\n";
		//  }

//*******************������Ӧֵ********************//
		double adapt[100];//��Ⱥ��Ӧֵ֮��		
		int best_pos=0;//��Ѹ�������Ⱥ�е�λ��

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
				best_pos=i;//��¼��Ѹ�������Ⱥ�е�λ��
				}
			}
		}cout<<"\n"<<"��"<<G+1<<"�����·�������г�"<<adapt_best;
		//cout<<"\n"<<"��"<<G+1<<"�����·������Ⱥ�е�λ��"<<best_pos;

//*******************���Ų���**********************//
        for(j=0;j<Num_city;j++){
			best_individual[j]=0;//��ʼ��
		}
		//cout<<"\n"<<"������Ѹ���"<<"\n";		
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
		//cout<<"\n"<<"��ǰ���·�����Ϊ"<<w<<" ";

//*******************ѡ�����**********************//
		double ada_sum=0;
		double ada_temp=0;//��ʼ���ۼ�ֵΪ0
		int new_pop[100][Num_city];//��ȡ����Ⱥ		
		double r=0;			
		for(i=0;i<pop_size;i++){
			ada_sum+=adapt[i];//������Ⱥ�и�����Ӧ��֮��
		}//cout<<ada_sum<<"\n";
		adapt_sum[G]=ada_sum/pop_size;//������Ⱥ����ƽ����Ӧ��
		for(i=0;i<pop_size-1;i++){
			r=(rand()%1001)/1000.0;//����0��1���С��
			r=r*(pop_size*100-ada_sum);//cout<<r<<"\n";
			j=0;
			for(ada_temp=0;ada_temp<=r;ada_temp=ada_temp+(100-adapt[j])){
				j=j++;				
			}
            j=j-1;//�˳�ѭ��ʱ��jֵ��Ϊ��ѡ��ĸ������
			//cout<<j<<"\n";
			for(k=0;k<Num_city;k++){
				new_pop[i][k]=pop[j][k];//ȡ����ѡ��ĸ����������Ⱥ
			}
		}
		for(j=0;j<Num_city;j++){
			new_pop[pop_size-1][j]=best_individual[j];//���Ž�������һ������
		}
		//cout<<"ѡ�����"<<"\n";
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				pop[i][j]=new_pop[i][j];
				//cout<<pop[i][j]<<" ";
			}//cout<<"\n";
		}


//*******************�������**********************//
	int l=Num_city-1;
		int m=0;
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				code[i][j]=j;//����ģ��
			}
		}
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
				for(k=0;k<=l;k++){
					if(pop[i][j]==code[i][k]){//����pop��Ԫ�������code�е�λ��
						if(k!=Num_city-1){
							temp[i][j]=k;//����ģ����б���
							for(m=k;m<=l-1;m++){
							    code[i][m]=code[i][m+1];//��λǰ�ƣ����ȡ���¿�ȱ
							}
                            l=l-1;
						}
                        else {temp[i][j]=k;
                            l=l-1;
						}
					}
				}
			}
			l=Num_city-1;//��ԭlֵ
		}
        //cout<<"�������"<<"\n";
		for(i=0;i<pop_size;i++){
			for(j=0;j<Num_city;j++){
			    pop[i][j]=temp[i][j];
				//cout<<pop[i][j]<<" ";
			}//cout<<"\n";
		}
	}
//*******************��ѭ������********************//


//*******************�������**********************//

 cout<<"\n"<<"\n";
 cout<<"������Ⱥƽ����Ӧ��"<<"\n";
    for(i=0;i<maxgen;i++){
	    cout<<adapt_sum[i]<<" ";
	}
 cout<<"\n"<<"\n"<<"���·��Ϊ"<<"\n";
    for(j=0;j<Num_city;j++){
    	cout<<best_individual[j]<<"-";
	}
 cout<<best_individual[0];

 cout<<"\n"<<"\n"<<"���·�������г�"<<adapt_best<<"(��λ��1000km)"<<"\n";
	fstream result;  
	result.open("������.txt", ios::app);  
	if(!result)  
	{  
		cout<<"can't open the <������.log> file!\n";  
		exit(0);  
	}  
	result<<"*-------------------------------------------------------------------------*"<<endl;  

	result<<"����·��:"<<endl;
	for(j=0;j<Num_city;j++){
		result<<best_individual[j]<<"-";
	}
	result<<best_individual[0];
	result<<"\n"<<"\n"<<"���·�������г�"<<adapt_best<<"(��λ��1000km)"<<"\n";
	result<<"*-------------------------------------------------------------------------*"<<endl<<endl;  
	result.close();  

}
