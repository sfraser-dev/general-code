#include <stdio.h>

int main(){
    FILE *fp;
    double mydbl;

    printf("hi hi\n");
    fp = fopen("est_rate.txt","r");
    while (!feof (fp)){
        fscanf(fp,"%lf",&mydbl);
        printf("mydbl=%f\n",mydbl);
    }
    fclose(fp);

return 0;
}
