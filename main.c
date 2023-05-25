#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define SIZE 6

struct myGraph {
    int numNodes;
    int **edges;
};

typedef struct myGraph Graph;

void destroyGraph(Graph *graph) {
    // verify if the memory is allocated
    if (graph->edges == NULL) {
        free(graph);
        return;
    }

    // deallocate memory
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->edges != NULL) {
            free(graph->edges[i]);
        }
    }
    free(graph->edges);
    free(graph);
}

Graph *createGraph(int numNodes) {
    Graph *graph = malloc(sizeof(Graph) * numNodes);

     // check allocation
    if (graph == NULL)
        return NULL;

    graph->numNodes = numNodes;

    // allocate rows of the matrix
    graph->edges = calloc(sizeof(int*), graph->numNodes);

    // check allocation
    if (graph->edges == NULL) {
        free(graph);
        return NULL;
    }

    // allocate cols of the matrix
    for (int i = 0; i < graph->numNodes; i++) {
        graph->edges[i] = calloc(sizeof(int), graph->numNodes);

        // check allocation
        if (graph->edges[i] == NULL) {
            destroyGraph(graph);
            return NULL;
        }
    } 
    return graph;
}

bool hasEdge(Graph *graph, unsigned int fromNode, unsigned int toNode) {
    assert(graph != NULL);
    assert(fromNode < graph->numNodes);
    assert(toNode < graph->numNodes);

    return graph->edges[fromNode][toNode];
}

bool addEdge(Graph *graph, unsigned int fromNode, unsigned int toNode, unsigned int weight) {
    assert(graph != NULL);
    assert(fromNode < graph->numNodes);
    assert(toNode < graph->numNodes);

    if (hasEdge(graph, fromNode, toNode))
        return false;

    graph->edges[fromNode][toNode] = weight;

    return true;
}

bool removeEdge(Graph *graph, unsigned int fromNode, unsigned int toNode) {
    assert(graph != NULL);
    assert(fromNode < graph->numNodes);
    assert(toNode < graph->numNodes);

    if (!hasEdge(graph, fromNode, toNode))
        return false;

    graph->edges[fromNode][toNode] = 0;

    return true;
}

void displayDotGraph(Graph *graph) {
    printf("- Display Graph:\n\n");
    printf("Writing graph.dot...\n");
        
    FILE *fptr;

    fptr = fopen("display/graph.dot","w");

    if(fptr == NULL) {
        printf("Error!");   
        exit(1);             
    }

        fprintf(fptr, "digraph {\n");
        fprintf(fptr, "rankdir=LR;\n");
        fprintf(fptr, "K=0.6;\n");
        for (int from = 0; from < graph->numNodes; from++) {
            for (int to = 0; to < graph->numNodes; to++) {
                if (graph->edges[from][to]) {
                    fprintf(fptr, "%d -> %d [label = %d];\n", from, to, graph->edges[from][to]);
                }
            }
        }
        fprintf(fptr, "}\n");

        fclose(fptr);

    printf("Generating graph.pdf...\n");
    system("dot display/graph.dot -Tpdf > display/graph.pdf");

    printf("Openning graph.pdf...\n");
    system("open display/graph.pdf");
}

void printAdjacencyMatrix(Graph *graph) {
    printf("- Adjacency Matrix:\n\n");
    printf("  ");
    for (int j = 0; j < graph->numNodes; j++)
        printf("%3d ", j);
    printf("\n");

    for (int i = 0; i < graph->numNodes; i++) {
        printf("%d ", i);
        for (int j = 0; j < graph->numNodes; j++) {
            printf("%3d ", graph->edges[i][j]);
        }
        printf("\n");
    }
}

int minDistance(Graph *graph, int dist[], bool sptSet[]) {
	int min = INT_MAX;
    int min_index;

	for (int v = 0; v < graph->numNodes; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v; 
        }
    }

	return min_index;
}

void dijkstra(Graph *graph, int dist[], int prev[], int src) {
	bool sptSet[graph->numNodes];

	for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

	dist[src] = 0;
    prev[src] = src;

	for (int count = 1; count < graph->numNodes; count++) {

		int u = minDistance(graph, dist, sptSet);
		sptSet[u] = true;

		for (int v = 0; v < graph->numNodes; v++) {
            if (!sptSet[v] && graph->edges[u][v] && dist[u] != INT_MAX && dist[u] + graph->edges[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->edges[u][v];
                prev[v] = u;
            }
        }
	}
}

void printPath(int node, int src, int prev[]) {
    while (node != src) {
        printf("%d <- ", node);
        node = prev[node];
    }
    printf("%d\n", src);
}

void printSolution(Graph *graph, int dist[], int prev[], unsigned int src) {
    printf("- Dijkstra's Algorithm Results:\n\n");
    printf("Sorce Vertex: %d\n\n", src);
	printf("Vertex \t\t Shorter Distance \t\t Path\n");
	for (int i = 0; i < graph->numNodes; i++) {
        printf("%d \t\t ", i);
		printf("%d \t\t\t\t ", dist[i]);
        printPath(i, src, prev);
    }
}

void display(Graph *graph, int dist[], int prev[], unsigned int src) {
    system("clear");
    printAdjacencyMatrix(graph);
    printf("\n\n");
	printSolution(graph, dist, prev, src);
    printf("\n\n");
    displayDotGraph(graph);
}

int main() {
    Graph *graph = createGraph(SIZE);

    unsigned int source = 0;

    int distances[graph->numNodes];
    int prev[graph->numNodes];

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 2);
    addEdge(graph, 0, 3, 3);
    addEdge(graph, 0, 4, 12);
    addEdge(graph, 0, 5, 17);

    addEdge(graph, 1, 2, 5);
    addEdge(graph, 1, 3, 7);
    addEdge(graph, 1, 4, 13);
    addEdge(graph, 1, 5, 12);

    addEdge(graph, 2, 3, 5);
    addEdge(graph, 2, 4, 2);
    addEdge(graph, 2, 5, 3);

    addEdge(graph, 3, 4, 7);
    addEdge(graph, 3, 5, 7);

    addEdge(graph, 4, 5, 9);
    addEdge(graph, 4, 0, 11);

    addEdge(graph, 5, 0, 11);
    

    dijkstra(graph, distances, prev, source);

    display(graph, distances, prev, source);

    destroyGraph(graph);

    return 0;
}