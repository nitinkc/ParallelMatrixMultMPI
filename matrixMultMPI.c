/******************************************************************************
				Assignment 3
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
               
#define MASTER 0              
#define TAG_MASTER 1      
#define TAG_WORKER 2         
#define MAXRAND 99999

int main (int argc, char *argv[])
{
int numtasks, taskid, numworkers,source,dest,rows,averow, extra, index;
int i, j, k, rc;
double startTime, endTime, startTimeLinear, endTimeLinear;           
double commTimeStart1, commTimeEnd1, commTimeStart2, commTimeEnd2;
int ROW_A = atoi(argv[1]),
    COL_A = atoi(argv[2]),
    COL_B = atoi(argv[3]);

double	a[ROW_A][COL_A], /* Declare the MAtrices*/
	b[COL_A][COL_B],
	c[ROW_A][COL_B]; 

if(argc<=3) { /* Testing the correct arguments passed */
        printf("Feed 3 arguments,ROW_A, COL_A, COL_B.. I will die now :( ..");
        exit(1);
     } 

MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
  }
numworkers = numtasks-1;

// master task
   if (taskid == MASTER)//initialize the arrays
   {
     // printf("multiplication has started with %d tasks.\n",numtasks);
     // printf("Initializing arrays...\n");
      for (i=0; i<ROW_A; i++)
         for (j=0; j<COL_A; j++)
            a[i][j]= ((double)rand()/(double)MAXRAND);
      for (i=0; i<COL_A; i++)
         for (j=0; j<COL_B; j++)
            b[i][j]= ((double)rand()/(double)MAXRAND);;

	/* Linear Execution of Matrix Multiplication by Master*/
	startTimeLinear = MPI_Wtime();
     for (k=0; k<ROW_A; k++){
         for (i=0; i<COL_B; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<COL_A; j++)
               c[i][k] = c[i][k] + a[i][j] * b[j][k];
         }
	}
 	endTimeLinear = MPI_Wtime();
        printf("\n ComputaionTime for Linear: %f milisec", endTimeLinear - startTimeLinear);
      
	/* Send matrix data to the worker tasks */
      averow = ROW_A/numworkers;
      extra = ROW_A%numworkers;

      index = 0;
      for (dest=1; dest<=numworkers; dest++)
      {
	if (dest <= extra)
		rows = averow + 1;
	else
                rows = averow;

		startTime = MPI_Wtime();  	
       //  printf("Sending %d rows to task %d index=%d\n",rows,dest,index);
         /*MPI_Send(void* data, int count, MPI_Datatype datatype, int destination,int tag, MPI_Comm communicator)*/
        commTimeStart1 = MPI_Wtime(); 
	MPI_Send(&index, 1, MPI_INT, dest, TAG_MASTER, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, TAG_MASTER, MPI_COMM_WORLD);
         MPI_Send(&a[index][0], rows*COL_A, MPI_DOUBLE, dest, TAG_MASTER,
                   MPI_COMM_WORLD);
         MPI_Send(&b, COL_A*COL_B, MPI_DOUBLE, dest, TAG_MASTER, MPI_COMM_WORLD);
         index = index + rows;
	commTimeEnd1 = MPI_Wtime();
      }

      /* Receive results from worker tasks */

      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&index, 1, MPI_INT, source, TAG_WORKER, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, TAG_WORKER, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[index][0], rows*COL_B, MPI_DOUBLE, source, TAG_WORKER, 
                  MPI_COMM_WORLD, &status);
	
         //printf("Received results from task %d\n",source);
      }
	  /*
      // Print results
      
	printf("******************************************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<ROW_A; i++)
      {
         printf("\n"); 
         for (j=0; j<COL_B; j++) 
            printf("%f   ", c[i][j]);
      }
      printf("\n******************************************************\n");
      printf ("Done.\n");
*/

	endTime = MPI_Wtime();
	double commDelay = (commTimeEnd2-commTimeStart2)+(commTimeEnd1-commTimeStart1);
	double computation = endTime - startTime;
	printf("\n ComputaionTime Parallel: %f sec", endTime - startTime);
	printf ("\n Communication TIme: %f sec",commDelay);
	printf("\nSpeed Up : %f sec", (endTimeLinear-startTimeLinear)/(endTime-startTime));
	printf("\nGranularity : %f",computation/commDelay);   
}
/* worker task ***********/
   if (taskid > MASTER)
   {
      MPI_Recv(&index, 1, MPI_INT, MASTER, TAG_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, TAG_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*COL_A, MPI_DOUBLE, MASTER, TAG_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, COL_A*COL_B, MPI_DOUBLE, MASTER, TAG_MASTER, MPI_COMM_WORLD, &status);

      for (k=0; k<COL_B; k++)
         for (i=0; i<rows; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<COL_A; j++)
               c[i][k] = c[i][k] + a[i][j] * b[j][k];
         }
	commTimeStart2 = MPI_Wtime();
      MPI_Send(&index, 1, MPI_INT, MASTER, TAG_WORKER, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, TAG_WORKER, MPI_COMM_WORLD);
      MPI_Send(&c, rows*COL_B, MPI_DOUBLE, MASTER, TAG_WORKER, MPI_COMM_WORLD);
	commTimeEnd2 = MPI_Wtime();
   }
   MPI_Finalize();


}
