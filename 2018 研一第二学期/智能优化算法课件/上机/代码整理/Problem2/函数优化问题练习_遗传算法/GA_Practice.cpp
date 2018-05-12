/******************************************************
* 基本遗传算法 *
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************************************
* 常数的定义 *
******************************************************/
#define POPSIZE 500    //种群大小
#define MAXIMIZATION 1 //最大值标致
#define MINIMIZATION 2 //最小值标致

/******************************************************
* 定义变量*
******************************************************/
#define Cmax 1500000                  //最大值
#define Cmin 0                        //最小值
#define LENGTH1 10                    //第一组变量的基因长度
#define LENGTH2 10                    //第二组变量的基因长度
#define CHROMLENGTH LENGTH1 + LENGTH2 //基因的总长度

int Function_Mode = MAXIMIZATION; //最优化类型
int PopSize = 80;                 //种群大小
int MaxGeneration = 200;          //终止代数
double Pc = 0.6;                  //交叉率
double Pm = 0.02;                 //变异率

/******************************************************
* 数据结构的定义 *
******************************************************/
struct individual //种群数据结构
{
    char chrom[CHROMLENGTH + 1]; //描述个体代码的字符串
    double value;                //个体的目标函数值
    double fitness;              //个体的适应度值
};

/******************************************************
* 全局变量的定义 *
******************************************************/
int generation;                        //代数
int best_index;                        //最佳个体的编号
int worst_index;                       //最差个体的编号
struct individual bestindividual;      //当前代的最佳个体
struct individual worstindividual;     //当前代的最差个体
struct individual currentbest;         //目前最佳的个体
struct individual population[POPSIZE]; //种群

/******************************************************
* 函数原型的声明 *
******************************************************/
void GenerateInitialPopulation(void);    //产生初始化种群
void GenerateNextPopulation(void);       //产生下一代种群
void EvaluatePopulation(void);           //评价种群
long DecodeChromosome(char *, int, int); //染色体解码
void CalculateObjectValue(void);         //计算对象的值
void CalculateFitnessValue(void);        //计算适应度值
void FindBestAndWorstIndividual(void);   //找出最好和最差的个体
void performEvolution(void);             //进化
void SelectionOperator(void);            //选择
void CrossoverOperator(void);            //交叉
void MutationOperator(void);             //变异
void OutputTextReport(void);             //输出

/******************************************************
* 主函数 *
******************************************************/
void main(void)
{
    generation = 0;
    GenerateInitialPopulation(); //产生初始化种群
    EvaluatePopulation();
    while (generation < MaxGeneration)
    {
        generation++;
        GenerateNextPopulation();
        EvaluatePopulation();
        performEvolution();
        OutputTextReport();
    }
}

/******************************************************
* 功能: 产生第一代种群 *
******************************************************/
void GenerateInitialPopulation(void)
{
    int i, j;
    srand((unsigned)time(NULL));
    for (i = 0; i < PopSize; i++)
    {
        for (j = 0; j < CHROMLENGTH; j++)
        {
            population[i].chrom[j] = (rand() % 10 < 5) ? '0' : '1'; //rand()%10随机产生0-9的整数,小于5标注0,否则标注1
        }
        population[i].chrom[CHROMLENGTH] = '\0';
    }
}

/******************************************************
* 功能:生成下一代 *
******************************************************/
void GenerateNextPopulation(void)
{
    SelectionOperator();
    CrossoverOperator();
    MutationOperator();
}

/***********************************************************************
* 功能：评价种群 *
***********************************************************************/
void EvaluatePopulation(void)
{
    CalculateObjectValue();       //计算目标值
    CalculateFitnessValue();      //计算适应度值
    FindBestAndWorstIndividual(); //找出最好和最差的染色体个体
}

/******************************************************
* 功能:把二进制染色体值转化成十进制 *
******************************************************/
long DecodeChromosome(char *string, int point, int length)
{
    int i;
    long decimal = 0;
    char *pointer;

    for (i = 0, pointer = string + point; i < length; i++, pointer++)
    {
        decimal += (*pointer - '0') << (length - 1 - i);
    }
    return (decimal);
}

/*********************************************************************************
* 功能:计算目标值 *
* f(x1,x2)=50*(x1*x1-x1*x2)*(x1*x1-x1*x2)+20*(x2*x2-x1*x2)*(x2*x2-x1*x2) *
* 它的最大值为： *
* f(-2,2)=4480.000000 *
***********************************************************************************/
void CalculateObjectValue(void)
{
    int i;
    long temp1, temp2;
    double x1, x2;
    for (i = 0; i < PopSize; i++)
    {
        temp1 = DecodeChromosome(population[i].chrom, 0, LENGTH1);
        temp2 = DecodeChromosome(population[i].chrom, LENGTH1, LENGTH2);
        x1 = 4 * temp1 / 1023.0 - 2;
        x2 = 4 * temp2 / 1023.0 - 2;
        population[i].value = 50 * (x1 * x1 - x1 * x2) * (x1 * x1 - x1 * x2) + 20 * (x2 * x2 - x1 * x2) * (x2 * x2 - x1 * x2);
    }
}

