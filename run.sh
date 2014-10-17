echo "Compiling......."
 mpicc matrixMultMPI.c -o matrixMultMPI.o
echo "running......"
 mpirun -hostfile machine -np 24 matrixMultMPI.o 480 480 480