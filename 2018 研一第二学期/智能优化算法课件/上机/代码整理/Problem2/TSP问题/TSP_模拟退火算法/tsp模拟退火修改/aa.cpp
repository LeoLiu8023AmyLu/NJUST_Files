#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include <algorithm>
#include <math.h>
#include <fstream.h>
  
#define CityNumber 5                      //���������Ŀ
#define rate 0.01                         //�������·����ı���ϵ��
#define EndTem 0.1                        //��������¶�
#define TemRate 0.9                       //�����¶��½�����

void main()
{
	ifstream in("city_x_y.txt");          //д����е�������Ϣ
//////////////////////////////////////////////////////
	struct City_x_y//�ṹ��
	{
		int num;
		double x;
		double y;
	}City[CityNumber];
//////////////////////////////////////////////////////
	for (int p=0;p<CityNumber;p++)
	{
       in>>City[p].num>>City[p].x>>City[p].y;
       cout<<City[p].num<<" "<<City[p].x<<" "<<City[p].y<<endl;
	}
///////////////////////��������/////////
//	int InitialCityRoute[CityNumber]={1,4,2,3,5};
			int InitialCityRoute[CityNumber],//��ʼ�����м�·��
		NowCityRoute[CityNumber]={0},                   //��ʼ����ǰ·��
		NewCityRoute[CityNumber]={0},                   //��ʼ����·��
		TemCityRoute[CityNumber]={0};                   //��ʼ����ʱ·��

			for (int s=0; s<CityNumber; s++) 
	          InitialCityRoute[s] = s+1; //��ʼ��·��


    int *pt;
	int i=0,j=0,k=0,q=0,
		Times=0;                                        //ģ���˻��㷨���еĴ���
	double	Cdistancemax=0.0;                           //���м�·�������ֵ  
    double  Cdistancemin=10000;                         //·����Сֵ
	double  fc=0;                                       //·����
	int m=0,n=0;
	long NL=CityNumber*4;                                             //����loopѭ������
    double sum=0,OldRouteLong=0,NewRouteLong=0,NowRouteLong=0;        //��ʼ��·������
	double InitialTem=0,NowTem=0,d=0.0;                               //��ʼ���¶�
    
	
	ofstream fout("result.txt");                        //���������ļ���result.txt��

	                                        
/***********************������м������Ϣ************************************/
 double a[CityNumber][CityNumber];	
	
 for(i=0;i<CityNumber;i++)
		for(j=0;j<CityNumber;j++)
		{	
			double d,d1,d2;
			if(j==i) {a[i][j]=0;} //ͬ�Ǿ���0
			else
			{
			d1=City[i].x-City[j].x;//x�����
			d2=City[i].y-City[j].y;//Y�����
			d=sqrt(pow((double)d1,2)+pow((double)d2,2)); //����
			a[i][j]=d; //���о������
			}
		}
	
cout<<"������ĿΪ:  "<<CityNumber<<endl;
cout<<"���о������������ʾ��"<<endl;


for(i=0;i<CityNumber;i++)
{
	for(j=0;j<CityNumber;j++)
	{
    cout<<setw(3)<<a[i][j]<<"  ";
	fout<<setw(3)<<a[i][j]<<"  ";//setw(n)���������
	}
    cout<<endl<<endl;
	
}

/************************ѡ���ʼ·����ȷ����ʼ�¶�**************************/

    cout<<"ѡ��ĳ�ʼ·��Ϊ��"<<endl;
	fout<<"ѡ��ĳ�ʼ·��Ϊ��"<<endl;
    for( q=0;q<CityNumber;q++)
	{
	cout<<setw(2)<<InitialCityRoute[q]<<"->";
    fout<<setw(2)<<InitialCityRoute[q]<<"->";
	}
		cout<<setw(2)<<InitialCityRoute[0];
    fout<<setw(2)<<InitialCityRoute[0];
	cout<<endl<<endl;
	fout<<endl<<endl;
    
	for(i=0;i<CityNumber-1;i++)
	 {	k++;	  
	    sum+=a[i][k];//����ʼ���� ��ʼ��·�߳� ����ص����
	 }	 
     sum+=a[i][0];//�ص������ĳ���  
     OldRouteLong=sum;
     cout<<"��ʼ·���ܳ���Ϊ�� "<<OldRouteLong<<endl;
     fout<<"��ʼ·���ܳ���Ϊ�� "<<OldRouteLong<<endl;


	 for(i=0;i<CityNumber-1;i++)
	 {
		 for(j=i+1;j<CityNumber;j++)
		 {
            
				if(a[i][j]>Cdistancemax) Cdistancemax=a[i][j];//���м�·�����ֵ
			    if(a[i][j]<Cdistancemin) Cdistancemin=a[i][j];//��С
		 }
	 }

/*
	 cout<<"���м�������Ϊ�� "<<Cdistancemax<<endl;
	 cout<<"���м���С����Ϊ�� "<<Cdistancemin<<endl;
	 fout<<"���м�������Ϊ�� "<<Cdistancemax<<endl;
	 fout<<"���м���С����Ϊ�� "<<Cdistancemin<<endl;
*/
     InitialTem=rate*(Cdistancemax-Cdistancemin);
     cout<<"ȷ���ĳ�ʼ�¶�Ϊ�� "<<InitialTem<<endl;
     fout<<"ȷ���ĳ�ʼ�¶�Ϊ�� "<<InitialTem<<endl;

/*************************ѡ��ǰ·����ȷ����ǰ�¶�***************************/

  for(i=0;i<CityNumber;i++) 
  { NowCityRoute[i]=InitialCityRoute[i];//��ǰ·��
    TemCityRoute[i]=InitialCityRoute[i];//��ʱ·��
  }
  
  NowTem=InitialTem;
  cout<<"��ǰ���¶�Ϊ��   "<<NowTem<<endl;
  fout<<"��ǰ���¶�Ϊ��   "<<NowTem<<endl;

  cout<<"��ǰ����·��Ϊ�� "<<endl;
  fout<<"��ǰ����·��Ϊ�� "<<endl;
  for( q=0;q<CityNumber;q++)
  {
	cout<<setw(2)<<NowCityRoute[q]<<"->";
    fout<<setw(2)<<NowCityRoute[q]<<"->";
  }
  	cout<<setw(2)<<NowCityRoute[0];//��·����ʾ
    fout<<setw(2)<<NowCityRoute[0];


 
/***************************�� 2-opt ���������µ�·��*****************************/

mark:  //MARK����ѭ��
 
	while(1)
	{
		i= (rand()%CityNumber)-1;
        if(i>0)break;
	}

	while(1)
	{
		j=(rand()%CityNumber);

		if(i!=j && j>0)
		{
			int t;
			t=i;
			i=j;
			j=t;
			break;
		}
	}

    k=TemCityRoute[i];                         
    TemCityRoute[i]=TemCityRoute[j];       
    TemCityRoute[j]=k;

	Times++;//ģ���˻��㷨������·���Ĵ��� 

    cout<<endl<<endl;
    fout<<endl<<endl;
   	cout<<"�� "<<Times<<" �β�������·��Ϊ:"<<endl;
    fout<<"�� "<<Times<<" �β�������·��Ϊ:"<<endl;
	for(q=0;q<CityNumber;q++) NewCityRoute[q]=TemCityRoute[q];


	for( q=0;q<CityNumber;q++)	
	{
	cout<<setw(3)<<NewCityRoute[q]<<"->";//��·����ʾ
    fout<<setw(3)<<NewCityRoute[q]<<"->";
	}
	cout<<setw(3)<<NewCityRoute[0];//��·����ʾ
    fout<<setw(3)<<NewCityRoute[0];
    cout<<endl;
	fout<<endl;
   
/*****************������·���͵�ǰ·�����ܳ��ȣ�������·���뵱ǰ·����·�̲�**************/
///////////��·��**********
    pt=NewCityRoute;	

    for(sum=0,i=0;i<CityNumber-1;i++)
	{
      m=*pt;
	  m--;
	  pt++;
	  n=*pt;
	  n--;
	  sum+=a[m][n];	 	  
    }

	m=*pt-1;
	sum+=a[m][0];
    NewRouteLong=sum;
	cout<<"��·�����ܳ���Ϊ:      "<<NewRouteLong<<endl;
    fout<<"��·�����ܳ���Ϊ:      "<<NewRouteLong<<endl;

    pt=NowCityRoute;	

    for(sum=0,i=0;i<CityNumber-1;i++)
	{
      m=*pt;
	  m--;
	  pt++;
	  n=*pt;
	  n--;
	  sum+=a[m][n];	 //����ǰ·�����ߵĳ������	  
    }

	m=*pt-1;//���һ�������ھ��������������
	sum+=a[m][0];//�ص���ʼ����
    NowRouteLong=sum;//��·������ 
	cout<<"��ǰ·�����ܳ���Ϊ:    "<<NowRouteLong<<endl;
	fout<<"��ǰ·�����ܳ���Ϊ:    "<<NowRouteLong<<endl;

	fc=NewRouteLong-NowRouteLong;//��·���͵�ǰ·�����Ȳ�
	cout<<"��·���뵱ǰ·����Ϊ�� "<<fc<<endl;
	fout<<"��·���뵱ǰ·����Ϊ�� "<<fc<<endl;



      
	if(fc<=0)//��·���Ƚ϶�  ������·��
	  {
		for(i=0;i<CityNumber;i++) NowCityRoute[i]=NewCityRoute[i];//����·��ȡ����ǰ·��
    	cout<<"��ǰ����·��Ϊ��    "<<endl;
        fout<<"��ǰ����·��Ϊ��    "<<endl;
		for( q=0;q<CityNumber;q++)	
		{
    	cout<<setw(3)<<NowCityRoute[q]<<"->";
		fout<<setw(3)<<NowCityRoute[q]<<"->";
		}
		 cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
        cout<<endl<<endl;
		fout<<endl<<endl;

loop:	NL--;//����loopѭ������     *******�������ж���ѭ���������Ƿ�ﵽֹͣ�¶�*****ÿ������·����Ҫ��ת����������ж�****
		if(NL>0)
		  goto mark;//2-opt ���������µ�·��  ��ʼ���ѡ����·��
		else 
		{			
			if(NowTem<EndTem)
			{		                 			  
			  int *p=NowCityRoute;
			  cout<<endl<<endl;
		      fout<<endl<<endl;
		      cout<<"��ǰ�¶�Ϊ�� "<<NowTem<<endl; 
	          cout<<"ѭ����������ʱ���·��Ϊ:"<<endl;
			  fout<<"��ǰ�¶�Ϊ�� "<<NowTem<<endl; 
	          fout<<"ѭ����������ʱ���·��Ϊ:"<<endl;
		      for( q=0;q<CityNumber;q++)
			  {
    	      cout<<setw(3)<<NowCityRoute[q]<<"->";
              fout<<setw(3)<<NowCityRoute[q]<<"->";
			  }
			   cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
              cout<<endl; 
              fout<<endl;

			  cout<<"��ʱ��Ӧ��·���ܳ���Ϊ��   ";
			  fout<<"��ʱ��Ӧ��·���ܳ���Ϊ��   ";

                for(sum=0,i=0;i<CityNumber-1;i++)//��������Ķ���NowCityRoute
				{
                    m=*p;
	                m--;
	                p++;
	                n=*p;
	                n--;
	                sum+=a[m][n];	 	  
				}

  	        m=*p-1;
        	sum+=a[m][0];

			cout<<sum<<endl<<endl;
			fout<<sum<<endl<<endl;
			}
			else //  ��ѭ������
			{
		      NowTem=TemRate*NowTem;
		      cout<<"��ǰ�¶�Ϊ�� "<<NowTem<<endl<<endl;
              fout<<"��ǰ�¶�Ϊ�� "<<NowTem<<endl<<endl;
			  NL=CityNumber*4;//����loopѭ������  ���½���ѡ�� 
		      goto mark;
			}		
		}
	  }
     else 
	 {
      double x=0.0,y=0.0;
      x=(rand()%100);
	  x=x/100;
      y=exp(-fc/NowTem);//�������һ��ֵXȥ��y�Ƚ�  ���y>x  �ͽ�����·��
        if(y>x)
		{
          for(i=0;i<CityNumber;i++) NowCityRoute[i]=NewCityRoute[i];
		  cout<<"��ǰ����·��Ϊ��     "<<endl;
          fout<<"��ǰ����·��Ϊ��     "<<endl;
		  for( q=0;q<CityNumber;q++)
		  {
    	  cout<<setw(3)<<NowCityRoute[q]<<"->";
          fout<<setw(3)<<NowCityRoute[q]<<"->";
		  }
		  cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
		  cout<<endl<<endl; 
		  fout<<endl<<endl;
       	  goto loop; 
		 } 
		else 	//���y<x  �Ͳ�������·��  �����ǰ·��
		{ 
			cout<<"��ǰ����·��Ϊ��     "<<endl;
            fout<<"��ǰ����·��Ϊ��     "<<endl;
		     for( q=0;q<CityNumber;q++)
			 {
    	         cout<<setw(3)<<NowCityRoute[q]<<"->";
                 fout<<setw(3)<<NowCityRoute[q]<<"->";
			 }
			  cout<<setw(3)<<NowCityRoute[0];
          fout<<setw(3)<<NowCityRoute[0];
			 cout<<endl<<endl; 
			 cout<<endl<<endl; 
		     goto loop;//��¼��ѭ������ GOTO MARK���²�����·�� �����ж�
		}
	  }
}