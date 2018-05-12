#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
int main(void)
{
 int ran_num;
 srand((unsigned)time(NULL));
  
 for (int i=0; i<4; i++)
 {
       switch((rand()%3))
       {
             case 1:
             ran_num = 65;
             ran_num += rand()%26;
             putchar(ran_num);
             break;
    
            case 2:
            ran_num = 97;
            ran_num += rand()%26;
            putchar(ran_num);
            break;
            default:
            ran_num = 48;
            ran_num += rand()%10;
            putchar(ran_num);
            break;
      }
 }
 cout<<endl;
 return 0;
}