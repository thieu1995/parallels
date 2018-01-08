Random  adjacency matrix:   matrix8.txt
        adjacency list:     matrix8_mpi.txt
for Prim's Algorithms

python random_matrix_prim.py matrix8 8

Run Tuan tu:
gcc -o prim_normal prim_normal.c
./prim_normal matrix8.txt

Run song song:
mpicc prim_mpi.c -o prim_mpi
mpicc -g -Wall -o prim_mpi prim_mpi.c

mpirun -n 16 prim_mpi matrix8_mpi.txt 0
