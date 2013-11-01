#include<stdio.h>
#include<stdlib.h>

int main() {
    int rows = 4;
    int cols = 3;
    int r, c;

    // heap1_arr contains all the data as 1d array.
    // heap1 points to the start of each row in the 1d data
    int *heap1_arr;
    int **heap1;

    // malloc a 1d array for the rows. In each array element,
    // malloc another 1d array for the cols
    int **heap2;

    heap1_arr = (int*)malloc(rows*cols*sizeof(int));
    heap1= (int**)malloc(rows*sizeof(int*));
    for (r=0; r<rows; r++) heap1[r]=&heap1_arr[r*cols];
    //for (r=0; r<rows; r++) heap1[r]=heap1_arr+(r*cols);
    
    heap2 = (int**)malloc(rows*sizeof(int*));
    for (r=0; r<rows; r++) heap2[r]=(int*)malloc(cols*sizeof(int));

    int simple[4][3];
    simple[0][0]=1;
    simple[0][1]=4;
    simple[0][2]=1;
    simple[1][0]=6;
    simple[1][1]=6;
    simple[1][2]=9;
    simple[2][0]=3;
    simple[2][1]=4;
    simple[2][2]=5;
    simple[3][0]=3;
    simple[3][1]=6;
    simple[3][2]=8;

    printf("simple:\n");
    for (r=0; r<rows; r++){
        for (c=0; c<cols; c++){
            heap1[r][c]=simple[r][c];
            heap2[r][c]=simple[r][c];
            printf("%d ",simple[r][c]);
        }
        printf("\n");
    }
   
    printf("heap1:\n");
    for (r=0; r<rows; r++){
        for (c=0; c<cols; c++){
            printf("%d ",heap1[r][c]);
        }
        printf("\n");
    }
    printf("heap2:\n");
    for (r=0; r<rows; r++){
        for (c=0; c<cols; c++){
            printf("%d ",heap2[r][c]);
        }
        printf("\n");
    }

    // free heap1
    free(heap1[0]); heap1[0]=NULL;
    free(heap1); heap1=NULL;

    // free heap2
    for (r=0; r<rows; r++){
        free(heap2[r]);
        heap2[r]=NULL;
    }
    free(heap2); heap2=NULL;


    return 0;
}
