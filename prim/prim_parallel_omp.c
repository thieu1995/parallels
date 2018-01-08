#include <stdio.h>
#include <limits.h>
#include<omp.h>
#define V 5

int minKey(int distance[], bool visited[])
{
    int min = INT_MAX, min_index;
    int num_threads;
#pragma omp parallel num_threads(V)
    {
        num_threads = omp_get_num_threads();
#pragma omp for
        for (int v = 0; v < V; v++)
            if (visited[v] == false && distance[v] < min)
#pragma omp critical
                min = distance[v], min_index = v;
    }
    //printf("\n%d threads created in function minKey\n", num_threads);
    return min_index;
}

void printMST(int from[], int n, int graph[V][V])
{
    printf("Edge   Weight\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d    %d \n", from[i], i, graph[i][from[i]]);
}

void primMST(int graph[V][V])
{
    int from[V];
    int distance[V], num_threads;//distance[i]  = distance[i] and visited[] = visited[]
    bool visited[V];
    for (int i = 0; i < V; i++)
        distance[i] = INT_MAX, visited[i] = false;

    distance[0] = 0;
    from[0] = -1;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minKey(distance, visited);
        visited[u] = true;

        int v;

#pragma omp parallel num_threads(V)
        {
            num_threads = omp_get_num_threads();
#pragma omp for
            for (v = 0; v < V; v++)
                if (graph[u][v] && visited[v] == false && graph[u][v] < distance[v])
#pragma omp critical
                    from[v] = u, distance[v] = graph[u][v];
        }
    }
    printMST(from, V, graph);
    printf("\n%d threads are created in primMST\n", num_threads);
}
int main()
{
    /* Let us create the following graph
    2    3
    (0)--(1)--(2)
    |   / \   |
    6| 8/   \5 |7
    | /     \ |
    (3)-------(4)
    9          */
    double start = omp_get_wtime();
    int graph[V][V] = { { 0, 2, 0, 6, 0 },
                        { 2, 0, 3, 8, 5 },
                        { 0, 3, 0, 0, 7 },
                        { 6, 8, 0, 0, 9 },
                        { 0, 5, 7, 9, 0 },
    };
    //Print the solution
    primMST(graph);
    double end = omp_get_wtime();
    printf("Time = %.16g", end - start);
    getchar();

    return 0;
}