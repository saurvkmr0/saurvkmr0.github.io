#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int u;
    int v;
    int weight;
} Edge;

typedef struct {
    Edge edges[MAX_NODES * MAX_NODES];
    int num_edges;
} Graph;

typedef struct {
    int dist[MAX_NODES];
    int prev[MAX_NODES];
    int visited[MAX_NODES];
} DijkstraResult;

void initGraph(Graph *graph) {
    graph->num_edges = 0;
}

void addEdge(Graph *graph, int u, int v, int weight) {
    graph->edges[graph->num_edges].u = u;
    graph->edges[graph->num_edges].v = v;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
}

void dijkstra(Graph *graph, int start_node, DijkstraResult *result, int num_nodes) {
    for (int i = 0; i < num_nodes; i++) {
        result->dist[i] = INT_MAX;
        result->prev[i] = -1;
        result->visited[i] = 0;
    }
    result->dist[start_node] = 0;

    for (int i = 0; i < num_nodes - 1; i++) {
        int min_dist = INT_MAX;
        int u = -1;

        for (int j = 0; j < num_nodes; j++) {
            if (!result->visited[j] && result->dist[j] < min_dist) {
                min_dist = result->dist[j];
                u = j;
            }
        }

        result->visited[u] = 1;

        for (int e = 0; e < graph->num_edges; e++) {
            if (graph->edges[e].u == u) {
                int v = graph->edges[e].v;
                int weight = graph->edges[e].weight;

                if (!result->visited[v] && result->dist[u] + weight < result->dist[v]) {
                    result->dist[v] = result->dist[u] + weight;
                    result->prev[v] = u;
                }
            }
        }
    }
}

void printResult(DijkstraResult *result, int num_nodes) {
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %d: Distance %d, Previous Node %d\n", i, result->dist[i], result->prev[i]);
    }
}

int main() {
    Graph graph;
    initGraph(&graph);
    addEdge(&graph, 0, 1, 10);
    addEdge(&graph, 0, 4, 3);
    addEdge(&graph, 1, 2, 2);
    addEdge(&graph, 1, 4, 4);
    addEdge(&graph, 2, 3, 9);
    addEdge(&graph, 3, 2, 7);
    addEdge(&graph, 4, 1, 1);
    addEdge(&graph, 4, 2, 8);
    addEdge(&graph, 4, 3, 2);

    int num_nodes = 5;
    DijkstraResult result;

    dijkstra(&graph, 0, &result, num_nodes);
    printResult(&result, num_nodes);

    return 0;
}
