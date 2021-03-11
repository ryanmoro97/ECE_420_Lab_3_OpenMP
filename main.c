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
    double *X;
    int i, k, j, row, max, max_index, local_max, local_index;
    int *index;
    double start, end, temp;

    if(Lab3LoadInput(&A, &size) == 1){
        printf("Error loading input file");
        exit(1);
    }
    X = CreateVec(size);
    index = malloc(size * sizeof(int));
    for(i = 0; i < size; i++){
        index[i] = i;
    }


    GET_TIME(start);


    // eliminate elements below the diagonal to zero one column after another
    if (size == 1){
        X[0] = A[0][1] / A[0][0];
    }
    else {
        #pragma omp parallel num_threads(thread_count) default(none) shared(A, X, size, index, max, max_index, thread_count) private(temp, k, row, i, j, local_max, local_index)
        {
            for (k = 0; k < size - 1; k++) {

                /*Pivoting
                from row k to row n, find the row kp that has the maximum
                absolute value of the element in the kth column */

                // Parallelize inner loop instead of outer since outer has dependencies. You could maybe try the outer loop with a lot more private stuff

                max = 0;
                local_max = 0;
                local_index = 0;
                #pragma omp for //parallel for num_threads(thread_count)
                for (row = k; row < size; row++) {
                    if (fabs(A[index[row]][k]) > local_max) {
                        local_max = fabs(A[index[row]][k]);
                        local_index = row;
                    }
                }
                if (local_max > max) {
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
                        s
                                index[max_index] = index[k];
                        index[k] = row;
                    }
                }
                // Gaussian elimination
//            #pragma omp single
//            {
                #pragma omp for schedule(guided)
                for (i = k + 1; i < size; i++) {
                    temp = A[index[i]][k] / A[index[k]][k];
                    // could maybe put a parallel for loop here with a critical section and barrier
                    for (j = k; j < size + 1; j++) {
                        A[index[i]][j] = A[index[i]][j] - temp * A[index[k]][j];
                    }
                }
//            }

            }


            // Jordan elimination
            // Could try the outer loop and make a lot more private variables
            for (k = size - 1; k > 0; k--) {
//            #pragma omp parallel num_threads(thread_count)
//            {
                // scheduled here because for increasing values of k, some stuff might get overwritten if the order is wrong
                #pragma omp for schedule(guided)
                for (i = 0; i < k; i++) {
                    A[index[i]][size] = A[index[i]][size] - ((A[index[i]][k] / A[index[k]][k]) * A[index[k]][size]);
                    A[index[i]][k] = 0;
                }
//            }
            }


            // Final solution
//        #pragma omp parallel num_threads(thread_count)
//        {
            #pragma omp for //parallel for num_threads(thread_count)
            for (i = 0; i < size; i++) {
                X[i] = A[index[i]][size] / A[index[i]][i];
            }
//        }
        }
    }

    GET_TIME(end);
    Lab3SaveOutput(X, size, end-start);
    printf("%f\n", end-start);
    DestroyVec(X);
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
//
    return 0;
}
