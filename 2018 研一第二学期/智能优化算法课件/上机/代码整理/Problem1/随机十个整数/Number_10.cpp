#include<stdlib.h>
#include<time.h>
#include <stdio.h>
int main(){
    int a[10],i,j;
    srand((unsigned)time(0));
    for(i=0; i<10; i++){
        a[i] = rand()%10+1;
        for(j=0;j<i;j++){
            if(a[i]==a[j]){
                a[i]=rand()%10+1;
                j=0;
            }
        }
        printf("%d\n",a[i]);
    }
    return 0 ;
}