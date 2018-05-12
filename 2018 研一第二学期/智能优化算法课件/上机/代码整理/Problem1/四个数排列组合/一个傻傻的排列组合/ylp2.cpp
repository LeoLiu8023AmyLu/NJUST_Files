#include <iostream>  
using namespace std;  
  
/************************************************************************/  
/*  
    num : ��Ҫ���е����� 
    count : �������� 
    numC�� �Ѿ����е����� 
    iUse���Ѿ����еĸ��� 
    iNull����0�ĸ���   
    sum: �������� 
*/  
/************************************************************************/  
template <class T>  
void ComBineNum(T *num, const int count, T *numC, int iUse, int* sum)  
{  
    int iNull = 0;  
    T *newNum = new T[count];  
    for (int i = 0; i < count; ++i){   
        memcpy(newNum, num, count);  
        if (newNum[i] == 0){  
            ++iNull;  
            if (iNull == count){  
                for (int i = 0; i < count; ++i){  
                    cout << numC[i];  
                }  
                cout << endl;  
                ++(*sum);  
                return;  
            }  
            continue;  
        }  
        numC[count - iUse] = newNum[i];  
        newNum[i] = 0;  
        ComBineNum(newNum, count, numC, iUse - 1, sum);  
    }  
    delete[] newNum;  
}  
  
  
int main()  
{  
    int sum = 0;  
    const int count = 4;  
    char num[count], pNum[count];  
    for (int i = 0; i < count; ++i)
	{  
        num[i] = i + '1';  
    }  
    ComBineNum<char>(num, count, pNum, count, &sum);  
    cout << "sum :" << sum << endl;  
  
    sum = 1;  
    for (int j = 1; j <= count; ++j)
	{  
        sum *= j;  
    }  
    cout << "sum :" << sum << endl;  
  
    return 0;  
}