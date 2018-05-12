#include<iostream.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define U 1000                       //��ѭ������
#define L 100                        //��ѭ������
#define K 20.0                       //����ȷ���˻��¶ȵĳ���
#define N 50                         //��Ʒ����
static int r=1000;                   //��������

//--------------------�����ȡ�¶Ⱥ���----------------------
double get_init_temp(int va1[],int na,int vo1[], int no)      //va1[]Ϊ����Ʒ�ļ�ֵ��vo1[]Ϊ����Ʒ�������na��no��Ϊ��Ӧ�ĸ�����
{
	double p[N],temp1,T0;
	int i,j,temp2;
	int q[N];
	for(i=0;i<N;i++)
	{
		p[i]=float(va1[i])/float(vo1[i]);                     //p[i]Ϊ��ֵ�����֮��
		q[i]=i;                                               //��Ʒ���
	}
	for(i=0;i<N;i++)
		for(j=i;j<N;j++)
		{
			if(p[i]<p[j])                                     //����ֵ���ĸ���p[i]
			{
				temp1=p[i];
				p[i]=p[j];
				p[j]=temp1;


				temp2=q[i];                                   //����ֵ������Ʒ��Ÿ���q[i]
				q[i]=q[j];
				q[j]=temp2;
			}
		}
    int	v_max=0;                                              //��ʼ�����װ����
    int v_min=0;                                              //��ʼ����Сװ����
	int rr;                                                   //װ����Ʒ������� 
	int k;
	for(rr=0,i=0;i<N,rr<r;i++)                                //������װ����
	{

         k=q[i];
         v_max+=va1[k];
         rr=rr+vo1[k];
	}
    for(rr=0,i=N-1;i>=0,rr<r;i--)                             //�����Сװ����
	{
        k=q[i];
        v_min=v_min+va1[k];
        rr=rr+vo1[k];
	}
    T0=(v_max-v_min)*K;                                       //����¶�T0
	return T0;
}

