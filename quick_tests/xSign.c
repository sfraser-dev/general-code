#include<stdio.h>

int main() {
    int a=211;
    int b=210;
    int x=a-b;
    int res;

    res=((x >> 31) | ((int)( (((unsigned int) -x)) >> 31)));

    printf("res=%d\n",res);

 return 0;
}
