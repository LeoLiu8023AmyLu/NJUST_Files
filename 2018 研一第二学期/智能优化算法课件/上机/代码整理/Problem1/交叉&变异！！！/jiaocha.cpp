#include <iostream>
#include <ctime>  //�����ں�ʱ��ת��Ϊ�ַ���
#include <cstdlib> //C++�����һ�����ú�����
using namespace std;
#define N 30
int main()
{
    int num1[N],num2[N];
	int jiaocha[4];
	int u;
	int m[4];  //�м仺��
	double random(double,double );
    srand(unsigned(time(0)));
	for(int i = 0; i < N; i++)
	{ 
		num1[i] =int(random(0,2));
	    num2[i] =int(random(0,2));
	} 
		cout<<"���ɵĶ�������Ϊ��\n" ;
		cout<<"num1 = ";
		for(u = 0;u<N;u++)
			cout<<num1[u];
		cout<<endl;
		cout<<"num2 = ";
        for(u = 0;u<N;u++)
			cout<<num2[u];
		cout<<endl;
    for(int j = 0; j < 4; j++)
		jiaocha[j] = int(random(0,N));

	cout<<"����λ�ֱ�Ϊ��";
    cout<<jiaocha[0]<<"  "<<jiaocha[1]<<"  "<<jiaocha[2]<<"  "<<jiaocha[3]<<"\n";
    for(int q = 0; q < 4; q++)
	{	
		m[jiaocha[q]] = num1[jiaocha[q]];//����λ����
	    num1[jiaocha[q]] = num2[jiaocha[q]];
		num2[jiaocha[q]] = m[jiaocha[q]];
    }
	cout<<"������������Ϊ��\n" ;
	cout<<"num1 = ";
	for(u = 0;u<N;u++)
			cout<<num1[u];
		cout<<endl;
	cout<<"num2 = ";
     for(u = 0;u<N;u++)
			cout<<num2[u];
	 	cout<<endl;
	return 0;
}


double random(double  start, double  end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}