void main(void)
{
	int va[N]={220,208,198,192,180,180,165,162,160,158,
               155,130,125,122,120,118,115,110,105,101,
               100,100,98,96,95,90,88,82,80,77,
               75,73,72,70,69,66,65,63,60,58,
               56,50,30,20,15,10,8,5,3,1};                    //����Ʒ�ļ�ֵ
    int vo[N]={80,82,85,70,72,70,66,50,55,25,
               50,55,40,48,50,32,22,60,30,32,
               40,38,35,32,25,28,30,22,50,30,
               45,30,60,50,20,65,20,25,30,10,
                20,25,15,10,10,10,4,4,2,1};                   //����Ʒ��Ӧ�����
    int *point1,int *point2;  
	point1=va;                                                //ָ��point1ָ������va
	point2=vo;                                                //ָ��point2ָ������vo
    
    int best_value=0;                                         //��������ֵ
    int best_volumn=0;                                        //����������
    int pa_best[N];                                           //�������װ�ط���


    int va_init=0;                                           //�����ʼ����ֵ
    int vo_init=0;                                           //�����ʼ�����
    int pa_init[N];                                          //�����ʼ��װ�ط���

    int va_pre=0;                                            //װ��������Ʒǰ���ܼ�ֵ
    int vo_pre=0;                                            //װ��������Ʒǰ�������
    int pa_pre[N];                                           //װ��������Ʒǰ��װ�ط���

	int va_current=0;                                        //װ��������Ʒ����ܼ�ֵ
    int vo_current=0;                                        //װ��������Ʒ����ܼ�ֵ
    int pa_current[N];                                       //װ��������Ʒ����ܼ�ֵ

    int k=0 ,i,j,m=0;                               
		
//---------------��ʼ����������----------------
    while(vo_init<r)                                         //װ����Ʒ
	{
         va_init=va_init+va[k];
         vo_init=vo_init+vo[k];
         pa_init[k]=1;
         k=k+1;
	}
	k=k-1;
	va_init=va_init-va[k];                                   //�����Ų���ȥ���Ǹ���Ʒ�õ�
    vo_init=vo_init-vo[k];

	for(j=k;j<N;j++)pa_init[j]=0;
	for(i=0;i<N;i++)                                         //����װ�ط���
	{
		pa_current[i]=pa_init[i];
		pa_best[i]=pa_init[i];
	}
	                          
    k=0;                                   
    for(i=0;i<N;i++)                                         //��������ֵ��������
	{
		best_value=best_value+va[i]*pa_best[i];
        best_volumn=best_volumn+vo[i]*pa_best[i];
	}
//--------------��ʼ������--------------

    double t0=get_init_temp(point1,N,point2,N),tk=t0;        //��ȡ��ʼ���¶�t0���ٸ���tk
    int n_rec=0;
    int n_rej=0;
    int v_cnt=1;
    int df;
    k=0;                                     
    va_pre=va_init;                                          //����ʼ����ֵ����va_pre                                 
    vo_pre=vo_init;                                          //����ʼ���������vo_pre
    for(i=0;i<N;i++)pa_pre[i]=pa_init[i];                    //����ʼ��װ�ط�������pa_pre[i]              

//-------------��ѭ����ʼ-------------
	while(tk>=0.0001)
	{	
		srand((unsigned)time(NULL));

//-------------��ѭ����ʼ-------------
		while(k<U)
		{                   
		    int rand1,rand2,tmp;                             //�������������  
		    
            rand1=rand()%N;
            rand2=rand()%N;
		    
            if(rand2<rand1)
			{
			   tmp=rand2;
		       rand2=rand1;
	           rand1=tmp;
			} 
	
            for(;rand1<=rand2;rand1++)                       //�����������֮�������װ�ط�ʽȡ��
               pa_current[rand1]=!pa_current[rand1];
            
//-----------��ǰװ�ط���---------------
            m=0;
            while(vo_current<r && m<N)                       //���װ����С�ڱ�����������װ��������50���ͼ���װ����Ʒ
			{
				va_current=va_current+va[m]*pa_current[m];
                vo_current=vo_current+vo[m]*pa_current[m];
                m=m+1;
			}
            m=m-1;
            va_current=va_current-va[m]*pa_current[m];       //�õ�����Ǹ�װ���µ���Ʒ
            vo_current=vo_current-vo[m]*pa_current[m];
            for(j=m;j<N;j++)pa_current[m]=0;
            df=va_current-va_pre;
			
		    double d;
		    d=double((rand() %10)/10.0);
        
//-------------�о��Ƿ�����µ�װ�ط���---------
            if(df>0 || exp(df/tk)>d)                        //��������������ͱ���Ϊ�µķ���
			{
                   v_cnt=v_cnt+1;     
                   va_pre=va_current;
		           for(m=0;m<N;m++)pa_pre[m]=pa_current[m];
                   n_rec=n_rec+1;
                   best_value=va_current;
                   best_volumn=vo_current;
		           for(m=0;m<N;m++)pa_best[m]=pa_current[m];            
			} 
			
            else                                            //����������������Ͳ�����
				n_rej=n_rej+1;                                         
                k=k+1;
				va_current=0;
				vo_current=0;
            if(n_rec>L)break;                                           
			else
                 continue;
		}
//---------------��ѭ������---------------

		tk=tk*0.95;                                        //�����¶ȣ�������һ��ѭ��         
        n_rec=0;
        n_rej=0;
        k=1;
	}	
		system("cls");		
//-----------------��ѭ������-------------

//------------------������--------------
        printf("-------------------------------���------------------------------\n");
        cout<<"��ʼ�¶�:"<<t0<<endl;
        cout<<"����¶�:"<<tk<<endl;
        cout<<"��󱳰���ֵ:"<<best_value<<endl;
        cout<<"��󱳰����:"<<best_volumn<<endl;
        cout<<"���·��:"<<endl;
        for(i=0;i<N;i++)printf("%d->",pa_best[i]);
		getch();
}

