/**
 * Parallel implementation of Prim's MST algorithm
 *
 * Compile with: mpicc prim_mpi.c -o prim_mpi
 * Execute with: mpiexec -n 2 prim_mpi input_graph 0
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

#define INFINITY INT_MAX

void start_communication_measure();
void end_communication_measure();
void reduce_global_min(int *, int *, int *, MPI_Datatype *);

int number_of_processors; /* Number of processors */
int rank;                 /* MPI rank */
int root = 0;             /* Rank of root process (which holds the final result) */
MPI_Datatype edge;        /* Datatype containing tuple (u,v,d[u]) */
MPI_Op reduce_op;         /* Reduce operation for findimg min edge */
int number_of_vertices;   /* Number of vertices in the graph */
int vertices_per_process; /* Number of vertices that each process will get */
int starting_vertex;      /* Initial vertice in MST */
int total_distance;       /* Total distance of MST (relevant only in root process) */

FILE * f;                 /* Input file containing graph. */

int ** weight;            /* Part of weighted adjacency matrix each process holds */
int * d;                  /* Array holding distances between MST and other vertices */
int * who;              /* who[i] holds the index of the vertex i would have to be
                             linked to in order to get a distance of d[i] */
unsigned char * in_tree;  /* 1 if the vertex is already in the MST; 0 otherwise*/

typedef struct edge_t {   /* struct representing one edge */
    int v;
    int u;
    int weight;
} edge_s;

edge_s * mst_edges;       /* Array of MST edges */
int edges_added_to_mst;   /* Number of edges currently added to MST */

double comp_start_time;   /* Start time of computation */
double comp_end_time;     /* End time of computation */

double comm_start_time;   /* Start time of communication */
double comm_end_time;     /* End time of communication */
double total_comm_time;   /* Total time of communication */

double parse_start_time;  /* Start time of parsing */
double parse_end_time;    /* End time of parsing */

void init_MPI(int argc, char** argv) {

	/* Parsing graph size */
	f = fopen(argv[1], "rb");
	fread(&number_of_vertices, sizeof(number_of_vertices), 1, f);

	starting_vertex = atoi(argv[2]);

	/* Initialize MPI */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processors);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Create 'edge' type (u, v, distance) */
	MPI_Type_contiguous(3, MPI_INT, &edge);
	MPI_Type_commit(&edge);

	/* Create reduce operation */
	MPI_Op_create((MPI_User_function *)reduce_global_min, 1, &reduce_op);

	total_comm_time = 0;

	if (rank == root) {
		//printf("Number of vertices: %d\n", number_of_vertices);
		//printf("Starting vertice: %d\n", starting_vertex);
	}

}

void check_conditions() {

	/* Check for error conditions */
	unsigned char isError = 0;
	if (number_of_vertices < number_of_processors) {
		printf("Number of vertices (%d) is smaller than number of processors (%d)\n", number_of_vertices, number_of_processors);
		isError = 1;
	}
	if (number_of_vertices % number_of_processors != 0) {
		puts("Number of vertices % number of processors must be 0\n");
		isError = 1;
	}
	if (starting_vertex > number_of_vertices) {
		printf("Invalid start vertice. Must be < %d\n", number_of_vertices);
		isError = 1;
	}

	if (isError == 1) {
		fclose(f);
		MPI_Type_free(&edge);
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}

}

void init_data_structures() {

	/* Create data structures */
	vertices_per_process = number_of_vertices / number_of_processors;
	if (rank == root) {
		//printf("Vertices per process: %d\n", vertices_per_process);
	}

	int i,j;

	/* Dynamic init of int weight[number_of_vertices][vertices_per_process] */
	weight = (int**)malloc(number_of_vertices * sizeof(int*));
	for (i = 0; i < number_of_vertices; i++) {
		weight[i] = (int*)malloc(vertices_per_process * sizeof(int));
		for (j = 0; j < vertices_per_process; j++) {
			weight[i][j] = 0;
		}
	}

	/* Dynamic init of d[vertices_per_process] */
	d = (int *) malloc(vertices_per_process * sizeof(int));
	who = (int *) malloc(vertices_per_process * sizeof(int));
	in_tree = (unsigned char *) malloc(vertices_per_process * sizeof(unsigned char));

	/* Initialize d with infinity and inTree with 0 */
	for (i = 0; i < vertices_per_process; i++) {
		d[i] = INFINITY;
		in_tree[i] = 0;
	}

	if (rank == root) {
		mst_edges = (edge_s *) malloc(number_of_vertices * sizeof(edge_s));
		edges_added_to_mst = 0;
	}

}

void parse_input() {

	/* Start and end index of vertex belonging to this process */
	int rank_range_start = rank * vertices_per_process;
	int rank_range_end = (rank + 1) * vertices_per_process;

	int total_edges, u, v, edge_weight;
	fread(&total_edges, sizeof(int), 1, f);

	int i;
	int f_edge[3];
	for (i=0; i < total_edges; i++) {
		fread(&f_edge, sizeof(int), 3, f);
		
		v = f_edge[0]; u = f_edge[1]; edge_weight = f_edge[2];

		if (v < 0 || u < 0) {
			printf("Invalid edge definition (%d, %d) = %d, index %d of %d edges\n", v, u, edge_weight, i, total_edges);
			return;
		}

		if ((v >= rank_range_start) && (v < rank_range_end)) {
			weight[u][get_local_vertex_index(v)] = edge_weight;
		}
		if ((u >= rank_range_start) && (u < rank_range_end)) {
			weight[v][get_local_vertex_index(u)] = edge_weight;
		}
	}
	
	fclose(f);

	MPI_Barrier(MPI_COMM_WORLD);
}

