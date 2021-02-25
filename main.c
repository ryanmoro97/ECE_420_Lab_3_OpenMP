#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <Lab3IO.h>
#include <timer.h>


// Gaussian Jordan elimination
int operate(thread_count){
    double **A;
    double start, end;

    GET_TIME(start);

    //gaussian elimination
    #pragma omp parallel num_threads(thread_count){

    }


    //jordan elimination



    GET_TIME(end);
    Lab3SaveOutput(x, size, end-start);
}

int main(argc, argv[]){
    if(argc != 2){
        printf("Incorrect args");
    }
    int num_threads = atoi(argv[1]);
    operate(thread_count);
}