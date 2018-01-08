//
// Created by thieunv on 08/01/2018.
//
// Basic ve khoi tao MPI

#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
    int num_proc, rank, rc;
    rc = MPI_Init(&argc,&argv);     // Khoi tao moi truong MPI

    if (rc != MPI_SUCCESS) {
        printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);  // Huy bo cac processor duoc gan vs Communicator
    }
    MPI_Comm_size(MPI_COMM_WORLD,&num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf ("Number of Processor= %d My rank= %d\n", num_proc,rank);
    /******* do some work *******/
    MPI_Finalize();         // Ket thuc moi truong MPI
}

// mpicc -o basic1 basic1.c
// mpirun -n 8 basic1
