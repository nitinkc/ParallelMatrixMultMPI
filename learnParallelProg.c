/*
 * learnParallelProg.c
 *
 *  Created on: Sep 10, 2014
 *      Author: nitin
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 500005

int main(){

	double a[SIZE];
	int i,x;
	//int num_threads = OMP_NUM_THREADS;

	/* Variables for Parallel execution time*/
			double begin=0, end=0;
			double time_spent=0;

	/* Variables for sequential execution of time*/
			clock_t begin_seq=0, end_seq=0;
			double time_spent_seq=0;


	/* Parallel Execution */
	begin = omp_get_wtime();
#pragma omp parallel private(x)
	{
		//x = omp_get_thread_num();
		//printf("%d", x);
	for(i=0;i<SIZE;i++){
		a[i] = a[i] + i;
		}
	}
	end = omp_get_wtime();
	time_spent = (double)(end - begin) ;
	printf("\nThe time spent in Parallel Execution : %1.5f\n", time_spent);


	/* Sequential Execution */
	printf("Serial Execution\n");
	begin_seq = clock();
	for(i=0;i<SIZE;i++){
		a[i] = a[i] + i;
	}
	end_seq = clock();
	time_spent_seq = (double)(end_seq - begin_seq) / CLOCKS_PER_SEC;
	printf("The time spent in sequential : %1.5f sec\n", time_spent_seq);
}
