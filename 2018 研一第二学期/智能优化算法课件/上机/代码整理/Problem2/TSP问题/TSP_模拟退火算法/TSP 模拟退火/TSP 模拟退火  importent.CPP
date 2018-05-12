#include <fstream.h>  //文件输入输出流，程序中使用文件
#include <ios.h>   //定义了打开方式枚举类型
#include <math.h>   //数学公式
#include <stdlib.h>  //srand随机数发生器
#include <time.h>   
#define M 70   //城市数
#define Tmin 0.001  //终止温度

//////////////////////////////////////////////////////////////定义城市结构体
struct City                                 
{
    int m_Index;
    double m_CoordinateX;
    double m_CoordinateY;
    City()
    {
        m_Index = 0;
        m_CoordinateX = 0.0;
        m_CoordinateY = 0.0;
    }
};

/////////////////////////////////////////////////////////////函数声明 形参

bool Input(City tmpCity[M]);
void GetDistence(City tmpCity[M], double tmpDistence[M][M]);
bool InitParam();
double GetPath(int tmpS[M+1]);
double GenerateChangeCity(int &C1, int &C2);
void GeneratePath(double DDistences, int C1, int C2);
void Annealing();
void Output();

/////////////////////////////////////////////////////////////定义的都是全局变量，在整个源程序中可以使用，区别于{}中定义的变量，为局部变量。
ifstream infile;      //定义输入文件的对象
ofstream outfile;     //定义输出文件的对象
double Distences[M][M];
City Citys[M];
int S[M+1],BestPathWay[M+1];
double T;
long Tl;
double a;
long k;
double PreMin, Path, BestPath;

////////////////////////////////////////////////////////////////文档输入  给城市结构体
bool Input(City tmpCity[M])
{
    char strGet[20];
    cout<<"Please input the Data File's Name: "<<endl;
    cin.getline(strGet,20);    //字符串输入函数，输入值给strGet,自动输入结束符
    infile.open(strGet,ios::in|0x20);  //用按位或|将几种打开方式组合 
    if(! infile)    //判断打开是否成功
    {
        cout<<"Can't open the Data File!"<<endl;
        return false;
    }
    for(int i=0; i<M; i++)
    {
        infile>>tmpCity[i].m_Index>>tmpCity[i].m_CoordinateX>>tmpCity[i].m_CoordinateY;
    }
    infile.close();
    return true;
}

////////////////////////////////////////////////////////////////////该函数计算任意两个城市之间的距离
void GetDistence(City tmpCity[M], double tmpDistence[M][M])//二维数组tmpDistence里面保存了所有城市两两间的距离
{
    for (int i=0; i<M; i++)
        for (int j=0; j<M; j++)
        {
            if (i == j)
                tmpDistence[i][j] = 0;
            else if (i < j)
            {
                tmpDistence[i][j] = sqrt(pow((tmpCity[i].m_CoordinateX-tmpCity[j].m_CoordinateX),2)+pow((tmpCity[i].m_CoordinateY-tmpCity[j].m_CoordinateY),2));
            }
            else tmpDistence[i][j] = tmpDistence[j][i];
        }
}

////////////////////////////////////////////////////////初始化函数，参数都是全局变量，所以可以在函数中直接初始化
bool InitParam()
{
    for (int i=0; i<M; i++) S[i] = i;  //顺序初始化为01234.。。。   注意0表示第一个城市
    S[M] = 0;//城市行走的顺序
    T = 1000;    //起始温度
    Tl = 3500;  //内循环次数
    a = 0.92;    //温度下降系数
    k = 0;  //一次退火中温度改变次数
    PreMin = 0; //上一次的最短路径
    Path = GetPath(S);   //实参只写数组名
    srand((unsigned)time( NULL )); 
    //cout<<"The initial Path Distence is: "<<Path<<endl;
	cout<<"初始路径总距离是: "<<Path<<endl;
    return true;
}

/////////////////////////////////////////////////////////////////////
double GetPath(int tmpS[M+1])   //形参为数组，M个城市 要回到原点 间隔M+1 包含有M+1个元素  计算这种顺序下路径总长度
{
    double tmp = 0;
    for (int i=0; i<M; i++)
        tmp = tmp + Distences[tmpS[i]][tmpS[i+1]];    
    return tmp;
}

////////////////////////////////////////////改变城市行走顺序 并计算和前一次的路程差
double GenerateChangeCity(int &C1, int &C2)   //一般形参是单向传递，只把实参的值给形参在函数中做运算而不改变实参的值；                                              
{                                             //  引用主要用于函数之间的数据传输，引用实质是变量的别称，用引用作形参会改变
    int tmp;                                  // 实参的值。
    double S2, S1;
    while (C1 == C2)
    {
        C1= (rand() % (M-1))+1;
        C2= (rand() % (M-1))+1;
        if (C1 > C2)
        {
            tmp = C1;
            C1 = C2;
            C2 = tmp;//使得C1 < C2
        }
    }
//  S2 = Distences[S[C1-1]][S[C2]]+Distences[S[C2]][S[C1+1]]+Distences[S[C2-1]][S[C1]]+Distences[S[C1]][S[C2+1]];
//  S1 = Distences[S[C1-1]][S[C1]]+Distences[S[C1]][S[C1+1]]+Distences[S[C2-1]][S[C2]]+Distences[S[C2]][S[C2+1]];
    S1 = Path;//当前路径距离总长
    
	tmp = S[C1];   
    S[C1] = S[C2];
    S[C2] = tmp;  //c1 c2对应的城市顺序交换
   
	S2 = GetPath(S);//计算更新后的所以路径总长
    return S2-S1;//返回新路径和当前路径之差
}

