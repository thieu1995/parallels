Programs are compiled with:

mpicc prim_mpi.c -o prim_mpi
mpicc kruskal_mpi.c -o kruskal_mpi -lm

Input file is a binary list of edges which are represented as three 4 byte integers.
First and second integers are vertex indices, while last integer is weight of edge connecting them.

Đây là chương trình từ bài báo. Gồm các file:
    prim_mpi_old.c 
    kruskal_mpi_old.c
    sample_input.c
    readme_old.md 
    
Đang code lại file: prim_mpi.c (dựa trên prim_mpi_old.c)    
    
