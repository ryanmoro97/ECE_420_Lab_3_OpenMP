#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "Lab3IO.h"
#include "timer.h"


// Gaussian Jordan elimination
int elimination(int thread_count){
    double **A;
    int size;
    double *x;
    int i, k, j, row, max, max_index, local_max, local_index;
    int *index;
    // local_max=0;
    // local_index=0;
    double start, end, temp;

    if(Lab3LoadInput(&A, &size) == 1){
        printf("Error loading input file");
        exit(1);
    }
    x = CreateVec(size);
    index = malloc(size * sizeof(int));
    for(i = 0; i < size; i++){
        index[i] = i;
    }


    GET_TIME(start);


    // eliminate elements below the diagonal to zero one column after another
    for(k = 0; k < size-1; k++){
        // printf("%d\n",k);
        /*Pivoting
        from row k to row n, find the row kp that has the maximum
        absolute value of the element in the kth column */
        #pragma omp parallel num_threads(thread_count) private(row, i, j, local_max, local_index)
            // shared(x, max, max_index, index, A) private(local_max, local_index, row, temp)
        {
            max = 0;
            local_max=0;
            #pragma omp parallel for num_threads(thread_count)
            for(row = k; row < size; row++){
                if(fabs(A[index[row]][k]) > local_max){
                    local_max = fabs(A[index[row]][k]);
                    local_index = row;
                }
            }
            if(local_max > max){
                #pragma omp critical
                max = local_max;
                max_index = local_index;
            }
            #pragma omp barrier
            // if k is not already the max, Swap row k and row kp
            // threads are consolidated -- need only single thread to preform indices swap
            #pragma omp single
            {
                if (max_index != k) {
                    row = index[max_index];
                    index[max_index] = index[k];
                    index[k] = row;
                }
            }
            // Gaussian elimination
            #pragma omp single
            {for(i = k+1; i < size; i++){
                temp = A[index[i]][k] / A[index[k]][k];
                // #pragma omp parallel for num_threads(thread_count)
                for(j = k; j < size+1; j++){
                    A[index[i]][j] = A[index[i]][j] - temp * A[index[k]][j];
                }
            }
          }

        }
    }

        // Jordan elimination

    for(k = size-1; k > 0; k--){
        #pragma omp parallel for num_threads(thread_count)
        for(i = 0; i < k; i++){
            A[index[i]][size] = A[index[i]][size] - ((A[index[i]][k] / A[index[k]][k]) * A[index[k]][size]);
            A[index[i]][k] = 0;
        }
    }


    // Final solution
    #pragma omp parallel for num_threads(thread_count)
    for(i = 0; i < size; i++){
        x[i] = A[index[i]][size] / A[index[i]][i];
    }

    GET_TIME(end);
    Lab3SaveOutput(x, size, end-start);
    DestroyVec(x);
    DestroyMat(A, size);
    free(index);
    return 0;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Incorrect args");
        exit(1);
    }
    int thread_count = atoi(argv[1]);
    elimination(thread_count);
}