/////////////////////////////////////////////////////////////////////////
void GeneratePath(double DDistences, int C1, int C2)//DDistences是新路径和当前路径之差  满足接收条件 城市路径改变
{
    double Probability;
    double TMP;
    if (DDistences<0)//新路径更短
        TMP = 0 - DDistences;// TMP保存差值
    TMP = DDistences;//路径之差变为正
    Probability = pow(2.71828,-(TMP/T));//double pow( double x, double y );计算x的y次幂  e = 2.718281828459   也就是exp（-(TMP/T)）
                      //要与Probability比较 看是否转移
    int ttt = rand();//产生一个随机数来进行新路径是否接受的判断
    if (DDistences<=0 || Probability>double((ttt%500)/500))//如果新路径更短或者 产生的随机数满足转移条件  那么接受更新
//  if (DDistences<=0 || Probability>0.8)
    {
        Path = Path + DDistences;//DDistences是负数  表示path长度缩短
        if (DDistences<=0) 
        {
            BestPath = Path;
            for (int m=0; m<=M; m++)
                BestPathWay[m] = S[m]+1; //比如s[0]里面值为2 其实是表示第三个城市 所以加1
        }
        return; //GenerateChangeCity函数里面已经交换过C1C2城市位置 所以如果接受这种变化 那么直接return
    }
    int i = S[C2];     //如果产生的随机数结果不接受这种更新 那么将C1C2交换回来。GenerateChangeCity里面交换了一次，相当于复原回原来的路径顺序  
    S[C2] = S[C1];
    S[C1] = i;   
}

//////////////////////////////////////////////////////////////////////
void Annealing()  //退火算法
{
    int i,j;
    double DDistence = 0;
    while (T>Tmin)//大于终止温度  降温不停内循环 每个温度输出一个当前路径
    {
        PreMin = Path;//当前路径总长作为上一次的最短路径记录
        for (i=0; i<Tl; i++)//Tl内循环次数
        {
            int C1 = 0;//GenerateChangeCity函数参数是C1C2的引用，所以其内部处理会改变C1C2的实际值
            int C2 = 0;
            DDistence = GenerateChangeCity(C1,C2);
            GeneratePath(DDistence,C1,C2);
        } //在内循环规定次数内不停更新行走路径 并判断是否接受这种改变

       // cout<<"The current Tempreture is:"<<T<<endl;//写出当前温度
      //  cout<<"Thre current Path is: "; //当前路径
		cout<<"当前温度是:"<<T<<endl;//写出当前温度
		cout<<"当前路径是: "; //当前路径
        for (j=0; j<M; j++)
        {

            cout<<S[j]+1<<"->"; //+1前面说的原因
        }
        cout<<S[M]+1<<endl;//S[M]+1 是城市1 即回到原点
       // cout<<"The current Path Distence is: "<<Path<<endl;//当前总长
		cout<<"当前路径总距离: "<<Path<<endl;//当前总长
        T = T * a;//温度下降系数a  降温
        k++;//温度下降次数+1

    }//在每一个温度T下完成T1次内循环，然后降温，在下一个T里面继续内循环T1次。
}

//////////////////////////////////////////////////////////////////////
void Output()  //输出到文件TXT   达到终止温度后输出到TXT  这是一次退火算法得到的最佳结果
{ 
   // outfile<<"The best Path is : "<<endl;
	outfile<<"最佳路径是: "<<endl;
    for (int i=0; i<M; i++)
        outfile<<BestPathWay[i]<<"->";
        outfile<<BestPathWay[M]<<endl;
       // outfile<<"The best Path Distence is: "<<Path<<endl<<endl;
		outfile<<"最佳路径距离是: "<<Path<<endl<<endl;
}

///////////////////////////// main function //////////////////////////////
void main()
{
    if (!Input(Citys)) //城市数据TXT文档读入  正确为1
        return;
    GetDistence(Citys,Distences);//Distences全局变量 所以GetDistence函数计算的所有城市两两距离保存在里面（形参不是单向传递的么？。。。）
    char strOut[] = "OutPut.txt";
    outfile.open(strOut);//打开的文档名字
    outfile<<"模拟退火算法参数: "<<endl;
    outfile<<"初始温度: 1000"<<endl;
    outfile<<"内循环次数: 3500"<<endl;
    outfile<<"终止温度: 0.001"<<endl;
    outfile<<"温度下降系数: 0.92"<<endl<<endl;
	//clock()是C/C++中的计时函数，而与其相关的数据类型是clock_t  clock_t clock(void) ; 函数返回从“开启这个程序进程”到“程序中调用clock()函数”时之间的CPU时钟计时单元（clock tick）数
    //每过千分之一秒（1毫秒），调用clock（）函数返回的值就加1
	clock_t start,finish;
    start = clock();//开始计时
    for (int k=0; k<20; k++) ////k是个局部变量，全局变量k在这里被隐藏了 全局变量遇到同名的局部变量，全局变量在这个函数中暂时消失
    {
        outfile<<"第"<<k+1<<"次退火: "<<endl;
        InitParam();//初始化
        Annealing();//退火
        Output();//输出 
    }                           //这里进行了20次退火  互相之间是独立的
    finish = clock();//计时结束   finish-start记录所用时间
    outfile<<endl<<"运行20次共花费"<<finish-start<<"毫秒"<<endl;
    outfile.close();
}
