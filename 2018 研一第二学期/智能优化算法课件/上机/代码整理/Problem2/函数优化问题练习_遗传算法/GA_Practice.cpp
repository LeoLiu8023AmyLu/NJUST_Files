/******************************************************
* �����Ŵ��㷨 *
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************************************
* �����Ķ��� *
******************************************************/
#define POPSIZE 500    //��Ⱥ��С
#define MAXIMIZATION 1 //���ֵ����
#define MINIMIZATION 2 //��Сֵ����

/******************************************************
* �������*
******************************************************/
#define Cmax 1500000                  //���ֵ
#define Cmin 0                        //��Сֵ
#define LENGTH1 10                    //��һ������Ļ��򳤶�
#define LENGTH2 10                    //�ڶ�������Ļ��򳤶�
#define CHROMLENGTH LENGTH1 + LENGTH2 //������ܳ���

int Function_Mode = MAXIMIZATION; //���Ż�����
int PopSize = 80;                 //��Ⱥ��С
int MaxGeneration = 200;          //��ֹ����
double Pc = 0.6;                  //������
double Pm = 0.02;                 //������

/******************************************************
* ���ݽṹ�Ķ��� *
******************************************************/
struct individual //��Ⱥ���ݽṹ
{
    char chrom[CHROMLENGTH + 1]; //�������������ַ���
    double value;                //�����Ŀ�꺯��ֵ
    double fitness;              //�������Ӧ��ֵ
};

/******************************************************
* ȫ�ֱ����Ķ��� *
******************************************************/
int generation;                        //����
int best_index;                        //��Ѹ���ı��
int worst_index;                       //������ı��
struct individual bestindividual;      //��ǰ������Ѹ���
struct individual worstindividual;     //��ǰ����������
struct individual currentbest;         //Ŀǰ��ѵĸ���
struct individual population[POPSIZE]; //��Ⱥ

/******************************************************
* ����ԭ�͵����� *
******************************************************/
void GenerateInitialPopulation(void);    //������ʼ����Ⱥ
void GenerateNextPopulation(void);       //������һ����Ⱥ
void EvaluatePopulation(void);           //������Ⱥ
long DecodeChromosome(char *, int, int); //Ⱦɫ�����
void CalculateObjectValue(void);         //��������ֵ
void CalculateFitnessValue(void);        //������Ӧ��ֵ
void FindBestAndWorstIndividual(void);   //�ҳ���ú����ĸ���
void performEvolution(void);             //����
void SelectionOperator(void);            //ѡ��
void CrossoverOperator(void);            //����
void MutationOperator(void);             //����
void OutputTextReport(void);             //���

/******************************************************
* ������ *
******************************************************/
void main(void)
{
    generation = 0;
    GenerateInitialPopulation(); //������ʼ����Ⱥ
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
* ����: ������һ����Ⱥ *
******************************************************/
void GenerateInitialPopulation(void)
{
    int i, j;
    srand((unsigned)time(NULL));
    for (i = 0; i < PopSize; i++)
    {
        for (j = 0; j < CHROMLENGTH; j++)
        {
            population[i].chrom[j] = (rand() % 10 < 5) ? '0' : '1'; //rand()%10�������0-9������,С��5��ע0,�����ע1
        }
        population[i].chrom[CHROMLENGTH] = '\0';
    }
}

/******************************************************
* ����:������һ�� *
******************************************************/
void GenerateNextPopulation(void)
{
    SelectionOperator();
    CrossoverOperator();
    MutationOperator();
}

/***********************************************************************
* ���ܣ�������Ⱥ *
***********************************************************************/
void EvaluatePopulation(void)
{
    CalculateObjectValue();       //����Ŀ��ֵ
    CalculateFitnessValue();      //������Ӧ��ֵ
    FindBestAndWorstIndividual(); //�ҳ���ú�����Ⱦɫ�����
}

/******************************************************
* ����:�Ѷ�����Ⱦɫ��ֵת����ʮ���� *
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
* ����:����Ŀ��ֵ *
* f(x1,x2)=50*(x1*x1-x1*x2)*(x1*x1-x1*x2)+20*(x2*x2-x1*x2)*(x2*x2-x1*x2) *
* �������ֵΪ�� *
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
* ���ܣ�������Ӧ�ȵ�ֵ *
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
* ���ܣ��ڵ�ǰ���ҳ���õĸ��� * 
******************************************************/
void FindBestAndWorstIndividual(void)
{
    int i;
    double sum = 0.0; //�ҳ���һ������ú�����ֵ
    bestindividual = population[0];
    worstindividual = population[0];
    for (i = 1; i < PopSize; i++)
    {
        if (population[i].fitness > bestindividual.fitness)
        { //���αȽϣ��ҳ���Ѹ���
            bestindividual = population[i];
            best_index = i;
        }
        else if (population[i].fitness < worstindividual.fitness)
        { //���αȽϣ��ҳ�������
            worstindividual = population[i];
            worst_index = i;
        }
        sum += population[i].fitness;
    } //�ҳ���õĸ���
    if (generation == 0)
    { //����һ����õ���ʱ�����currentbest��
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
* ����:��ʾ���۽��������ǰ����õĸ���������ĸ��� *
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
* ����:����ѡ���㷨 *
***************************************************/
void SelectionOperator(void)
{
    int i, index;
    double p, sum = 0.0;
    double cfitness[POPSIZE]; //������Ӧ�ʵ�ֵ
    struct individual newpopulation[POPSIZE];
    //���������Ӧ��
    for (i = 0; i > PopSize; i++)
    {
        sum += population[i].fitness;
    }
    for (i = 0; i > PopSize; i++)
    {
        cfitness[i] = population[i].fitness / sum;
    }
    //�����ۼ���Ӧ��
    for (i = 1; i < PopSize; i++)
    {
        cfitness[i] = cfitness[i - 1] + cfitness[i];
    }
    //ѡ��
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
* ����:���㽻���㷨 *
***************************************************/
void CrossoverOperator(void)
{
    int i, j;
    int index[POPSIZE];
    int point, temp;
    double p;
    char ch; //��Ⱥ�������������
    for (i = 0; i < PopSize; i++)
    {
        index[i] = i;
    }
    for (i = 0; i < PopSize; i++)
    {
        point = rand() % (PopSize - i); //������Ⱥ˳��
        temp = index[i];
        index[i] = index[point + i];
        index[point + i] = temp;
    }
    //���㽻���㷨
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
* ����: Ⱦɫ�������� *
***************************************************/
void MutationOperator(void)
{
    int i, j;
    double p; //������
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
* ����: �����ǰ���Ľ�� *
***********************************************************/
void OutputTextReport(void)
{
    int i;
    double sum;     //��ʱ������
    double average; //Ŀ���ƽ����Ⱥ��

    //����ƽ��Ŀ��ֵ
    sum = 0.0;
    for (i = 0; i < PopSize; i++)
    {
        sum += population[i].value;
    }
    average = sum / PopSize;

    //�������
    printf("gen=%d,avg=%f,best=%f,", generation, average, currentbest.value);
    printf("chromosome=");
    for (i = 0; i < CHROMLENGTH; i++)
    {
        printf("%c", currentbest.chrom[i]);
    }
    printf("\n");
}