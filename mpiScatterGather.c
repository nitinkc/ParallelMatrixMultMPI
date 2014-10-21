// jug.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{	
	int NumberOfProcessor , RankNumber ;
	
	int no=atoi(argv[1]);

	int FirstRowSize=no;
	int FirstColSize=no;

	int SecondRowSize=no;
	int SecondColSize=no;

	MPI_Init ( &argc , & argv );
	MPI_Comm_size ( MPI_COMM_WORLD , & NumberOfProcessor );
	MPI_Comm_rank ( MPI_COMM_WORLD , & RankNumber );
	
	//recbuf is the buffer that any processor will receive after scattering
	long *ReceiverBuf=new long[((SecondRowSize*SecondColSize)/NumberOfProcessor)+SecondRowSize];
	
	
	long StartComputationTime,EndComputationTime;
	long StartCommunityTime1,EndCommunityTime1;
	long StartCommunityTime2,EndCommunityTime2;

	long **FirstMatrix,**SecondMatrix;

	//matrix declaration
	FirstMatrix=(long **)malloc(sizeof(long*)*(FirstRowSize));
	for(int i=0;i<FirstRowSize;i++)
	{
		FirstMatrix[i]=(long *)malloc(sizeof(long*)*(FirstColSize));
	}
	
	SecondMatrix=(long **)malloc(sizeof(long*)*(SecondRowSize));
	for(int i=0;i<SecondRowSize;i++)
	{
		SecondMatrix[i]=(long *)malloc(sizeof(long*)*(SecondColSize));
	}

	// convert the B matrix to one dimension matrix 

	long *SecondRootBuf=new long[SecondRowSize*SecondColSize];
	
	//number of each processes assign to each processor to be calculated 
	int* ProcessNoForEachCPU=new int[NumberOfProcessor];
	//starting address for partitioning the second matrix in scatter and gather 
	int* StartProcessOfEachCPU=new int[NumberOfProcessor];
	//mumber of columns assigned to each processor
	int PNo=SecondColSize/NumberOfProcessor;
	//remaining columns which are not assigned to any processor
	int PRemaining=SecondColSize%NumberOfProcessor;
	
	//initializing the first matrix

		for(int i=0;i<FirstRowSize;i++)
		{
			for(int j=0;j<FirstColSize;j++)
			{
				FirstMatrix[i][j]=rand()%100;
				//FirstMatrix[i][j]=1;
			}
		}
	
		
	if(RankNumber==0)
	{
	//initializing the second matrix
		for(int i=0;i<SecondRowSize;i++)
		{
			for(int j=0;j<SecondColSize;j++)
			{
				SecondMatrix[i][j]=rand()%100;
				//SecondMatrix[i][j]=1;
			}
		}


		int k=0;
		int j=0;
		
		//converting two dimension matrix into one dimension matrix to use scatter
		for(int i=0;i<SecondRowSize*SecondColSize;i++)
		{
			if(k>=SecondRowSize)
			{
				k=0;
				j=j+1;
			}
			SecondRootBuf[i]=SecondMatrix[k][j];
			k++;
		}
	
	}
		int Pointer=0;
		//partitioning
		for(int i=0;i<NumberOfProcessor;i++)
		{   
			ProcessNoForEachCPU[i]=PNo;

			StartProcessOfEachCPU[i]=Pointer;
			Pointer+=ProcessNoForEachCPU[i]*SecondRowSize;

			if(PRemaining>0) 
			{	
				ProcessNoForEachCPU[i]++; 
				Pointer+=SecondRowSize;  
				PRemaining--; 
			} 	 
		}

		for (int i=0;i<NumberOfProcessor;i++)
		{
			ProcessNoForEachCPU[i]*=SecondRowSize;
		}


	StartCommunityTime1 = MPI_Wtime();
	//MPI_Scatter ( FirstMatrix,FirstColSize*FirstRowSize,MPI_long ,FirstReceiverBuf, FirstColSize*FirstRowSize , MPI_long ,0 , MPI_COMM_WORLD );
	MPI_Scatterv ( SecondRootBuf, ProcessNoForEachCPU,StartProcessOfEachCPU , MPI_LONG ,ReceiverBuf, (PNo+1)*SecondRowSize , MPI_LONG ,0 , MPI_COMM_WORLD );
	EndCommunityTime1 = MPI_Wtime();
	
	/*for(int i=0; i<FirstColSize;i++)
	{
		for(int j=0;j<FirstRowSize;j++)
		{
			printf("%d",FirstMatrix[i][j]);
		}
	}*/
	
	
	int NumberOfColForEachCPU=ProcessNoForEachCPU[RankNumber] /SecondRowSize;
	long **RecSecondMatrix;

	//converting one dimension matrix into two dimension matrix
	RecSecondMatrix=(long **)malloc(sizeof(long*)*(SecondRowSize));
	for(int i=0;i<FirstRowSize;i++)
	{
		RecSecondMatrix[i]=(long *)malloc(sizeof(long*)*(NumberOfColForEachCPU));
	}
	// Multiple matrix which fills in each matrix according to its data
	long **MultipleEachCPU;
	MultipleEachCPU=(long **)malloc(sizeof(long*)*(FirstRowSize));
		for(int i=0;i<FirstRowSize;i++)
	{
		MultipleEachCPU[i]=(long *)malloc(sizeof(long*)*(NumberOfColForEachCPU));
	}
	int k=0;
	int j=0;

	//converting the one dimension matrix taken from scater into two dimension matrix for easier calculation

	for(int i=0;i<ProcessNoForEachCPU[RankNumber];i++)
	{
		if(k>=SecondRowSize)
		{
			k=0;
			j=j+1;
		}
		RecSecondMatrix[k][j]=ReceiverBuf[i];
		k++;
	}
	
	StartComputationTime=MPI_Wtime();
	//calculation
	for (int i=0;i<FirstRowSize;i++)
	{
		for(int j=0;j<NumberOfColForEachCPU;j++)
		{
			MultipleEachCPU[i][j]=0;
			for(k=0;k<FirstColSize;k++)
			{
				MultipleEachCPU[i][j]+=	FirstMatrix[i][k]*RecSecondMatrix[k][j];
			}
		}
	}
	EndComputationTime=MPI_Wtime();

	//converting two dimension matrix into one dimension for gathering
	long *ThirdRootBuf=new long[(FirstRowSize*NumberOfColForEachCPU)];
	
	k=0;
	j=0;
	for(int i=0;i<FirstRowSize*NumberOfColForEachCPU;i++)
	{
		if(k>=FirstRowSize)
		{
			k=0;
			j=j+1;
		}
		ThirdRootBuf[i]=MultipleEachCPU[k][j];
		k++;
	}


	//one dimension matrix of the product
	long *FinalBuf=new long[(FirstRowSize*SecondColSize)];
	
	StartCommunityTime2 = MPI_Wtime();
	MPI_Gatherv(ThirdRootBuf,FirstRowSize*NumberOfColForEachCPU, MPI_LONG ,FinalBuf,ProcessNoForEachCPU,StartProcessOfEachCPU,MPI_LONG ,0 , MPI_COMM_WORLD );
	EndCommunityTime2 = MPI_Wtime();
	
	
	
	//two dimension product matrix
	long **FinalmultipleMatrix;
	FinalmultipleMatrix=(long **)malloc(sizeof(long*)*(FirstRowSize));
	for(int i=0;i<FirstRowSize;i++)
	{
		FinalmultipleMatrix[i]=(long *)malloc(sizeof(long*)*(SecondColSize));
	}
	k=0;
	j=0;
	//converting one dimension matrix into two dimension
	if(RankNumber==0){
	for(int i=0;i<FirstRowSize*SecondColSize;i++)
	{
		if(k>=SecondRowSize)
		{
			k=0;
			j=j+1;
		}
		FinalmultipleMatrix[k][j]=FinalBuf[i];
		k++;
	}
	}
	EndCommunityTime2 = MPI_Wtime();
	if(RankNumber==0)
	{
	
		/*for(int i=0; i<FirstColSize;i++)
		{
			for(int j=0;j<SecondRowSize;j++)
			{
				printf("%d",FinalmultipleMatrix[i][j]);
			}
			printf("\n");
		}*/

	
		printf("\n CommunityTime: %f milisec",(EndCommunityTime1-StartCommunityTime1)+(EndCommunityTime2-StartCommunityTime2));
		printf("\n ComputaionTime: %f milisec",EndComputationTime-StartComputationTime);

	
	}
	MPI_Finalize();
	   return 0;
}