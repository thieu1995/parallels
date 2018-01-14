#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int V, int dist[], bool sptSet[]) {
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed distance array
int printSolution(int V, int dist[]) {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int V, int graph[V][V], int src) {
    int dist[V];     // The output array.  dist[i] will hold the shortest
    // distance from src to i

    bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in first iteration.
        int u = minDistance(V, dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    // print the constructed distance array
    printSolution(V, dist);
}

// driver program to test above function
int main(int argc, char **argv) {

    if (argc <= 1) {
        printf("You need to enter the filename.\n");
        exit(1);
    }

    FILE * filePtr;
    int n;          // Number of vertices in the graph
    filePtr = fopen(argv[1], "r");      // argv[0] : name of this program
    if(filePtr == NULL) {
        printf("Filename doesn't existed.\n");
        exit(1);
    }

    fscanf(filePtr, "%d", &n);

    const int V = n;

    int (*graph)[V] = (int (*)[V])malloc(V * sizeof (int [V]));

    int i, j;
    for(i = 0; i < V; i++) {
        for(j = 0; j < V; j++) {
            fscanf(filePtr, "%d", &graph[i][j]);
        }
    }
    fclose(filePtr);

    //calculate running time
    clock_t begin = clock(); // start running
    dijkstra(V, graph, 0);
    clock_t end = clock(); //end running
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Running time: %f\n", time_spent);

    return 0;
}