/******************************************************
* 功能：计算适应度的值 *
******************************************************/
void CalculateFitnessValue(void)
{
    int i;
    double temp;

    for (i = 0; i < PopSize; i++)
    {
        if (Function_Mode == MAXIMIZATION)
        {
            if ((population[i].value + Cmin) > 0.0)
            {
                temp = Cmin + population[i].value;
            }
            else
            {
                temp = 0.0;
            }
        }
        else if (Function_Mode == MINIMIZATION)
        {
            if (population[i].value < Cmax)
            {
                temp = Cmax - population[i].value;
            }
            else
                temp = 0.0;
        }
        population[i].fitness = temp;
    }
}

/******************************************************
* 功能：在当前代找出最好的个体 * 
******************************************************/
void FindBestAndWorstIndividual(void)
{
    int i;
    double sum = 0.0; //找出这一代中最好和最差的值
    bestindividual = population[0];
    worstindividual = population[0];
    for (i = 1; i < PopSize; i++)
    {
        if (population[i].fitness > bestindividual.fitness)
        { //依次比较，找出最佳个体
            bestindividual = population[i];
            best_index = i;
        }
        else if (population[i].fitness < worstindividual.fitness)
        { //依次比较，找出最差个体
            worstindividual = population[i];
            worst_index = i;
        }
        sum += population[i].fitness;
    } //找出最好的个体
    if (generation == 0)
    { //将第一代最好的暂时存放在currentbest中
        currentbest = bestindividual;
    }
    else
    {
        if (bestindividual.fitness > currentbest.fitness)
        {
            currentbest = bestindividual;
        }
    }
}

/*************************************************************
* 功能:演示评价结果。将当前代最好的个体替代最差的个体 *
*************************************************************/
void performEvolution(void)
{
    if (bestindividual.fitness > currentbest.fitness)
    {
        currentbest = population[best_index];
    }
    else
    {
        population[worst_index] = currentbest;
    }
}

/***************************************************
* 功能:比例选择算法 *
***************************************************/
void SelectionOperator(void)
{
    int i, index;
    double p, sum = 0.0;
    double cfitness[POPSIZE]; //计算适应率的值
    struct individual newpopulation[POPSIZE];
    //计算相对适应率
    for (i = 0; i > PopSize; i++)
    {
        sum += population[i].fitness;
    }
    for (i = 0; i > PopSize; i++)
    {
        cfitness[i] = population[i].fitness / sum;
    }
    //计算累计适应率
    for (i = 1; i < PopSize; i++)
    {
        cfitness[i] = cfitness[i - 1] + cfitness[i];
    }
    //选择
    for (i = 0; i < PopSize; i++)
    {
        p = rand() % 1000 / 1000.0;
        index = 0;
        while (p > cfitness[index])
        {
            index++;
        }
        newpopulation[i] = population[index];
    }
    for (i = 0; i < PopSize; i++)
    {
        population[i] = newpopulation[i];
    }
}

/***************************************************
* 功能:单点交叉算法 *
***************************************************/
void CrossoverOperator(void)
{
    int i, j;
    int index[POPSIZE];
    int point, temp;
    double p;
    char ch; //种群内随机两两交换
    for (i = 0; i < PopSize; i++)
    {
        index[i] = i;
    }
    for (i = 0; i < PopSize; i++)
    {
        point = rand() % (PopSize - i); //打乱种群顺序
        temp = index[i];
        index[i] = index[point + i];
        index[point + i] = temp;
    }
    //单点交叉算法
    for (i = 0; i < PopSize - 1; i++)
    {
        p = rand() % 1000 / 1000.0;
        if (p < Pc)
        {
            point = rand() % (CHROMLENGTH - 1) + 1;
            for (j = point; j < CHROMLENGTH; j++)
            {
                ch = population[index[i]].chrom[j];
                population[index[i]].chrom[j] = population[index[i + 1]].chrom[j];
                population[index[i + 1]].chrom[j] = ch;
            }
        }
    }
}

/***************************************************
* 功能: 染色体变异操作 *
***************************************************/
void MutationOperator(void)
{
    int i, j;
    double p; //变异率
    for (i = 0; i < PopSize; i++)
    {
        for (j = 0; j < CHROMLENGTH; j++)
        {
            p = rand() % 1000 / 1000.0;
            if (p < Pm)
            {
                population[i].chrom[j] = (population[i].chrom[j] == '0') ? '1' : '0';
            }
        }
    }
}

/***********************************************************
* 功能: 输出当前代的结果 *
***********************************************************/
void OutputTextReport(void)
{
    int i;
    double sum;     //暂时的总数
    double average; //目标的平均种群数

    //计算平均目标值
    sum = 0.0;
    for (i = 0; i < PopSize; i++)
    {
        sum += population[i].value;
    }
    average = sum / PopSize;

    //数据输出
    printf("gen=%d,avg=%f,best=%f,", generation, average, currentbest.value);
    printf("chromosome=");
    for (i = 0; i < CHROMLENGTH; i++)
    {
        printf("%c", currentbest.chrom[i]);
    }
    printf("\n");
}