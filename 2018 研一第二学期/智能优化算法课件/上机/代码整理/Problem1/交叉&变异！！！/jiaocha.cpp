#include <iostream>
#include <ctime>  //把日期和时间转换为字符串
#include <cstdlib> //C++里面的一个常用函数库
using namespace std;
#define N 30
int main()
{
    int num1[N],num2[N];
	int jiaocha[4];
	int u;
	int m[4];  //中间缓存
	double random(double,double );
    srand(unsigned(time(0)));
	for(int i = 0; i < N; i++)
	{ 
		num1[i] =int(random(0,2));
	    num2[i] =int(random(0,2));
	} 
		cout<<"生成的二进制数为：\n" ;
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

	cout<<"交叉位分别为：";
    cout<<jiaocha[0]<<"  "<<jiaocha[1]<<"  "<<jiaocha[2]<<"  "<<jiaocha[3]<<"\n";
    for(int q = 0; q < 4; q++)
	{	
		m[jiaocha[q]] = num1[jiaocha[q]];//交叉位调换
	    num1[jiaocha[q]] = num2[jiaocha[q]];
		num2[jiaocha[q]] = m[jiaocha[q]];
    }
	cout<<"交叉后二进制数为：\n" ;
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
