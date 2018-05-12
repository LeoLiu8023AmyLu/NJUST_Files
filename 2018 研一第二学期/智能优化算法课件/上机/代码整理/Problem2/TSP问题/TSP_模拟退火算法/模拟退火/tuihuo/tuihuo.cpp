#include <fstream.h>  
#include <ios.h>   
#include <math.h>   
#include <stdlib.h>  
#include <time.h>   
#define M 170   
#define Tmin 0.001  


struct City								    
{
	int m_Index;
	double m_X;
	double m_Y;
	City()
	{
		m_Index = 0;
		m_X = 0.0;
		m_Y = 0.0;
	}
};


bool Input(City tmpCity[M]);
void GetDistence(City tmpCity[M], double tmpDistence[M][M]);
bool InitParam();
double GetPath(int tmpS[M+1]);
double GenerateChangeCity(int &C1, int &C2);
void GeneratePath(double DDistences, int C1, int C2);
void Annealing();
void Output();


ifstream infile;      
ofstream outfile;     
double Distences[M][M];
City Citys[M];
int S[M+1],BestPathWay[M+1];
double T;
long Tl;
double a;
long k;
double PreMin, Path, BestPath;

//���庯��

bool Input(City tmpCity[M])
{
	char strGet[20];
	cout<<"Please input the Data File's Name: "<<endl;
	cin.getline(strGet,20);    
	infile.open(strGet,ios::in|0x20);   
	if(! infile)    
	{
		cout<<"Can't open the Data File!"<<endl;
		return false;
	}
	for(int i=0; i<M; i++)
	{
		infile>>tmpCity[i].m_Index>>tmpCity[i].m_X>>tmpCity[i].m_Y;
	}
	infile.close();
	return true;
}

//����������������֮��ľ���
void GetDistence(City tmpCity[M], double tmpDistence[M][M])
{
	for (int i=0; i<M; i++)
		for (int j=0; j<M; j++)
		{
			if (i == j)
				tmpDistence[i][j] = 0;
			else if (i < j)
			{
				tmpDistence[i][j] = sqrt(pow((tmpCity[i].m_X-tmpCity[j].m_X),2)+pow((tmpCity[i].m_Y-tmpCity[j].m_Y),2));
			}
			else tmpDistence[i][j] = tmpDistence[j][i];
		}
}

//��ʼ��
bool InitParam()
{
	for (int i=0; i<M; i++) S[i] = i;	
	S[M] = 0;
	T = 4100;    //��ʼ�¶�
	Tl = 30000;  //��ѭ������
	a = 0.9;    //�¶��½�ϵ��
	k = 0;
	PreMin = 0;
	Path = GetPath(S);   
	srand((unsigned)time( NULL )); 
	cout<<"The initial Path Distence is: "<<Path<<endl;
	return true;
}


double GetPath(int tmpS[M+1])  
{
	double tmp = 0;
	for (int i=0; i<M; i++)
		tmp = tmp + Distences[tmpS[i]][tmpS[i+1]];    
	return tmp;
}


double GenerateChangeCity(int &C1, int &C2)                                                 
{											  
	int tmp;								  
	double S2, S1;
	while (C1 == C2)
	{
		C1= (rand() % (M-1))+1;
		C2= (rand() % (M-1))+1;
		if (C1 > C2)
		{
			tmp = C1;
			C1 = C2;
			C2 = tmp;
		}
	}

	S1 = Path;
	tmp = S[C1];
	S[C1] = S[C2];
	S[C2] = tmp;
	S2 = GetPath(S);
	return S2-S1;
}


void GeneratePath(double DDistences, int C1, int C2)
{
	double Probability;
	double TMP;
	if (DDistences<0)
		TMP = 0 - DDistences;
	TMP = DDistences;
	Probability = pow(2.71828,-(TMP/T));
	int ttt = rand();
	if (DDistences<=0 || Probability>double((ttt%500)/500))

	{
		Path = Path + DDistences;
		if (DDistences<=0) 
		{
			BestPath = Path;
			for (int m=0; m<=M; m++)
				BestPathWay[m] = S[m]+1;
		}
		return;
	}
	int i = S[C2];
	S[C2] = S[C1];
	S[C1] = i;
}


void Annealing()
{
	int i,j;
	double DDistence = 0;
	while (T>Tmin)
	{
		PreMin = Path;
		for (i=0; i<Tl; i++)
		{
			int C1 = 0;
			int C2 = 0;
			DDistence = GenerateChangeCity(C1,C2);
			GeneratePath(DDistence,C1,C2);
		}
		cout<<"The current Tempreture is:"<<T<<endl;
		cout<<"Thre current Path is: ";
		for (j=0; j<M; j++)
		{
           	cout<<S[j]+1<<"->";
		}
		cout<<S[M]+1<<endl;
		cout<<"The current Path Distence is: "<<Path<<endl;
		T = T * a;;
		k++;
	}
}


void Output()
{
	outfile<<"The best Path is : "<<endl;
	for (int i=0; i<M; i++)
		outfile<<BestPathWay[i]<<"->";
		outfile<<BestPathWay[M]<<endl;
		outfile<<"The best Path Distence is: "<<Path<<endl<<endl;
}


void main()
{
	if (!Input(Citys))
		return;
	GetDistence(Citys,Distences);
	char strOut[] = "OutPut.txt";
	outfile.open(strOut);
    outfile<<"������Ŀ���: Problem 2:170-city�������㷨��ģ���˻��㷨"<<endl;
	outfile<<"��������ԣ��ѧ�ţ�117104010651"<<endl;
	outfile<<"ģ���˻��㷨����: "<<endl;
	outfile<<"��ʼ�¶�: 4100"<<endl;
	outfile<<"��ѭ������: 30000"<<endl;
	outfile<<"��ֹ�¶�: 0.001"<<endl;
	outfile<<"�¶��½�ϵ��: 0.9"<<endl<<endl;
	clock_t start,finish;
	start = clock();
	for (int k=0; k<10; k++)
	{
		outfile<<"��"<<k+1<<"���˻�: "<<endl;
		InitParam();
		Annealing();
		Output();
	}
	finish = clock();
	outfile<<endl<<"����10�ι�����"<<finish-start<<"����"<<endl;
	outfile.close();
}