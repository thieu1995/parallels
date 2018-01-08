//
// Created by thieunv on 08/01/2018.
//
// Basic ve Giao tiep: Point-to-Point (Bat buoc chi co 2 processor)

// 1. Synchronous send
// 2. Blocking send/blocking receive
//  Blocking:   MPI_Send(&buffer, count, type, dest, tag, comm)
//              MPI_Recv(&buffer, count, type, source, tag, comm, &status)

//  Non-Blocking :  MPI_Isend(&buffer, count, type, dest, tag, comm, &request)
//                  MPI_Irecv(&buffer, count, type, source, tag,comm, &request)

// MPI wait : Được sử dụng để cho non-blocking send và receive, nó sẽ dừng lại cho đến khi việc nhận và gửi thành công.
//      MPI_Wait (&request,&status)
//      MPI_Waitall(count,&array_of_requests,&array_of_statuses)


//  buffer: Là Application Buffer, là địa chỉ con trỏ được sử dụng để gửi hoặc nhận
//  count: Số lượng dữ liệu được truyền đi.
//  dest:   Là tham số trong phương thức send, chỉ ra processes sẽ được nhận dữ liệu(rank of process)
//  source: Tham số trong phương thức nhận, chỉ ra process truyền dữ liệu. Nếu tham số này là MPI_ANY_SOURCE thì sẽ nhận từ mọi process.
//  tag:    Là định danh của một bản tin. Hai phương thức truyền và nhận phải có tag trùng nhau.
//          Sử dụng MPI_ANY_TAG nếu không quan tâm tới định danh của bản tin
//  comm: Chỉ ra communicator mà các processes sẽ giao tiếp với nhau.
//  status: Sử dụng trong phương thức nhận, chỉ ra source và tag của bản tin.
//          Được định nghĩa trong cấu trúc MPI_Status ( stat.MPI_SOURCE, stat.MPI_TAG)
//  request:    Được sử dụng trong phương thức send và receive non-blocking.
//              Hệ thống sẽ gán cho mỗi phương thức một “số request” duy nhất để người lập trình sử dụng.




// 3. Non-Blocking send/ non-blocking receive

// 4. Buffered send
// Moi processor co 1 bo nho dem. Send tu processor nay vao bo nho dem processor kia

//Combined send/receive
// Ready send




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
