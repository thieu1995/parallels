# Parallel-Dijkstras-Algorithm

MPI program that parallelizes Dijkstra’s. Input for the program consists of the number of vertices and adjacency matrix and the output of the program will be a list of lengths of the shortest paths from vertex 0 to all other vertices. Project from C and Parallel Computing class; Language: C.


Run in terminal:
    python randomadjacencymatrix.py matrix1024.txt 1024
    mpicc -g -Wall -o dijkstra_parallel dijkstra_parallel.c
    mpirun -n 16 --oversubscribe dijkstra_parallel < ./matrix1024.txt
    
    gcc -o dijkstra_normal dijkstra_normal.c
    ./dijkstra_normal matrix1024.txt