int get_global_vertex_index(int localIndex) {
	/* Converts global vertex index to local depending on process */
	if (localIndex == -1 || rank == 0) {
		return localIndex;
	} else {
		return localIndex + (rank * vertices_per_process);
	}
}

int get_local_vertex_index(int globalIndex) {
	/* Converts local vertex index to global depending on process */
	int localIndex = -1;
	if ((globalIndex >= rank * vertices_per_process) && (globalIndex < (rank + 1) * vertices_per_process)) {
		localIndex = globalIndex - (rank * vertices_per_process);
	}
	return localIndex;
}

void update_distances(int addedVertex) {

	/* Updates d[] to contain minimum distances after new vertex has been added to MST */
	int i;
	for (i = 0; i < vertices_per_process; i++) {
		if ((weight[addedVertex][i] != 0) && (d[i] > weight[addedVertex][i])) {
			d[i] = weight[addedVertex][i];
			who[i] = addedVertex;
		}
	}

}

void reduce_global_min(int *invec, int *inoutvec, int *len, MPI_Datatype *type) {

	/* Find global min from MPI_Reduce buffer */
	if (invec[2] < inoutvec[2]) {
		inoutvec[0] = invec[0];
		inoutvec[1] = invec[1];
		inoutvec[2] = invec[2];
	}
}

void find_mst() {

	/* Add starting vertex and update d[] for it */
	int localIndex = get_local_vertex_index(starting_vertex);
	if (localIndex != -1) {
		d[localIndex] = 0;
		in_tree[localIndex] = 1;
	}
	update_distances(starting_vertex);

	/* Buffers for MPI */
	int sendbuf[3];
	int recvbuf[3];

	/* Find MST */
	int vertices_added = 1;
	while (vertices_added < number_of_vertices) {

		/* Find the vertex with the smallest distance to the tree */
		int min = -1;
		int i;
		for (i = 0; i < vertices_per_process; i++) {
			if (in_tree[i] != 1) {
				if ((min == -1) || (d[min] > d[i])) {
					min = i;
				}
			}
		}

		sendbuf[0] = get_global_vertex_index(min); // v
		sendbuf[1] = who[min]; // u
		sendbuf[2] = d[min]; //d[v]

		/* Gather all results and find vertex with global minimum */
		start_communication_measure();
		MPI_Reduce(sendbuf, recvbuf, 3, MPI_INT, reduce_op, 0, MPI_COMM_WORLD);
		end_communication_measure();

		int u, v, globalMin;
		if (rank == root) {
			u = recvbuf[0];
			v = recvbuf[1];
			globalMin = recvbuf[2];

			edge_s e = { v, u, globalMin };
			mst_edges[edges_added_to_mst++] = e;
			total_distance += globalMin;
		}

		/* Broadcast vertex with global minimum */
		start_communication_measure();
		MPI_Bcast(&globalMin, 1, MPI_INT, root, MPI_COMM_WORLD);
		end_communication_measure();

		/* Mark vertex as in tree for appropriate process */
		if (get_local_vertex_index(globalMin) != -1) {
			in_tree[get_local_vertex_index(globalMin)] = 1;
		}

		update_distances(globalMin);

		vertices_added++;
	}
}

void printTotalDistance() {
	/* Only root's totalDistance is correct */
	if (rank == root) {
		printf("Total distance: %d\n", total_distance);
	}
}

void printMSTEdges() {
	/* Only root's has mst_edges array */
	if (rank == root) {
		int i;
		for(i = 0; i < edges_added_to_mst; i++) {
			edge_s * e = &mst_edges[i];
			int v = (*e).v;
			int u = (*e).u;
			int w = (*e).weight;

			printf("(%d,%d)=%d\n", v, u, w);
		}
	}
}

void free_resources() {

	/* Free memory used by dynamic data structures */
	int i;
	for (i = 0; i < number_of_vertices; i++) {
		free(weight[i]);
	}

	free(weight);
	free(d);
	free(who);
	free(in_tree);

	/* Shut down MPI */
	MPI_Type_free(&edge);
	MPI_Finalize();

}

void printWeight() {
	/* Helper function to print contents of weight[][] (used for debug) */
	int i,j;
	for (i = 0; i < number_of_vertices; i++) {
		for (j = 0; j < vertices_per_process; j++) {
			printf("Proc %d: weight[%d][%d]= %d\n", rank, i, j, weight[i][j]);
		}
	}
}

void printDistances() {
	/* Helper function to print contents of d[] (used for debug) */
	int i;
	for (i = 0; i < vertices_per_process; i++) {
		printf("Proc %d: d[%d]= %d\n", rank, i, d[i]);
	}
}

void start_computation_measure() {
	comp_start_time = MPI_Wtime();
}

void end_computation_measure() {
	comp_end_time = MPI_Wtime();
}

void start_communication_measure() {
	comm_start_time = MPI_Wtime();
}

void end_communication_measure() {
	comm_end_time = MPI_Wtime();
	total_comm_time += comm_end_time - comm_start_time;
}

void start_parse_measure() {
	parse_start_time = MPI_Wtime();
}

void end_parse_measure() {
	parse_end_time = MPI_Wtime();
}

void print_measurement_times() {
	if (rank == root) {
		printf("Total: %f, Comm: %f, Parse: %f\n",
		comp_end_time - comp_start_time,
		total_comm_time,
		parse_end_time - parse_start_time);
	}
}

int main(int argc, char** argv) {

	if (argc != 3) {
		printf("usage: %s filename starting_vertex\n", argv[0]);
		return 1;
	}

	init_MPI(argc, argv);

	check_conditions();

	init_data_structures();

	start_parse_measure();
	parse_input();
	end_parse_measure();

	start_computation_measure();
	find_mst();
	end_computation_measure();

	print_measurement_times();

	//printMSTEdges();

	free_resources();

	return 0;
}

