mpicc matrixMultMPI.c -o matrixMultMPI.o 
echo "Speed up curve 5000,2000,5000"
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 5000 2000 5000
mpirun -hostfile machine -np 8 ./matrixMultMPI.o 5000 2000 5000
mpirun -hostfile machine -np 12 ./matrixMultMPI.o 5000 2000 5000
mpirun -hostfile machine -np 16 ./matrixMultMPI.o 5000 2000 5000
mpirun -hostfile machine -np 20 ./matrixMultMPI.o 5000 2000 5000
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 5000 2000 5000

echo "Speed up curve 100 60 100"
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 8 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 12 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 16 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 20 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 100 60 100

echo "Speed up curve 500 200 500"
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 8 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 12 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 16 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 20 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 500 200 500

echo "Speed up curve 1000 500 1000"
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 8 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 12 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 16 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 20 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 1000 500 1000

echo
echo "Granularity effect with 24"
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 24 ./matrixMultMPI.o 5000 2000 5000

echo
echo "Granularity effect with 4"
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 100 60 100
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 500 200 500
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 1000 500 1000
mpirun -hostfile machine -np 4 ./matrixMultMPI.o 5000 2000 5000
